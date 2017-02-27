#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/fcntl.h>

#define FIFO_NAME_SRV "/tmp/server.fifo"

#define MAX_LINE 1024

#define FILE_MODE DEFFILEMODE
