#ifndef __FONTHACK_UTILITY_H__
#define __FONTHACK_UTILITY_H__

#define SafeDelte( Pointer ) \
	{ if( Pointer ){ delete Pointer; Pointer = nullptr; } }

#define SafeDeleteArray( Pointer ) \
	{ if( Pointer ){ delete [ ] Pointer; Pointer = nullptr; } }

#define SafeCloseFile( File ) \
	{ if( File ){ fclose( File ); File = nullptr; } }

bool FileExists( const char *p_pFilePath );

#endif //__FONTHACK_UTIILITY_H__

