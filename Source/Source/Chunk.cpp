#include <Chunk.h>

uint32_t WriteChunk( const uint16_t p_ChunkType, const uint64_t p_ChunkSize,
	FILE *p_pFileHandle )
{
	ZED_FILE_CHUNK Chunk { p_ChunkType, p_ChunkSize };
	
	size_t Written = fwrite( &Chunk, sizeof( Chunk ), 1, p_pFileHandle );

	if( Written != 1 )
	{
		return 1;
	}

	return 0;
}

