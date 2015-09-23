#include <libimobiledevice/libimobiledevice.h>

#define COM_SIZE 1024

int bind_tunnelserver(char *, int, int);

void *th_communication_tcp(void *);
extern void *th_communication_device(void *);
