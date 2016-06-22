#include "../chatroom.h"

int make_socket_server( int portnumber )
{
	int sockfd;
	struct sockaddr_in socketaddr;

	if( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("make_socket_server: function socket");
		exit(EXIT_FAILURE);
	}

	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(portnumber);
	socketaddr.sin_addr.s_addr = INADDR_ANY;
	
	if( bind(sockfd, (struct sockaddr *)&socketaddr, sizeof(socketaddr) ) == -1 )
	{
		perror("make_socket server: function bind");
		exit(EXIT_FAILURE);
	}
	
	return sockfd;
}
