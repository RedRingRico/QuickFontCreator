#include <Utility.h>
#include <errno.h>
#include <sys/stat.h>
#include <iostream>

bool FileExists( const char *p_pFilePath )
{
	struct stat FileStat;
	std::cout << "Testing file" << std::endl;

	if( lstat( p_pFilePath, &FileStat ) < 0 )
	{
		switch( errno )
		{
			default:
			{
				break;
			}
		}

		std::cout << "ERR" << std::endl;

		return false;
	}

	if( S_ISREG( FileStat.st_mode ) || S_ISLNK( FileStat.st_mode ) )
	{
		std::cout << "Fine" << std::endl;
		return true;
	}

	return false;
}

