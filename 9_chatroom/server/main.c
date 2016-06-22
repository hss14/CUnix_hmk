#include "../chatroom.h"

int main (void)
{
	int sock;
	FILE *logfp;
	fd_set active_fd_set, read_fd_set;
	int i;
	struct sockaddr_in clientaddr;
	size_t size;
	char timebuffer[TIMEBUFFER];
	struct cinfo *head = NULL, *node;
	char notify_disconnect[] = "SYSTEM: disconnected as you demand....(you just pressed ctrl+D)\n";
	char srv_shutdown[] = SERVER_SHUTDOWN;
	int rvalue;

	setbuf(stdout, NULL);

	/* Create the socket and set it up to accept connections. */
	sock = make_socket_server (PORT);
	if (listen (sock, 3) < 0)
	{
		perror ("server main: function listen");
		exit (EXIT_FAILURE);
	}
	printf("SERVER LAUCHED. LISTENING....\n");
	printf("YOU are the master! Input anything and hit ENTER to shutdown the server....\n");

	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
	FD_SET (sock, &active_fd_set);
	FD_SET (STDIN_FILENO, &active_fd_set);
	
	if( (logfp = fopen( SERVERLOG, "a") ) == NULL )
	{
		perror("server main: function fopen logfp");
		exit(EXIT_FAILURE);
	}

	fprintf(logfp, "\n\n===============================================\n");
	if( pr_time(timebuffer,TIMEBUFFER) == 0 )
		fprintf(logfp,"--- server launched at %s --\n", timebuffer ); 
	
	while (1)
	{
		/* Block until input arrives on one or more active sockets. */
		read_fd_set = active_fd_set;
		rvalue = select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL );
		if ( rvalue < 0 )
		{
			perror ("server main: function select");
			exit (EXIT_FAILURE);
		}
		if( rvalue == 0 )    //time out, no client there, shut down
		{
			fprintf(logfp, "time out.  server shut down.\n");
			if( pr_time(timebuffer,TIMEBUFFER) == 0 )
				fprintf(logfp,"---  %s ---\n", timebuffer );
			fprintf(logfp, "==============================================\n\n");
			break; 
		}

		/* Service all the sockets with input pending. */
		for (i = 0; i < FD_SETSIZE; i++)
			if (FD_ISSET (i, &read_fd_set))
			{
				if (i == STDIN_FILENO )
				{
					fprintf(logfp, "the master is inpatient.  server shutting down.\n");
					fgets(timebuffer, TIMEBUFFER, stdin);
					node = head;
					while(node!=NULL)
					{
						if( write(node->sockfd, srv_shutdown, sizeof(srv_shutdown) ) == -1 )
							perror("server main: notify server shutdown" );
						node = node->next;
					}
					destroy(&head);
					close(sock);
					if( pr_time(timebuffer,TIMEBUFFER) == 0 )
						fprintf(logfp,"---  %s ---\n", timebuffer );
					fprintf(logfp, "==============================================\n\n");
					exit(0);
				}
				else if (i == sock)
				{
					/* Connection request on original socket. */
					int new;
					size = sizeof (clientaddr);
					new = accept (sock, (struct sockaddr *) &clientaddr, (socklen_t *)&size);
					if (new < 0)
					{
						perror ("server main: function accept");
						exit (EXIT_FAILURE);
					}
					FD_SET (new, &active_fd_set);
					new_client( &head, new );
					fprintf (logfp, "CONNECT:\n\tfrom host %s, port %hd, %s\n\tsockfd=%d, id=%d\n", 
						inet_ntoa (clientaddr.sin_addr), ntohs (clientaddr.sin_port),
						pr_time(timebuffer, TIMEBUFFER)? " " : timebuffer, head->sockfd, head->id );
				}
				else
				{
					/* Data arriving on an already-connected socket. */
					rvalue = mesg_from_client (i, logfp, &head);
					if( rvalue < 0)
						if( write(i, notify_disconnect, sizeof(notify_disconnect) ) == -1 )
							perror("server main: write to notify disconnected clients");
					if ( rvalue )
					{
						close (i);
						search_client( head, &node, i);
						fprintf(logfp, "DISCONNECT: sockfd %d, id %d last nickname %s\n", i, node->id,
								node->nameset ? node->nickname : " "	 );
#ifdef HSSDEBUG
	printf("before delete_client\n");
#endif
						delete_client( &head, i, logfp );
#ifdef HSSDEBUG
	printf("after  delete_client\n");
#endif
						FD_CLR (i, &active_fd_set);
					}
				}
			} //end of if FD_ISSET
		fflush( logfp );

	} //end of while(1)
#ifdef HSSDEBUG
	printf("before destroy\n");
#endif
	destroy( &head );
	close(sock);
	exit(0);
}
