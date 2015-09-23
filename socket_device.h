#include <libimobiledevice/libimobiledevice.h>

#define COM_SIZE 1024

int connect_udid(idevice_t *, idevice_connection_t *, char *, int);

void *th_communication_device(void *);
