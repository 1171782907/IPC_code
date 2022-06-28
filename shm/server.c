#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main (void){
    key_t key;                  /*系统唯一性标识ipc*/
    int shmid;                  /*共享内存标识*/
    char  *shms;                /* 共享内存挂接后的地址， */   
    struct shmid_ds shmbuf;      
    pid_t p;                                        /*进程号*/
    key = ftok(".", 'a');                           /*生成系统唯一标识ipc*/
    if(key<0)
    {
        perror("ftok failed");
        return 1;
    }
    shmid = shmget(key, 1024, IPC_CREAT|0600);      /*获得共享内存,大小为1024个字节*/
     if(shmid<0)
    {
        perror("shmget failed");
        return 2;
    }
    shms = (char *)shmat(shmid, NULL, 0);           /*挂接共享内存*/
    printf("shmat success!\n");

    while(strlen(shms)==0){
        sleep(1);
        printf("%s\n",shms);
    }

    shmdt(shms);                                /*摘除共享内存*/
    printf("shmdt success!\n");
    shmctl(shmid,IPC_RMID,&shmbuf);             /*删除共享内存*/
    return 0;
}
 



