#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 10000

void client(int, int);
void server(int, int);

int main(int argc, char** argv)
{
	int pipe_1[2], pipe_2[2];
	pid_t child_pid;

	if (pipe(pipe_1) != 0)
	{
		printf("pipe pipe_1 failed!\n");
		return -1;
	}
	if (pipe(pipe_2) != 0)
	{
		printf("pipe pipe_2 failed!\n");
		return -1;
	}


	if ((child_pid = fork()) == 0)
	{
		// child process	
		close(pipe_1[1]);
		close(pipe_2[0]);

		printf("process[%d]: start to run!!\n", getpid());

		server(pipe_1[0], pipe_2[1]);

		printf("process[%d]: stop to run!!\n", getpid());
		exit(0);
	}
	else
	{
		// parent process	
		close(pipe_1[0]);
		close(pipe_2[1]);

		printf("process[%d]: start to run!!\n", getpid());

		client(pipe_2[0], pipe_1[1]);
		waitpid(child_pid, NULL, 0);

		printf("process[%d]: stop to run!!\n", getpid());
		exit(0);
	}

	return 0;	
}

void client(int read_fd, int write_fd)
{
	size_t len;
	ssize_t n;
	char buff[MAX_LINE];

	fgets(buff, MAX_LINE, stdin);
	printf("fgets result: buff=[%s]", buff);
	len = strlen(buff);

	if (buff[len-1] == '\n')
	{
		len--;
	}

	write(write_fd, buff, len);

	while ((n = read(read_fd, buff, MAX_LINE)) > 0)
	{
		printf("client[%d]:[%s]", getpid(), buff);	
	}
}

void server(int read_fd, int write_fd)
{
	int fd;
	ssize_t n;
	char buff[MAX_LINE+1];

	if ((n = read(read_fd, buff, MAX_LINE)) == 0)
	{
		printf("read no lines from read_fd!!!\n");	
		return;
	}

	printf("read [%ld]:[%s] from read_fd\n", n, buff);
	buff[n] = '\0';

	if ((fd = open(buff, O_RDONLY)) < 0)
	{
		snprintf(buff + n, sizeof(buff) - n, "cannot open file!!\n");
		n = strlen(buff);
		write(write_fd, buff, n);
	}
	else
	{
		printf("open file success\n");
		while ((n = read(fd, buff, MAX_LINE) > 0))	
		{
			printf("buff=[%s]\n", buff);
			printf("n=[%ld]", n);
			write(write_fd, buff, n);
		}
		close(fd);
	}
}


