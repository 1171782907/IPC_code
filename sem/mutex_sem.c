#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

static char msg[]="你好,共享内存！\n";
static char msg1[]="你不是很好,共享内存！\n";
typedef int sem_t;
union semun{                /*信号量操作的联合结构*/
    int val;                /*整型变量*/
    struct semid_ds *buf;   /*semid_ds 结构指针*/
    unsigned short *array; /*数组类型*/

}arg;                       /*定义一个全局变量*/             


void SetvalueSem (sem_t semid, int value){ /*设置信号量的值*/
    union semun sem;        /*信号量操作的结构*/
    sem.val = value;        /*值初始化*/
    semctl(semid, 0, SETVAL, sem);/*设置信号量的值*/
}


int GetvalueSem (sem_t semid){ /*获得信号量的值*/
    union semun sem;    /*信号量操作的结构*/
    return semctl(semid, 0, GETVAL, sem);/*获得信号量的值*/
}




sem_t CreateSem (key_t key, int value)   /*建立信号量,系统唯一标识key和信号量的初始值value*/
{   
    union semun sem;                    /*信号量结构变量*/
    sem_t semid;                        /*信号量ID*/
    sem.val = value;                    /*设置信号量初始值*/
    semid = semget(key, 1, IPC_CREAT|0600);     /*创建1个信号量，获得信号量的ID*/
    if (-1 == semid)                            /*获得信号量ID失败*/
    {
        printf("create semaphore error\n");      /*打印信息*/
        perror("reason");
        return -1;                              /*返回错误*/
    }
    semctl(semid, 0, SETVAL, sem);             /*发送命令,操作编号为0的信号量，信号量初始值为value*/
    return semid;                              /*返回建立的信号量*/    
}

void DestroySem (sem_t semid){                  /*销毁信号量*/
    union semun sem;                            /*信号量操作的结构*/
    sem.val = 0;                                /*信号量值的初始化*/
    semctl(semid, 0, IPC_RMID, sem);            /*设置信号量*/
}

int Sem_P(sem_t semid){                             /*减少信号量*/
    struct sembuf sops={0,-1,SEM_UNDO};           /*建立信号量结构值*/
    return (semop(semid, &sops, 1));                /*发送命令*/

}
int Sem_V (sem_t semid){                            /*增加信号量值*/
    struct sembuf sops={0,+1, SEM_UNDO};          /*建立信号量结构值*/
    return (semop(semid, &sops, 1));                /*发送信号量操作方法*/
}


int main (void){
    key_t key;                  /*系统唯一性标识ipc*/
    int semid, shmid;           /*信号量，共享内存标识*/
    char  *shms, *shmc;         /* shms 服务器共享内存挂接后的地址， shmc 客户端共享内存挂接后的地址*/
    struct semid_ds buf;        /*信号量管理结构信息存储*/      
    struct shmid_ds shmbuf;      
    char buffer[80];
    pid_t p;                                        /*进程号*/
    key = ftok(".", 'a');                           /*生成系统唯一标识ipc*/
    shmid = shmget(key, 1024, IPC_CREAT|0600);      /*获得共享内存,大小为1024个字节*/
    semid = CreateSem(key, 1);                      /*建立信号量,信号量初始值为0*/
    p = fork();
    if (p > 0){                                     /*父进程*/
        shms = (char *)shmat(shmid, 0,0);           /*挂接共享内存*/
        char * cur = shms;
        Sem_P(semid); 
        while(*cur != '\0'){
            cur += 1;
        }
        printf("父进程数据开始写入共享内存!\n");
        memcpy(cur, msg, strlen(msg) +1);          /*复制内容*/
        printf("父进程查看共享内存的值为:%s\n",shms);           /*打印信息*/
        Sem_V(semid);                               /*增加信号量*/
        
        sleep(3);
        shmdt(shms);                                /*摘除共享内存*/
        DestroySem(semid);                         /*销毁信号量*/
        shmctl(shmid,IPC_RMID,&shmbuf);
    } else if(p == 0){                              /*子进程*/
        
        shmc = (char *)shmat(shmid, 0,0);           /*挂接共享内存*/
        char * cur = shmc;
        Sem_P(semid); 
        while(*cur != '\0'){
            cur += 1;
        }
        printf("子进程数据开始写入共享内存!\n");
        memcpy(cur, msg1, strlen(msg1) +1);          /*复制内容*/
        printf("子进程查看共享内存的值为:%s\n",shmc);           /*打印信息*/
        Sem_V(semid);                               /*增加信号量*/
       
        sleep(3);
        shmdt(shmc);                                /*摘除共享内存*/
        DestroySem(semid);                         /*销毁信号量*/
        shmctl(shmid,IPC_RMID,&shmbuf);
    }
    return 0;
}
 



