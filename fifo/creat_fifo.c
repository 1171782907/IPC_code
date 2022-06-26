#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
int main()
{
    int ret = mkfifo("./myfifo",0600);  // 创建命名管道文件，用户权限为可读、可写
	if(ret == -1){                      //判断是否创建失败
		printf("mkfifo creat failed");  //打印失败信息
		perror("failed reason");        //打印失败原因
	}
	return 0;
}