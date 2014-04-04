#include <Utility.h>
#include <errno.h>
#include <sys/stat.h>
#include <iostream>

bool FileExists( const char *p_pFilePath )
{
	struct stat FileStat;

	if( lstat( p_pFilePath, &FileStat ) < 0 )
	{
		switch( errno )
		{
			default:
			{
				break;
			}
		}

		return false;
	}

	if( S_ISREG( FileStat.st_mode ) || S_ISLNK( FileStat.st_mode ) )
	{
		return true;
	}

	return false;
}

