#include "../chatroom.h"

int main (void)
{
	int sock;
	struct sockaddr_in srvaddr;
	int secnum = 1;
	fd_set active_fd_set, read_fd_set;
	char broadcast[MAXMSG + MAXNAME + MAXNAME];
	struct c_mesg mymessage;
	char srv_shutdown[] = SERVER_SHUTDOWN;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT);
	srvaddr.sin_addr.s_addr = inet_addr(SRVADDR);	
	
	setbuf(stdout, NULL);
	printf("TRYING TO CONNECT THE SERVER.....\n");

	while( connect( sock, (struct sockaddr *)&srvaddr, sizeof(srvaddr) ) == -1 )
	{
		sleep(secnum);
		secnum <<= 1;
		if( secnum > MAXSLEEP )
		{
			printf("failed to connect to server! exit...\n");
			exit(0);
		}
	}

	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
	FD_SET (sock, &active_fd_set);
	FD_SET (STDIN_FILENO , &active_fd_set);
	
	printf("\nCONNECTION SETUP!\n");
	printf("input: \"set nickname(newline)YOURNICKNAME\" to change your nickname\n");
	printf("input: \"exit\" or press ctrl+d to exit the chatroom\n");
	printf("all of your input that doesn't conform to any of the above formats will be considered as your message and be broadcasted to everyone else\n");
	printf("now have fun!\n===================================================================\n\n");

	while (1)
	{
		/* Block until input arrives on one or more active sockets. */
		read_fd_set = active_fd_set;
		if (select (sock+1, &read_fd_set, NULL, NULL, NULL) < 0)
		{
			perror ("client main: function select");
			exit (EXIT_FAILURE);
		}

		/* Service all the sockets with input pending. */
		if ( FD_ISSET (sock, &read_fd_set) )
		{
			/* echo to stdout*/
			if( read( sock, broadcast, MAXMSG+2*MAXNAME ) > 0 )
				printf( "%s", broadcast );
			if( strncmp( broadcast, srv_shutdown, strlen(srv_shutdown) ) == 0 )
			{
				// server is shutting down
				printf("server is shutting down by the master.\nExit.\n");
				shutdown(sock, SHUT_RDWR);
				exit(0);
			}
		}
		else if ( FD_ISSET (STDIN_FILENO, &read_fd_set) ) 
		{
			/* decide message type, construct message, send to sock */
			if( ( fgets( broadcast, sizeof(broadcast), stdin) == NULL )
				|| ( strncmp( broadcast, "exit", 4) == 0  )  )
			{
				mymessage.type = 3;
				mymessage.mesg[0]=' ';
				mymessage.mesg[1]='\0';
				write( sock, &mymessage, sizeof(mymessage) );
				printf("you just pressed exit(ctrl+d), now disconnect to server and exit...\n");
				sleep(3);
				shutdown(sock, SHUT_RDWR);
				exit(0);
			}
			if( strncmp( broadcast, "set nickname", 12 ) == 0 )  
			{
				fgets( broadcast, MAXNAME, stdin);
				mymessage.type = 1;
				strncpy( mymessage.mesg, broadcast, MAXNAME );
			}
			else
			{
				mymessage.type = 2;
				strncpy( mymessage.mesg, broadcast, MAXMSG-1 ); 
				mymessage.mesg[MAXMSG-1] = '\0';
			}
			write( sock, &mymessage, sizeof(mymessage) );
		}

	} //end of while(1)
	shutdown(sock, SHUT_RDWR);
	exit(0);
}
