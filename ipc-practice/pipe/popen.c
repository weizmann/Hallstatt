#include <stdio.h>

#define MAX_LINE 1024

int main(int argc, char** argv)
{
	size_t n;
	char path_name[MAX_LINE], command[MAX_LINE];
	FILE* fp;

	if (fgets(path_name, MAX_LINE, stdin) == NULL)
	{
		printf("error: read path name error, return!!!\n");
		return -1;
	}

	n = strlen(path_name);
	while (path_name[n-1] == '\n')
	{
		printf("path name[%d] is line-break\n", n-1);
		n--;
	}

	snprintf(command, sizeof(command), "cat %s", path_name);
	fp = popen(command, "r");

	printf("after popen: fp=[%p]\n", fp);

	fp = NULL;
	char output_buffer[MAX_LINE];
	while (fgets(output_buffer, MAX_LINE, fp) != NULL)
	{
		fputs(output_buffer, stdout);
	}

	pclose(fp);
	return 0;
}
