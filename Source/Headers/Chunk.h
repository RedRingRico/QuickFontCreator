#ifndef __QUICKFONTCREATOR_CHUNK_H__
#define __QUICKFONTCREATRO_CHUNK_H__

#include <stdint.h>
#include <cstdio>

const uint16_t ZED_CHUNK_FONT_GLYPH		= 0x0100;
const uint16_t ZED_CHUNK_FONT_TEXTURE	= 0x0200;

const uint32_t ZED_CHUNK_END	= 0xFFFF;

#pragma pack( 1 )

typedef struct __ZED_FILE_CHUNK
{
	uint16_t	Type;
	uint64_t	Size;
}ZED_FILE_CHUNK;

#pragma pack( )

uint32_t WriteChunk( const uint16_t p_ChunkType, const uint64_t p_ChunkSize,
	FILE *p_pFileHandle );

#endif // __QUICKFONTCREATOR_CHUNK_H__

