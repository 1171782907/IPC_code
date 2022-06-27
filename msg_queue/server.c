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

 


int main (int argc,char *argv[]){
    int ret = -1;
    int msg_flags, msg_id;          
    struct msgmbuf{                 /*消息的缓冲区结构*/
        long mtype;
        char mtext[100];
    };

    struct msqid_ds msg_info;
    struct msgmbuf msg_mbuf;

    int msg_rflags;
    msg_flags = IPC_CREAT;             /*消息队列的类型*/
    msg_id = msgget(0x610e8d71, msg_flags | 0666);    /*建立消息队列*/
    if(-1 == msg_id){
        printf("消息队列建立失败\n!");
        perror("建立失败原因：");
        return 0;
    }

    msg_show_attr(msg_id, msg_info);            /*打印msqid_ds信息 显示消息队列的属性*/
    msg_mbuf.mtype = 3;                        /*消息类型设置为10*/
    
    // msg_rflags =  0;   //设置为阻塞
    msg_rflags = IPC_NOWAIT|MSG_NOERROR;                    /*消息队列接受函数标志设置为非阻塞 并且信息大于不报错，直接截取*/
    ret = msgrcv(msg_id, &msg_mbuf, sizeof(msg_mbuf.mtext), msg_mbuf.mtype ,msg_rflags);    /*接收消息*/
    if(-1== ret){
         printf("接收消息失败\n");
         perror("失败原因：");
         
    }else{
        printf("接收消息成功,长度: %d\n",ret);
    }
    
    msg_show_attr(msg_id, msg_info);                       /*打印msqid_ds信息 显示消息队列的属性*/
    // msg_info.msg_perm.uid = 8;
    // msg_info.msg_perm.gid = 8;
    // msg_info.msg_qbytes = 12345;
    // ret = msgctl(msg_id, IPC_SET, &msg_info);             /*设置消息属性*/
    // if( -1 == ret){
    //     printf("设置消息属性失败\n!");
    //     perror("属性设置失败原因：");
    //     return 0;
    // }
    // msg_show_attr(msg_id, msg_info);                       /*打印msqid_ds信息 显示消息队列的属性*/
    ret = msgctl (msg_id, IPC_RMID, NULL);                  /*删除消息队列*/
    if( -1 == ret){
        printf("删除消息队列失败\n");
        perror("删除消息队列失败原因：");
        return 0;
    }
    return 0;
}
