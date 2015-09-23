#include <stdio.h>
#include <stdlib.h>

#include "misc.h"
#include "socket_tcp.h"

int main(int argc, char *argv[])
{
	if(argc < 3) {
		fprintf(stderr, "usage : %s [iphone_port] [local_port]\n", argv[0]);
		return -1;
	}

	char **devices_list;
	uint32_t de_count, i;

	if(idevice_get_device_list(&devices_list, &de_count)) {
		fprintf(stderr, "error : idevice_get_device_list()\n");
		return -1;
	}

	if(de_count <= 0) {
		fprintf(stderr, "there are no avaliable devices\n");
		return -1;
	}
	
	printf("========================================== %d devices ==========================================\n", de_count);
	for(i = 0; i < de_count; i++)
		printf("%d. [udid : %s]\n", i + 1, devices_list[i]);
	printf("===============================================================================================\n");

	printf("devices no? : ");
	scanf("%d", &i);
	
	if(i > de_count || i < 1) {
		fprintf(stderr, "no fuck\n");
		return -1;
	}

	bind_tunnelserver(devices_list[i - 1], atoi(argv[1]), atoi(argv[2]));

	idevice_device_list_free(devices_list);
	return 0;
}

void init_snippet(comm_snippet_t snippet)
{
	snippet->device = (idevice_t)0;
	snippet->device_connection = (idevice_connection_t)0;

	snippet->c_sockfd = 0;
	snippet->is_end = 0;
}

void *free_device(void *args)
{
	pthread_t *t_tcp, *t_device;

	t_tcp = ((comm_snippet_t)args)->t_tcp;
	t_device = ((comm_snippet_t)args)->t_device;

	while(!((comm_snippet_t)args)->is_end);
	if(!pthread_cancel(*t_tcp) && !pthread_cancel(*t_device)) {
		idevice_disconnect(((comm_snippet_t)args)->device_connection);
		idevice_free(((comm_snippet_t)args)->device);

		free(((comm_snippet_t)args)->tcp_buf);
		free(((comm_snippet_t)args)->device_buf);

		free(((comm_snippet_t)args)->t_tcp);
		free(((comm_snippet_t)args)->t_device);

		free(((comm_snippet_t)args));
		printf("[free all of memory for snippet is success]\n");
	}
	else 
		fprintf(stderr, "Error occur during cancel threads\n");
}

void sign_free(comm_snippet_t snippet)
{
	snippet->is_end = 1;
}
