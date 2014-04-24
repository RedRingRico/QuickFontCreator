#ifndef __FONTHACK_FONTFILE_H__
#define __FONTHACK_FONTFILE_H__

#include <stdint.h>
#include <string>

const uint16_t CHUNK_FONT_GLYPH		= 0x0100;
const uint16_t CHUNK_FONT_TEXTURE	= 0x0200;

const size_t FILE_OUTPUT_BUFFER_SIZE = 1024*1024;

typedef enum __ZED_FONT_TYPE
{
	BITMAP_FONT = 1,
	VECTOR_FONT,
	SIGNED_DISTANCE_FONT
}FONT_TYPE;

typedef enum __ZED_FONT_BACKGROUNDMODE
{
	COLOURKEY_BACKGROUND = 1,
	SOLID_BACKGROUND,
	TRANSPARENT_BACKGROUND,
	TRANSLUCENT_BACKGROUND
}FONT_BACKGROUNDMODE;

typedef struct _ZED_COLOUR
{
	float Red;
	float Green;
	float Blue;
	float Alpha;
}ZED_COLOUR;

#pragma pack( 1 )
typedef struct __FONTHEADER
{
	char				ID[ 4 ];
	FONT_TYPE			Type;
	FONT_BACKGROUNDMODE	BackgroundMode;
	ZED_COLOUR			ColourKey;
}FONTHEADER;

typedef struct __GLYPHHEADER
{
	uint32_t	Count;
	uint32_t	TextureWidth;
	uint32_t	TextureHeight;
}GLYPHHEADER;

typedef struct __TARGA_HEADER
{
	unsigned char	IDLength;
	unsigned char	ColourmapType;
	unsigned char	ImageType;
	unsigned char	ColourmapSpecification[ 5 ];
	uint16_t		X;
	uint16_t		Y;
	uint16_t		Width;
	uint16_t		Height;
	unsigned char	BitsPerPixel;
	unsigned char	ImageDescription;
}TARGA_HEADER,*LPTARGA_HEADER;
#pragma pack( )

class FontFile
{
public:
	FontFile( );
	~FontFile( );

	uint32_t SetFilePath( const std::string p_FilePath );
	std::string GetFilePath( ) const;

	uint32_t SetTargaFile( const std::string p_TargaFile );
	std::string GetTargaFile( ) const;

	uint32_t SetGlyphFile( const std::string p_GlyphFile );
	std::string GetGlyphFile( ) const;

	uint32_t Write( );

private:
	FontFile( const FontFile &p_Copy );
	FontFile &operator=( const FontFile &p_Copy );

	std::string m_FilePath;
	std::string m_TargaFile;
	std::string m_GlyphFile;

	GLYPHHEADER	m_GlyphHeader;
};

#endif

