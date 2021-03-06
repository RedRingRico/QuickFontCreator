#include <FontFile.h>
#include <Utility.h>
#include <cstring>
#include <iostream>
#include <GlyphSet.h>
#include <Chunk.h>

FontFile::FontFile( )
{
}

FontFile::~FontFile( )
{
}

uint32_t FontFile::SetFilePath( const std::string p_FilePath )
{
	m_FilePath = p_FilePath;

	return 0;
}

std::string FontFile::GetFilePath( ) const
{
	return m_FilePath;
}

uint32_t FontFile::SetTargaFile( const std::string p_TargaFile )
{
	if( !FileExists( p_TargaFile.c_str( ) ) )
	{
		return 1;
	}

	m_TargaFile = p_TargaFile;

	return 0;
}

std::string FontFile::GetTargaFile( ) const
{
	return m_TargaFile;
}

uint32_t FontFile::SetGlyphFile( const std::string p_GlyphFile )
{
	if( !FileExists( p_GlyphFile.c_str( ) ) )
	{
		return 1;
	}

	m_GlyphFile = p_GlyphFile;

	return 0;
}

uint32_t FontFile::Write( )
{
	if( m_FilePath.empty( ) )
	{
		return 1;
	}

	FILE *pOutputFile = nullptr;
	pOutputFile = fopen( m_FilePath.c_str( ), "wb" );

	if( pOutputFile == nullptr )
	{
		return 1;
	}

	// Write out the header, followed by the glyphs, then the font texture
	FONTHEADER Header;
	memset( &Header, 0, sizeof( Header ) );

	Header.ID[ 0 ] = 'Z';
	Header.ID[ 1 ] = 'E';
	Header.ID[ 2 ] = 'D';
	Header.ID[ 3 ] = 'F';

	Header.Type = BITMAP_FONT;

	fwrite( &Header, sizeof( Header ), 1, pOutputFile );
	if( !m_TargaFile.empty( ) )
	{
		unsigned char *pTargaData =
			new unsigned char[ FILE_OUTPUT_BUFFER_SIZE ];
		memset( pTargaData, 0, FILE_OUTPUT_BUFFER_SIZE );

		FILE *pTargaFile = fopen( m_TargaFile.c_str( ), "rb" );

		fseek( pTargaFile, SEEK_SET, SEEK_END );

		size_t BytesRead = ftell( pTargaFile );
		rewind( pTargaFile );

		if( BytesRead <= sizeof( TARGA_HEADER ) )
		{
			SafeCloseFile( pTargaFile );
			SafeCloseFile( pOutputFile );
			std::cout << "<ERROR> Targa file is no larger or even smaller "
				"than a Targa header" << std::endl;

			return 1;
		}

		TARGA_HEADER TargaHeader;
		size_t ReadHeader =
			fread( &TargaHeader, sizeof( TARGA_HEADER ), 1, pTargaFile );

		if( ReadHeader != 1 )
		{
			SafeCloseFile( pTargaFile );
			SafeCloseFile( pOutputFile );
			std::cout << "<ERROR> Failed to read header" << std::endl;

			return 1;
		}

		rewind( pTargaFile );

		m_GlyphHeader.TextureWidth = TargaHeader.Width;
		m_GlyphHeader.TextureHeight = TargaHeader.Height;

		std::cout << "Writing " << m_TargaFile << " as Targa to disk ..." <<
			std::endl;

		// Write start chunk
		if( WriteChunk( ZED_CHUNK_FONT_TEXTURE, BytesRead, pOutputFile ) != 0 )
		{
			SafeCloseFile( pTargaFile );
			SafeCloseFile( pOutputFile );
			std::cout << "<ERROR> Failed to write Targa start chunk" <<
				std::endl;

			return 1;
		}
		else
		{
			bool WriteFailed = false;
			bool ReadFailed = false;

			while( BytesRead > 0 )
			{
				size_t BytesToRead = 0;

				if( FILE_OUTPUT_BUFFER_SIZE > BytesRead )
				{
					BytesToRead = BytesRead;
					BytesRead -= BytesRead;
				}
				else
				{
					BytesToRead = FILE_OUTPUT_BUFFER_SIZE;
					BytesRead -= FILE_OUTPUT_BUFFER_SIZE;
				}

				size_t Read = fread( pTargaData, sizeof( unsigned char ),
					BytesToRead, pTargaFile );

				if( Read != BytesToRead )
				{
					std::cout << "<ERROR> Failed to read Targa file" <<
						std::endl;
					std::cout << "\tExpected " << BytesToRead << " bytes, "
						"received " << Read << " bytes" << std::endl;
					ReadFailed |= true;

					break;
				}

				size_t Written = fwrite( pTargaData, sizeof( unsigned char ),
					BytesToRead, pOutputFile );

				if( Written != BytesToRead )
				{
					std::cout << "<ERROR> Failed to write Targa file" <<
						std::endl;
					std::cout << "\tExpected " << BytesToRead << " bytes, "
						"recieved " << Written << " bytes" << std::endl;
					WriteFailed |= true;

					break;
				}
			}


			SafeDeleteArray( pTargaData );

			if( WriteFailed || ReadFailed )
			{
				SafeCloseFile( pTargaFile );
				SafeCloseFile( pOutputFile );
				std::cout << "Incomplete" << std::endl;

				return 1;
			}
			else
			{
				std::cout << "Complete" << std::endl;
			}

			if( WriteChunk( ZED_CHUNK_END, 0, pOutputFile ) != 0 )
			{
				SafeCloseFile( pTargaFile );
				SafeCloseFile( pOutputFile );
				std::cout << "<ERROR> Failed to write end chunk for Targa" <<
					std::endl;

				return 1;
			}
		}
	}

	if( !m_GlyphFile.empty( ) )
	{
		GlyphSet Glyphs;

		Glyphs.ReadFromFile( m_GlyphFile );

		m_GlyphHeader.Count = Glyphs.GetCount( );

		if( WriteChunk( ZED_CHUNK_FONT_GLYPH, Glyphs.GetSizeOnDisk( ) +
			sizeof( m_GlyphHeader ), pOutputFile ) != 0 )
		{
			SafeCloseFile( pOutputFile );
			std::cout << "<ERROR> Failed to write glyph chunk" << std::endl;

			return 1;
		}
		else
		{
			size_t WriteSize =
				fwrite( &m_GlyphHeader, sizeof( GLYPHHEADER ), 1, pOutputFile );
			if( WriteSize != 1 )
			{
				SafeCloseFile( pOutputFile );
				std::cout << "<ERROR> Failed to write glyph count" <<
					std::endl;
				
				return 1;
			}

			if( m_GlyphHeader.Count == 0 )
			{
				std::cout << "<WARNING> No glyphs have been processed" <<
					std::endl;
			}
			else
			{
				std::cout << "Writing " << m_GlyphHeader.Count << " glyphs ..." <<
					std::endl;

				bool WriteFailed = false;

				for( size_t i = 0; i < m_GlyphHeader.Count; ++i )
				{
					GLYPH_METRICS Glyph;
					if( Glyphs.GetGlyph( i, &Glyph ) != 0 )
					{
						std::cout << "<ERORR> Failed to write glyph " << i <<
							std::endl;
						WriteFailed |= true;

						break;
					}
					
					size_t Written = fwrite( &Glyph, sizeof( Glyph ), 1,
						pOutputFile );

					if( Written != 1 )
					{
						std::cout << "<ERROR> Failed to write glyph " << i <<
							" to disk" << std::endl;
						WriteFailed |= true;

						break;
					}

					std::cout << "Glyph " << i << " written to disk" <<
						std::endl;
				}

				if( WriteFailed )
				{
					std::cout << "Incomplete" << std::endl << std::endl;
				}
				else
				{
					std::cout << "Complete" << std::endl << std::endl;
				}
			}

			if( WriteChunk( ZED_CHUNK_END, 0, pOutputFile ) != 0 )
			{
				SafeCloseFile( pOutputFile );
				std::cout << "<ERROR> Failed to write end chunk for glyphs" <<
					std::endl;

				return 1;
			}
		}
	}

	if( WriteChunk( ZED_CHUNK_END, 0, pOutputFile ) != 0 )
	{
		SafeCloseFile( pOutputFile );
		std::cout << "<ERROR> Failed to write the final end chunk" <<
			std::endl;

		return 1;
	}

	std::cout << "END" << std::endl;

	SafeCloseFile( pOutputFile );

	return 0;
}

