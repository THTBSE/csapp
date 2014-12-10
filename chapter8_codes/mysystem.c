#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

int mysystem(char *command)
{
	if (command == NULL)
		return 1;
	pid_t pid;
	int status;
	
	if ((pid = fork() < 0))
		status = -1;
	else if (pid == 0)
	{
		execl("/bin/sh","sh","-c",command,NULL);
	}
	else
	{
		while (waitpid(pid,&status,0) < 0)
		{
			if (errno != EINTR)
				status = -1;
			break;
		}
		
	}
	
	return status;
}


int main()
{
	int status;
	char *cmd = "date";
	printf("mysystem version\n");
	status = mysystem(cmd);
	printf("return value is %d\n", status);
	
	printf("system version\n");
	status = system(cmd);
	printf("return value is %d\n", status);
	
	return 0;
}