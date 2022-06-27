#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
 

void msg_show_attr(int msg_id, struct msqid_ds msg_info){/*打印消息属性的函数*/
    int ret = -1;
    sleep(1);
    ret = msgctl(msg_id, IPC_STAT, &msg_info);           /*获取消息队列的信息*/
    if( -1 == ret){
        printf ("获得消息信息失败!\n");
        return ;
    }
   
    printf("*******************消息队列属性信息如下********************************\n");    /*以下打印消息的信息*/
   
    printf("现在队列中的字节数: %ld\n",msg_info.msg_cbytes);             /*消息队列中的字节数*/
    printf("队列中消息数: %ld\n",msg_info.msg_qnum);                   /*消息队列中的消息数*/
    printf("队列中最大字节数: %ld\n",msg_info.msg_qbytes);              /*消息队列中的最大字节数*/
    printf("最后发送消息的进程pid: %d\n",msg_info.msg_lspid);          /*最后发送消息的进程*/
    printf("最后接收消息的进程 pid: %d\n",msg_info.msg_lrpid);         /*最后接收消息的进程*/
    printf("最后发送消息的时间: %s",ctime(&(msg_info.msg_stime)));     /*最后发送消息的时间*/
    printf("最后接收消息的时间: %s",ctime(& (msg_info.msg_rtime)));    /*最后接收消息的时间*/
    printf("最后变化时间: %s",ctime (& (msg_info.msg_ctime)));          /*消息的最后变化时间*/
    printf("消息UID是: %d\n",msg_info.msg_perm.uid);                   /*消息的UID*/
    printf("消息GID是: %d\n",msg_info.msg_perm.gid);                    /*消息的GID*/
}
int main (void){
    int ret = -1;
    int msg_flags, msg_id;          
    key_t key;                      //定义消息队列唯一，描述符
    struct msgmbuf{                 /*消息的缓冲区结构*/
        long mtype;
        char mtext[100];
    };

    struct msqid_ds msg_info;
    struct msgmbuf msg_mbuf;

    int msg_sflags, msg_rflags;
    char *msgpath = ".";            /*key产生所用的路径, 当前目录*/
    key = ftok (msgpath, 'a');      /*产生key*/
    if (key != -1){                 /*产生key成功*/
        printf ("成功建立 KEY\n");
    }else{                          /*产生key失败*/
        printf("建立 KEY失败\n");
    }
    msg_flags = IPC_CREAT|IPC_EXCL;             /*消息队列的类型*/
    msg_id = msgget (key, msg_flags | 0666);    /*建立消息队列*/
    if(-1 == msg_id){
        printf("消息队列建立失败\n!");
        return 0;
    }

    msg_show_attr(msg_id, msg_info);            /*打印msqid_ds信息 显示消息队列的属性*/
    msg_sflags = IPC_NOWAIT;                    /*消息队列发送函数标志设置为非阻塞*/
    
    memcpy(msg_mbuf.mtext,"jjjjjj from hbw", sizeof ("jjjjj from hbw"));                /*复制字符串*/
    
    for(int i = 0; i<10; i++){
        msg_mbuf.mtype = i + 1;     /*消息类型设置*/
        ret = msgsnd(msg_id, &msg_mbuf, sizeof ("jjjjjj from hbw"), msg_sflags);                       /*发送消息*/
        if( -1 == ret){
            printf("发送消息失败\n");
        }
    }
    msg_show_attr(msg_id, msg_info);            /*打印msqid_ds信息 显示消息队列的属性*/
    return 0;
}