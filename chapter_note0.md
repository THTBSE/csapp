CS:APP Chapter 8
================

UNIX系统调用函数总结
------------------

### fork
	pid_t fork(void);
fork()创建一个子进程，一次调用返回两次。一次返回在父进程中，返回子进程的
PID,如果创建失败则返回-1；另一次在子进程中返回0，用于辨别程序是出于父进
程还是子进程。如

	pid_t pid;
	if ((pid = fork()) == 0)
		printf("child process\n");
	else if (pid == -1)
		printf("error\n");
	else
		printf("parent process\n");
子进程得到与父进程相同虚拟地址空间的一份拷贝，子进程还可以读写父进程中打开
的任何文件。*创建子进程后，子进程与父进程执行的顺序是不一定的。*

### waitpid
	pid_t waitpid(pid_t pid, int *status, int options);
		返回：调用成功则返回子进程PID，如果WNOHANG,则为0， 如果其他错
		误返回-1.

等待成员的集合通过参数pid给定。
如果pid > 0,等待集合则为一个单独的ID等于pid的子进程。
如果pid = -1,等待集合就是父进程的所有子进程。

options 默认为0， 如果想修改默认行为，可以将options设为WNOHANG与
WUNTRACED的各种组合。详见CS:APP page 496

status用于检查已回收子进程的推出状态。

