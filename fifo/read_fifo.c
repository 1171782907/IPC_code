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
	char buf[100] = {0};  //用于保存读取的管道数据

	int fd = open("./myfifo",O_RDONLY);	 //打开管道文件，模式为只读， 默认阻塞
	// int fd = open("./myfifo",O_RDONLY| O_NONBLOCK);	 //打开管道文件，模式为只读， 非阻塞
     if(fd == -1){
        printf("open failed!");
        perror("failed reason: ");
    }
	int n_read = read(fd,buf,100);      //读取管道数据 存入buf
    if(n_read == -1){
        perror("failed reason: ");
    }
	printf("read %d byte from myfifo, info is \":%s\" \n",n_read,buf); //打印读取的管道数据
	close(fd);              //关闭文件描述符

	return 0;
}
