#ifndef __QUICKFONTCREATOR_GLYPHSET_H__
#define __QUICKFONTCREATOR_GLYPHSET_H__

#include <list>
#include <stdint.h>
#include <string>

typedef struct __GLYPH_METRICS
{
	unsigned char	Character;
	uint32_t		X;
	uint32_t		Y;
	uint32_t		Width;
	uint32_t		Height;
}GLYPH_METRICS;

class GlyphSet
{
public:
	GlyphSet( );
	~GlyphSet( );

	uint32_t ReadFromFile( const std::string p_FileName );

private:
	std::list< GLYPH_METRICS > m_GlyphMetrics;
};

#endif // __QUICKFONTCRATOR_GLYPH_H__
