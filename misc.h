#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>

#include <libimobiledevice/libimobiledevice.h>

struct comm_snippet {
	idevice_t device;
	idevice_connection_t device_connection;

	int c_sockfd;
	int is_end;

	pthread_t *t_tcp;
	pthread_t *t_device;

	char *tcp_buf;
	char *device_buf;
};

typedef struct comm_snippet comm_snippet;
typedef comm_snippet *comm_snippet_t;

void init_snippet(comm_snippet_t);
void *free_device(void *);
void sign_free(comm_snippet_t);
