#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#include "misc.h"
#include "socket_tcp.h"

void *th_communication_tcp(void *args)
{
	idevice_t device;
	idevice_connection_t device_connection;

	int c_sockfd, len, sent_bytes, r;

	device = ((comm_snippet_t)args)->device;
	device_connection = ((comm_snippet_t)args)->device_connection;
	c_sockfd = ((comm_snippet_t)args)->c_sockfd;

	char *buf = (char *)malloc(sizeof(char) * COM_SIZE);
	((comm_snippet_t)args)->tcp_buf = buf;
	while(1) {
		if((len = recv(c_sockfd, buf, COM_SIZE - 1, 0)) > 0) {
			r = idevice_connection_send(device_connection, buf, len, &sent_bytes);
			if(r) {
				sign_free(((comm_snippet_t)args));
			}
		}
		else if(!len) {
			sign_free(((comm_snippet_t)args));
		}
	}
	return 0;
}

int bind_tunnelserver(char *udid, int iport, int lport)
{
	struct sockaddr_in s_addr, c_addr;	

	uint32_t len;
	int s_sockfd, c_sockfd;

	idevice_t device;
	idevice_connection_t device_connection;
	pthread_t *t_tcp, *t_device, t_free;

	comm_snippet_t snippet;

	s_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(lport);
	s_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(s_sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr))) { 
		fprintf(stderr, "bind() error\n");
		return -1;
	}
	if(listen(s_sockfd, 1024)) {
		fprintf(stderr, "listen() error\n");
		return -1;
	}
	
	printf("[%s : listen for tunnel server is success]\n", __FUNCTION__);
	while(1) {
		snippet = (comm_snippet_t)malloc(sizeof(comm_snippet));
		init_snippet(snippet);

		len = sizeof(c_addr);
		snippet->c_sockfd = accept4(s_sockfd, (struct sockaddr *)&c_addr, &len, SOCK_NONBLOCK);

		if(snippet->c_sockfd != -1) 
			printf("[New Client]\n");

		connect_udid(&(snippet->device), &(snippet->device_connection), udid, iport);

		t_tcp = (pthread_t *)malloc(sizeof(pthread_t));
		t_device = (pthread_t *)malloc(sizeof(pthread_t));
		snippet->t_tcp = t_tcp; snippet->t_device = t_device;

		pthread_create(t_tcp, NULL, th_communication_tcp, (void *)snippet);
		pthread_create(t_device, NULL, th_communication_device, (void *)snippet);
		pthread_create(&t_free, NULL, free_device, (void *)snippet);
	}

	return 0;
}
			
