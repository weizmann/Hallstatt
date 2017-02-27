#include "consts.h"

#include <string.h>
#include <stdio.h>

int main(int argc, const char** argv)
{
	int fifo_read, fifo_write;
	size_t len;
	ssize_t n;
	char *ptr, fifo_name[MAX_LINE], buffer[MAX_LINE];
	pid_t pid = getpid();
	snprintf(fifo_name, sizeof(fifo_name), "/tmp/fifo.%ld", (long)pid);

	if ((mkfifo(fifo_name, FILE_MODE) < 0) && (errno != EEXIST))
	{
		printf("make fifo error: file_name=[%s]", fifo_name);
		return -1;
	}

	snprintf(buffer, sizeof(buffer), "%ld", (long)pid);
	len = strlen(buffer);
	ptr = buffer + len;

	if (fgets(ptr, MAX_LINE - len, stdin) == NULL)
	{
		printf("fgets error, return!!!");
	}
	else
	{
		printf("ptr=[%s]", ptr);
	}
	fifo_write = open(FIFO_NAME_SRV, O_WRONLY, 0);
	write(fifo_write, buffer, strlen(buffer));

	fifo_read = open(fifo_name, O_RDONLY, 0);
	while ((n = read(fifo_read, buffer, MAX_LINE)) > 0)
	{
		write(stdout, buffer, n);
	}

	close(fifo_read);
	close(fifo_name);

	return 0;
}
