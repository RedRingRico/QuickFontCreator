#include <FontFile.h>
#include <Utility.h>
#include <cstring>
#include <iostream>
#include <GlyphSet.h>

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

	FILE *pOutput = nullptr;
	pOutput = fopen( m_FilePath.c_str( ), "wb" );

	if( pOutput == nullptr )
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

	fwrite( &Header, sizeof( Header ), 1, pOutput );


	if( !m_GlyphFile.empty( ) )
	{
		GlyphSet Glyphs;

		Glyphs.ReadFromFile( m_GlyphFile );

		size_t GlyphCount = Glyphs.GetCount( );

		if( GlyphCount == 0 )
		{
			std::cout << "<WARNING> No glyphs have been processed" <<
				std::endl;
		}
		else
		{
			std::cout << "Writing " << GlyphCount << " glyphs ..." <<
				std::endl;

			bool WriteFailed = false;

			for( size_t i = 0; i < GlyphCount; ++i )
			{
				GLYPH_METRICS Glyph;
				if( Glyphs.GetGlyph( i, &Glyph ) != 0 )
				{
					std::cout << "<ERORR> Failed to write glyph " << i <<
						std::endl;
					WriteFailed |= true;

					break;
				}
				
				size_t Written = fwrite( &Glyph, sizeof( Glyph ), 1, pOutput );

				if( Written != 1 )
				{
					std::cout << "<ERROR> Failed to write glyph " << i <<
						" to disk" << std::endl;
					WriteFailed |= true;

					break;
				}

				std::cout << "Glyph " << i << " written to disk" << std::endl;
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
	}

	if( !m_TargaFile.empty( ) )
	{
		unsigned char *pTargaData =
			new unsigned char[ FILE_OUTPUT_BUFFER_SIZE ];
		memset( pTargaData, 0, FILE_OUTPUT_BUFFER_SIZE );

		FILE *pTargaFile = fopen( m_TargaFile.c_str( ), "rb" );

		fseek( pTargaFile, SEEK_SET, SEEK_END );

		size_t BytesRead = ftell( pTargaFile );
		rewind( pTargaFile );

		std::cout << "Writing " << m_TargaFile << " as Targa to disk ..." <<
			std::endl;

		// Write start chunk
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
				std::cout << "<ERROR> Failed to read Targa file" << std::endl;
				std::cout << "\tExpected " << BytesToRead << " bytes, "
					"received " << Read << " bytes" << std::endl;
				ReadFailed |= true;

				break;
			}

			size_t Written = fwrite( pTargaData, sizeof( unsigned char ),
				BytesToRead, pOutput );

			if( Written != BytesToRead )
			{
				std::cout << "<ERROR> Failed to write Targa file" << std::endl;
				std::cout << "\tExpected " << BytesToRead << " bytes, "
					"recieved " << Written << " bytes" << std::endl;
				WriteFailed |= true;

				break;
			}
		}

		// Write end chunk
		SafeDeleteArray( pTargaData );

		if( WriteFailed || ReadFailed )
		{
			fclose( pOutput );
			std::cout << "Incomplete" << std::endl;

			return 1;
		}
		else
		{
			std::cout << "Complete" << std::endl;
		}
	}

	fclose( pOutput );

	return 0;
}

