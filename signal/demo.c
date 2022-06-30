#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void printfPending(sigset_t *pending)
{
    int i=1;
    for(;i<32;i++)
    {
        if(sigismember(pending,i))
        {
            printf("1");
        }
        else
            printf("0");
    }
    printf("\n");
}

int main()
{
    sigset_t set,oset;       //定义两个信号集    
    sigemptyset(&set);       //初始化信号集， 清空信号集，使所有标志位置0
    sigemptyset(&oset);
    sigaddset(&set,2);      //将要操作的信号编号加入set信号集 2代表SIGINT(crtl + c 发出信号)
    sigprocmask(SIG_SETMASK,&set,&oset);//将当前的set设置为信号屏蔽字，并将之前的内核信号屏蔽字保存到oset
    sigset_t pending;       //用于保存系统的pending表
    int count=0;
    while(1)
    {
        sigemptyset(&pending);      //初始化，使所有标志位置0
        sigpending(&pending);       //拿到系统中的pending表保存到pending中
        printfPending(&pending);    //打印系统的pending表
        sleep(2);
        count++;
        if(count>=5)                //打印5次
        {
            sigprocmask(SIG_SETMASK,&oset,NULL);//将之前的信号屏蔽字重新设为block表，2号信号不会被阻塞，递达给系统，进程被终止。
        }
    }
    return 0;
}