#include "freport.h"

int traverse( char *pathname, int myfun(), struct timespec *now, int opt )
{
	DIR *dirp;
	struct dirent *pentry;
	struct stat buf;
	char *fullname;
	char *symb_content;
	static int fullen = 128;
	int pathlen, filelen;

#ifdef HSSDEBUG
	fprintf(stdout, "start traverse : %s: ", pathname);
#endif

	if( ( lstat(pathname, &buf) ) == -1 )
	{
		fprintf( stderr, "error get stat info: %s\n", pathname );
		return -1;
	}


	if( S_ISDIR(buf.st_mode) )
	{
#ifdef HSSDEBUG
		fprintf(stdout, "\n==== directory ====\n");
#endif
		if( hashcheck( buf.st_ino ) )   // has been traversed
			return 0;

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

			traverse( fullname, myfun, now, opt );
			
		}
		free(fullname);	
		closedir(dirp);	
	}
	else if( S_ISLNK(buf.st_mode) )
	{
		symb_content = malloc( buf.st_size + 1 );
		if( readlink( pathname, symb_content, buf.st_size+1) != buf.st_size )
		{
			fprintf(stderr, "READLINK ERROR %s\n", pathname);
			return -2;
		}
		symb_content[buf.st_size] = '\0';
			
		if( (symb_content[0]!='/') && (symb_content[0]!='.') )  //relative path
		{
			pathlen = strlen( pathname ); // exclulde \0
			fullname = malloc(pathlen + buf.st_size + 2);

			strcpy( fullname, pathname );
			strcpy( strrchr(fullname, '/'), "/\0" );
			strcat( fullname, symb_content ); 

#ifdef HSSDEBUG
			fprintf(stdout, "\nGot symb_name = %s\n", fullname );
#endif
			traverse( fullname, myfun, now, opt );
			free(fullname);
		}
		else
		{
#ifdef HSSDEBUG
			fprintf(stdout, "\nUse symb_content = %s\n", symb_content);
#endif
			traverse( symb_content, myfun, now, opt );
		}
		free( symb_content);
	}
	else
	{
		if( hashcheck( buf.st_ino ) )   // has been traversed
		{
#ifdef HSSDEBUG
			fprintf(stdout, "skipping......has been traversed\n");
#endif
			return 0;
		}
		myfun( pathname, &buf, now, opt );
	}


	return 0;

}


