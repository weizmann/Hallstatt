#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void read_and_write_message(int fd, const char* message, const char* tag)
{
	char result_buffer[1024];
	while (1)
	{
		write(fd, message, strlen(message));
		sleep(1);

		ssize_t ret = read(fd, result_buffer, sizeof(result_buffer) - 1);
		if (ret > 0)
		{
			result_buffer[ret] = '\0';
			printf("[%s received message]: %s\n", tag, result_buffer);
		}
		else
		{
			printf("%s encounter error: ret=[%zd]", tag, ret);
		}
	}
}

int main()
{
	int duplex_fd[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, duplex_fd) < 0)
	{
		printf("make socket pair failed!!!");
		return -1;
	}
	if (fork() == 0)
	{
		close(duplex_fd[1]);
		read_and_write_message(duplex_fd[0], "message sent from child process", "child-process");
	}
	else
	{
		close(duplex_fd[0]);
		read_and_write_message(duplex_fd[1], "message sent from parent process", "parent-process");
	}
	return 0;
}
