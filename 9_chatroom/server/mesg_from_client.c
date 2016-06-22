#include "../chatroom.h"

int mesg_from_client (int filedes, FILE *logfp, struct cinfo **phead)
{
	struct c_mesg msg;
	int nbytes;
	char notify_toolong[] = "! SYSTEM NOTIFICATION: name too long! Nickname set yet truncated to 255 characters.\n";
	char notify_noname[] = "! SYSTEM NOTIFICATIOM: no name part found in your message! failed to change nickname!\n";
	char notify_succeed[] = "! SYSTEM NOTIFICATION: succeed changing nickname!:)\n";
	int rvalue;
	struct cinfo *node;
	char broadcast[MAXMSG + MAXNAME + MAXNAME];

	nbytes = read ( filedes, &msg, sizeof(msg) );
	if (nbytes < 0)
	{
		/* Read error. */
		perror ("read_from_client");
		exit (EXIT_FAILURE);
	}
	else if (nbytes == 0)
		/* End-of-file. */
		return -1;
	else
	{
		if( msg.type == 1 )   // change nickname of client from socket fd == filedes
		{
			rvalue = change_name(filedes, msg.mesg, phead, logfp);
			if( rvalue == 1 ) //truncated
				write(filedes, notify_toolong, sizeof(notify_toolong) );
			else if( rvalue == -1)
				write(filedes, notify_noname, sizeof(notify_noname) );
			else if( rvalue == 0 )
				write(filedes, notify_succeed, sizeof(notify_succeed) );
			else
				return -1;
			return 0;
		}
		else if(msg.type == 2)  // send message to all clients except socket fd == filedes
		{
			if( search_client( *phead, &node, filedes) == -1 )
			{
				fprintf(logfp, "\nERROR: cannot find fd %d in client list, therefore failed to broadcast\n", filedes);
				return -1;
			}
			sprintf(broadcast,"%d(%s):\n\t%s\n", node->id, node->nameset? node->nickname:" ", msg.mesg );
			fprintf(logfp,"\nMSG from %d(%s), fd=%d:\n\t%s\n", node->id, 
				node->nameset ? node->nickname : " ", node->sockfd, msg.mesg );

			node = *phead;
			while( node != NULL )
			{
				if( node->sockfd != filedes )
					write( node->sockfd, broadcast, MAXMSG+2*MAXNAME );
				node = node->next;
			}
		}
		else if(msg.type == 3 )    //client want to leave
		{
			return 1;
		}

		return 0;
	}
}
