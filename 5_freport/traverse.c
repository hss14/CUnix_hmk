#include "freport.h"

int traverse( char *pathname, int myfun(), struct timespec *now, int opt, const char *startpoint )
{
	DIR *dirp;
	struct dirent *pentry;
	struct stat buf;
	char *fullname;
	char *symb_name;
	static int fullen = 128;
	int pathlen, filelen;

	fullname = malloc(fullen);
	pathlen = strlen( pathname ); // exclulde \0

	if( ( dirp = opendir( pathname ) ) == NULL )
	{
		fprintf( stderr, "error open dir: %s\n", pathname);
		return(-1);
	}

	while( ( pentry = readdir(dirp) ) != NULL )
	{
		if( ( strcmp( pentry->d_name, ".") == 0 ) || ( strcmp(pentry->d_name, "..") == 0 ) )
			continue;
		filelen = strlen( pentry->d_name );  // null terminated; strlen exclude \0
		while( fullen < pathlen + filelen + 2 )
		{
			fullen = 2 * fullen;
			fullname = realloc( fullname, fullen );
		}
		strcpy( fullname, pathname );
		strcat( fullname, "/\0");
		strcat( fullname, pentry->d_name ); 

		if( ( lstat(fullname, &buf) ) == -1 )
		{
			fprintf( stderr, "error get stat info: %s\n", fullname );
			continue;
		}

		if( buf.st_nlink > 1)
		{
			if( hashcheck( buf.st_ino ) )   // has been traversed
				continue;
		}

		if( S_ISDIR(buf.st_mode) )
		{
			traverse( fullname, myfun, now, opt, startpoint );
		}
		else if( S_ISLNK(buf.st_mode) )
		{
			symb_name = malloc( buf.st_size + 1 );
			if( readlink( fullname, symb_name, buf.st_size+1) != buf.st_size )
			{
				fprintf(stderr, "READLINK ERROR %s\n", fullname);
				continue;
			}
			symb_name[buf.st_size] = '\0';
			
			if( buf.st_size >= strlen(startpoint) )
			{	
				if( strncmp( symb_name, startpoint, strlen(startpoint) ) == 0 ) 
					// symbolic link to within startpoint, ignore
					continue;
			}
			else
			{
				
			}

			free( symb_name);
		}
		else
		{
			myfun( fullname, &buf, now, opt );
		}
	}

	free(fullname);
	closedir(dirp);
	return 0;

}


