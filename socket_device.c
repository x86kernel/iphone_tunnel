#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>

#include "misc.h"
#include "socket_device.h"

int connect_udid(idevice_t *device, idevice_connection_t *device_connection, char *udid, int port)
{
	uint32_t c_handle;
	int r;

	if((r = idevice_new(device, udid))) {
		fprintf(stderr, "%s : error until idevice_new() [code : %d]\n", __FUNCTION__, r);
		return -1;
	}

	if((r = idevice_connect(*device, port, device_connection))) {
		fprintf(stderr, "%s : error until idevice_connect() [code : %d]\n", __FUNCTION__, r);
		do {
			fprintf(stderr, "reconnect to device next 3 sec\n");
			sleep(3);

			if(idevice_get_handle(*device, &c_handle)) {
				idevice_free(*device);
				idevice_new(device, udid); 
			}
			r = idevice_connect(*device, port, device_connection);
		}while(r);
	}
	return 0;
}

void *th_communication_device(void *args)
{
	pthread_mutex_t mutex;

	idevice_t device;
	idevice_connection_t device_connection;

	uint32_t c_handle;
	int c_sockfd, sent_bytes, r;

	device = ((comm_snippet_t)args)->device;
	device_connection = ((comm_snippet_t)args)->device_connection;;
	c_sockfd = ((comm_snippet_t)args)->c_sockfd;

	char *buf = (char *)malloc(sizeof(char) * COM_SIZE);
	((comm_snippet_t)args)->device_buf = buf;
	while(1) {
		// idevice_connection_receive is not blocking function
		r = idevice_connection_receive(device_connection, buf, COM_SIZE - 1, &sent_bytes);
		if(sent_bytes > 0) {
			sent_bytes = send(c_sockfd, buf, sent_bytes, 0);
			if(sent_bytes == -1) {
				sign_free(((comm_snippet_t)args));
			}
		}
		else {
			if(r) {
				if(idevice_get_handle(device, &c_handle)) {
					// device detach 
					fprintf(stderr, "Not yet..\n");
					return 0;	
				}
				sign_free(((comm_snippet_t)args));
			}
		}
	}
	return 0;
}
