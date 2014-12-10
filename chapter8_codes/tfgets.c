#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

sigjmp_buf jbuf;

void handler(int sig)
{
	siglongjmp(jbuf,1);
}


char* tfgets(char *buf, int maxCount, FILE *file)
{
	signal(SIGALRM,handler);
	printf("You have only 5s for input\nTime is begin\n");
	alarm(5);
	char *result = NULL;
	if (!sigsetjmp(jbuf,1))
	{
		result = fgets(buf,maxCount,file);
	}
	else
	{
		printf("Time is out!\n");
		return result;
	}

	return result;
}

int main()
{
	char temp[MAXSIZ];
	char *input = tfgets(temp,MAXSIZ,stdin);
	if (input)
	    printf("Your input is %s\n",input);
	else
	    printf("Your input nothing!\n");
	return 0;	
}
