#include "freport.h"

int traverse( char *pathname, int myfun(), struct timespec *now, int opt, const char *startpoint, int check4symb )
{
	DIR *dirp;
	struct dirent *pentry;
	struct stat buf;
	char *fullname;
	char *symb_name;
	static int fullen = 128;
	int pathlen, filelen;

	if( ( lstat(pathname, &buf) ) == -1 )
	{
		fprintf( stderr, "error get stat info: %s\n", pathname );
		return -1;
	}

	if( (buf.st_nlink > 1) || ( check4symb == 1 ) )
	{
		if( hashcheck( buf.st_ino ) )   // has been traversed
			return 0;
	}


	if( S_ISDIR(buf.st_mode) )
	{
		if( ( dirp = opendir( pathname ) ) == NULL )
		{
			fprintf( stderr, "error open dir: %s\n", pathname);
			return(-3);
		}

		fullname = malloc(fullen);
		pathlen = strlen( pathname ); // exclulde \0

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

			traverse( fullname, myfun, now, opt, startpoint, 0 );
			
		}
		free(fullname);	
		closedir(dirp);	
	}
	else if( S_ISLNK(buf.st_mode) )
	{
		symb_name = malloc( buf.st_size + 1 );
		if( readlink( pathname, symb_name, buf.st_size+1) != buf.st_size )
		{
			fprintf(stderr, "READLINK ERROR %s\n", pathname);
			return -2;
		}
		symb_name[buf.st_size] = '\0';
			
		if( buf.st_size >= strlen(startpoint) )
		{	
			if( strncmp( symb_name, startpoint, strlen(startpoint) ) == 0 ) 
				// symbolic link to dir/file within startpoint, ignore
				return 0;
		}
		else
			traverse( symb_name, myfun, now, opt, startpoint, 1 );

		free( symb_name);
	}
	else
	{
		myfun( pathname, &buf, now, opt );
	}


	return 0;

}


