#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char *str = "I am writting data to fifo";   //需要写入管道的数据

	// int fd = open("./myfifo",O_WRONLY);         //打开管道文件，模式为只写， 默认阻塞
	int fd = open("./myfifo",O_WRONLY | O_NONBLOCK);         //打开管道文件，模式为只写， 非阻塞
    if(fd == -1){
        printf("open failed!");
        perror("failed reason: ");
    }
    
	int num = write(fd,str,strlen(str));   //向管道写数据 
    if(num == -1){
        perror("failed reason: ");
    }
	printf("%d",num);
    close(fd);                   //关闭文件描述符
	
	return 0;
}