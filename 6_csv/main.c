#include "csv6.h"

//test
int main( int argc, char *argv[] )
{
	int vreturn;
	if( argc < 3 )
	{
		printf("illegal argument!\n");
		printf("for transform csv file to bin file, use command:  \"./csv6 t name_of_csv_file name_of_bin_file\"\n");
		printf("for print out contents of bin flie, use command:  \"./csv6 p name_of_bin_file\"\n");
		exit(0);
	}
	
	// argc >= 3
	if( argv[1][0] == 't' )  //preform csv2vin transform
	{
		if( argc < 4 )
		{
			printf("argument filename_bin missing!\nlegal example for transform:\n");
			printf("./csv6 t filename_csv filename_bin\n");
			exit(0);
		}
		if( (vreturn = csv2bin( argv[2], argv[3] ) ) != 0 )
			printf("error occur csv2bin(%d)\n", vreturn );
	}

	if( argv[1][0] == 'p' )  //preform csv2vin transform
	{
		if( (vreturn = pr_bin( argv[2] ) ) != 0 )
			printf("\nerror occur pr_bin(%d)\n", vreturn );
	}

	exit(0);
}
