#include <GlyphSet.h>
#include <iostream>
#include <Utility.h>

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

	std::cout << FileSize << std::endl;

	fread( pFileString, sizeof( char ), FileSize, pGlyphFile );

	FileAsString = pFileString;

	SafeDeleteArray( pFileString );

	// Create substrings to process
	std::list< std::string > GlyphFileLines;

	size_t NewLine = FileAsString.find_first_of( "\n" );

	std::cout << "Reading lines..." << std::endl;
	std::cout << FileAsString << std::endl;
/*
	while( NewLine != std::string::npos )
	{
		size_t LineStart = NewLine;
		NewLine = FileAsString.find_first_of( "\n" );

		std::string Line = FileAsString.substr( LineStart, NewLine-LineStart );

		std::cout << Line << std::endl;
	}*/

	size_t FileIterator = 0;

	while( FileIterator != FileSize )
	{
		++FileIterator;
	}

	fclose( pGlyphFile );

	return 0;
}

