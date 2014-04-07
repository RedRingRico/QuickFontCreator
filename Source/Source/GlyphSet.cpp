#include <GlyphSet.h>
#include <iostream>
#include <Utility.h>
#include <cstring>
#include <cstdlib>

GlyphSet::GlyphSet( )
{
}

GlyphSet::~GlyphSet( )
{
}

uint32_t GlyphSet::ReadFromFile( const std::string p_FileName )
{
	// The format should be:
	// Character <whitespace> X Offset <whitespace> Y Offset <whitespace>
	// Width <whitespace> Height<line break>
	// For each glyph
	
	// Read in the whole file
	FILE *pGlyphFile = fopen( p_FileName.c_str( ), "rb" );

	if( pGlyphFile == nullptr )
	{
		return 1;
	}

	fseek( pGlyphFile, SEEK_SET, SEEK_END );
	std::string FileAsString;
	size_t FileSize = ftell( pGlyphFile );
	rewind( pGlyphFile );

	FileAsString.reserve( FileSize );

	char *pFileString = new char[ FileSize ];

	fread( pFileString, sizeof( char ), FileSize, pGlyphFile );

	fclose( pGlyphFile );

	FileAsString = pFileString;

	SafeDeleteArray( pFileString );

	// Create substrings to process
	std::list< std::string > GlyphFileLines;

	size_t NewLine = FileAsString.find_first_of( "\n" );
	size_t StartCharacter = 0;

	std::cout << "Reading lines..." << std::endl;

	while( NewLine != std::string::npos )
	{
		NewLine = FileAsString.find_first_of( "\n", StartCharacter );

		std::string Line = FileAsString.substr( StartCharacter,
			NewLine - StartCharacter );

		if( NewLine != std::string::npos )
		{
			StartCharacter = NewLine + 1;
			
			if( !Line.empty( ) )
			{
				GlyphFileLines.push_back( Line );
			}
		}
	}

	std::list< std::string >::const_iterator LineItr = GlyphFileLines.begin( );
	size_t LineCounter = 0;

	while( LineItr != GlyphFileLines.end( ) )
	{
		if( this->ProcessLine( ( *LineItr ) ) != 0 )
		{
			std::cout << "<ERROR> Could not process line " << LineCounter <<
				std::endl << "\t" << ( *LineItr ) << std::endl;
			return 1;
		}

		++LineCounter;
		++LineItr;
	}

	return 0;
}

uint32_t GlyphSet::ProcessLine( const std::string p_Line )
{
	if( p_Line.empty( ) )
	{
		return 1;
	}

	GLYPH_METRICS Metrics;
	memset( &Metrics, 0, sizeof( Metrics ) );

	Metrics.Character = p_Line[ 0 ];

	size_t StartChar = 0, EndChar = 0;

	Metrics.X = this->NextNumberUL( p_Line, &StartChar, &EndChar );
	Metrics.Y = this->NextNumberUL( p_Line, &StartChar, &EndChar );
	Metrics.Width = this->NextNumberUL( p_Line, &StartChar, &EndChar );
	Metrics.Height = this->NextNumberUL( p_Line, &StartChar, &EndChar );

	std::cout << "Glyph metrics" << std::endl;
	std::cout << "\tCharacter: " << Metrics.Character << std::endl;
	std::cout << "\tX:         " << Metrics.X << std::endl;
	std::cout << "\tY:         " << Metrics.Y << std::endl;
	std::cout << "\tWidth:     " << Metrics.Width << std::endl;
	std::cout << "\tHeight:    " << Metrics.Height << std::endl;

	return 0;
}

uint32_t GlyphSet::NextNumberUL( const std::string p_Line,
	size_t *p_pStartChar, size_t *p_pEndChar ) const
{
	uint32_t Number = 0;
	size_t StartChar = 0;
	size_t EndChar = 0;

	if( ( *p_pStartChar ) != 0 )
	{
		StartChar = ( *p_pStartChar );
	}

	if( ( *p_pEndChar ) != 0 )
	{
		EndChar = ( *p_pEndChar );
	}

	StartChar = p_Line.find_first_of( " \t", EndChar );
	++StartChar;
	EndChar = p_Line.find_first_of( " \t", StartChar );

	if( EndChar == std::string::npos )
	{
		EndChar = p_Line.size( );
	}

	std::string NumberStr = p_Line.substr( StartChar, EndChar - StartChar );

	Number = strtoul( NumberStr.c_str( ), nullptr, 10 );

	( *p_pStartChar ) = StartChar;
	( *p_pEndChar ) = EndChar;

	return Number;
}

