#include <Main.h>
#include <iostream>
#include <Arguments.h>
#include <string>
#include <FontFile.h>
#include <GitVersion.h>

int main( int p_Argc, char **p_ppArgv )
{
	// Okay, so this very quick throw-away tool is going to work like this:
	// Read the arguments from the command line:
	// 	-t = .tga texutre to use
	// 	-g = .glyph text descriptor
	// 	-b = Background mode (colourkey, solid, transparent, or translucent)
	// 	-c = Colour key (only if background mode is colourkey)
	//
	// Glyph files are stored as:
	// 	Glyph - char-based, used for translating the string used for rendering
	// 	Width -		For now, the glyphs are very simple and only the width and
	// 	Hieght -	height are considered when rendering them (baselines and
	// 				advances are ignored)
	// 	X Offset - The X and Y offsets determine where in the texture the
	// 	Y Offset - glyph's top-left corner starts
	
	if( p_Argc == 1 )
	{
		PrintUsage( p_ppArgv[ 0 ] );
		return 0;
	}

	FontFile Output;

	std::cout << "ZED Quick Font Creator | Ver. " << GIT_BUILD_VERSION <<
		std::endl;

	for( int i = 1; i < p_Argc; ++i )
	{
		uint32_t Argument = GetArgumentType( p_ppArgv[ i ] );

		switch( Argument )
		{
			case ARGUMENT_TGA:
			{
				if( ( i + 1 ) > ( p_Argc - 1 ) )
				{
					std::cout << "Error, no argument given for the texture" <<
						std::endl;

					return 1;
				}

				++i;

				if( Output.SetTargaFile( p_ppArgv[ i ] ) )
				{
					std::cout << "Error, file not valid" << std::endl;
					return 1;
				}

				break;
			}
			case ARGUMENT_GLYPH:
			{
				if( ( i + 1 ) > ( p_Argc - 1 ) )
				{
					std::cout << "Error, no glyph file given" << std::endl;

					return 1;
				}

				++i;

				if( Output.SetGlyphFile( p_ppArgv[ i ] ) )
				{
					std::cout << "Error, glyph file not valid" << std::endl;
					return 1;
				}

				break;
			}
			case ARGUMENT_BACKGROUND:
			{
				break;
			}
			case ARGUMENT_COLOURKEY:
			{
				break;
			}
			default:
			{
				std::cout << "<ERROR> Unknown argument type: " <<
					p_ppArgv[ i ] << std::endl;

				break;
			}
		}
	}

	Output.SetFilePath( "test.zed" );

	Output.Write( );

	return 0;
}

void PrintUsage( const char *p_pProgram )
{
	std::cout << "Usage:" << std::endl;
	std::cout << "\t" << p_pProgram << " <options> zedfontfile" << std::endl;
	std::cout << std::endl << "Options:" << std::endl;
	std::cout << "\t-g <glyph file>" << std::endl;
	std::cout << "\t-t <targa file>" << std::endl;
	std::cout << "\t-b <background mode>" << std::endl;
	std::cout << "\t-c <colour key>" << std::endl << std::endl;
	std::cout << "Background mode may be one of:" << std::endl;
	std::cout << "\tcolourkey" << std::endl;
	std::cout << "\tsolid" << std::endl;
	std::cout << "\ttranparent" << std::endl;
	std::cout << "\ttranslucent" << std::endl;
}

