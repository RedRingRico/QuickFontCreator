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
		std::cout << "Would be writing glyph data..." << std::endl;
		GlyphSet Glyphs;

		Glyphs.ReadFromFile( m_GlyphFile );
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

		// Write start chunk

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

			fwrite( pTargaData, sizeof( unsigned char ), BytesToRead,
				pOutput );
		}

		// Write end chunk
		SafeDeleteArray( pTargaData );
	}

	fclose( pOutput );

	return 0;
}

