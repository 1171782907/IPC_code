#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int fd[2];       //匿名管道描述符
	pid_t pid;       //进程号
	char buf[128]; 	 //数据缓冲区
	int ret;         //创建管道结果
	ret = pipe(fd);
	if(ret == -1){
		printf("pipe creat  failed\n");
		return -1;
	}

	pid = fork();

	if(pid < 0){
		printf("child thread creat failed\n");
		return -1;
	}
	else if(pid > 0){                  // pid>0当前位于父进程中
		printf("this is father thread!\n");
		close(fd[0]);				   //父进程关闭读端
		write(fd[1],"I am your father!",strlen("I am your father!"));   //父进程像管道写入数据
		wait(NULL);						//等候子进程结束，父进程完成资源回收，阻塞。
	}else{								// pid==0当前位于子进程中
		printf("this is child thread!\n");		
		close(fd[1]);					//子进程关闭写端
		read(fd[0],buf,128);			//子进程读取管道数据，并写入buf中
		printf("father's info:%s\n",buf);
		exit(0);						//子进程结束
	}
	
	return 0;
}
