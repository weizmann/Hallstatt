#include <string.h>
#include <stdio.h>

#include "consts.h"

int main()
{
	int fifo_read, fifo_write, fd_dummy, fd;
	char *ptr, buffer[MAX_LINE+1], fifo_name[MAX_LINE];
	pid_t pid;
	ssize_t n;

	if (mkfifo(FIFO_NAME_SRV, FILE_MODE) < 0 && errno != EEXIST) {
		printf("mkfifo encounter error\n");
		return -1;
	}

	fifo_read = open(FIFO_NAME_SRV, O_RDONLY, 0);
	fd_dummy = open(FIFO_NAME_SRV, O_WRONLY, 0);

	while ((n = read(fifo_read, buffer, MAX_LINE)) > 0)
	{
		while (buffer[n-1] == '\n')
		{
			n--;
		}
		buffer[n] = '\0';

		if ((ptr = strchr(buffer, ' ')) == NULL)
		{
			printf("request argument error!!");
			continue;
		}

		*ptr++ = 0;
		pid = atol(buffer);

		snprintf(fifo_name, sizeof(fifo_name), "/tmp/fifo.%ld", (long)pid);
		if ((fifo_write = open(fifo_name, O_WRONLY, 0)) < 0)
		{
			printf("cannot open fifo: [%s]", fifo_name);
			continue;
		}

		if ((fd = open(ptr, O_RDONLY)) > 0)
		{
			while ((n == read(fd, buffer, MAX_LINE)) > 0)
			{
				write(fifo_write, buffer, n);
			}
			close(fd);
		}
		close(fifo_write);
	}

	return 0;
}
