#ifndef __H_CHATROOM__
#define __H_CHATROOM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include<time.h>
#include<sys/time.h>

#define PORT    6666
#define SRVADDR "127.0.0.1"
#define SERVERLOG "chatroom.log"
#define SERVER_SHUTDOWN "SYSTEM: server is shuting down. please exit.\n"
#define MAXSLEEP 60
#define MAXMSG  2048
#define MAXNAME 256
#define TIMEBUFFER 256

struct c_mesg {
	int type;
	char mesg[MAXMSG];
};

struct cinfo {
	int sockfd;
	int id;
	char nickname[MAXNAME];
	int nameset;
	struct cinfo *next;
};

int mesg_from_client( int filedes, FILE *logfp, struct cinfo **phead);

void new_client( struct cinfo **phead, int fd);
void destroy(struct cinfo **phead);
int delete_client( struct cinfo **phead, int fd, FILE *logfp);
int search_client( const struct cinfo *head, struct cinfo **ppresult, int fd);
int change_name(int fd, char *s, struct cinfo **phead, FILE *logfp);

int make_socket_server(int portnumber);
// return sockfd;

int pr_time(char *buf, size_t maxsize);
//print readable time string into buf, return 0 if success, -1 otherwise 

#endif
