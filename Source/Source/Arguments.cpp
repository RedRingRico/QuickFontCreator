#include <Arguments.h>
#include <string>

uint32_t GetArgumentType( const char *p_pArgument )
{
	std::string Argument( p_pArgument );

	if( Argument[ 0 ] != '-' )
	{
		return ARGUMENT_INVALID;
	}

	if( Argument.compare( "-t" ) == 0 )
	{
		return ARGUMENT_TGA;
	}

	if( Argument.compare( "-g" ) == 0 )
	{
		return ARGUMENT_GLYPH;
	}

	if( Argument.compare( "-b" ) == 0 )
	{
		return ARGUMENT_BACKGROUND;
	}

	if( Argument.compare( "-c" ) == 0 )
	{
		return ARGUMENT_COLOURKEY;
	}

	return ARGUMENT_INVALID;
}

