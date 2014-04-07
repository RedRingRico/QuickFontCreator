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

	size_t GetCount( ) const;

	uint32_t GetGlyph( const size_t p_Index, GLYPH_METRICS *p_pGlyph ) const;

private:
	std::list< GLYPH_METRICS > m_GlyphMetrics;

	uint32_t ProcessLine( const std::string p_Line );
	uint32_t NextNumberUL( const std::string p_Line, size_t *p_pStartChar,
		size_t *p_pEndChar ) const;
};

#endif // __QUICKFONTCRATOR_GLYPH_H__

