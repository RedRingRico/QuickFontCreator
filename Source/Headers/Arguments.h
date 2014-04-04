#ifndef __FONTHACK_ARGUMENTS_H__
#define __FONTHACK_ARGUMENTS_H__

#include <stdint.h>

const uint32_t ARGUMENT_TGA			= 1;
const uint32_t ARGUMENT_GLYPH		= 2;
const uint32_t ARGUMENT_BACKGROUND	= 3;
const uint32_t ARGUMENT_COLOURKEY	= 4;
const uint32_t ARGUMENT_INVALID		= 256;

uint32_t GetArgumentType( const char *p_pArgument );

#endif

