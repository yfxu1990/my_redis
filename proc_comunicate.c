#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/shm.h>
#include "cl_shm.h"

/*
进程间通讯
1:父进程打开的文件，子进程是否可以直接读写，资源是否共享？
    父进程打开的文件子进程不用重复打开，直接就可以读写，父进程close()
    不会影响子进程的使用，证明内核对象未关闭

*/
int test_filedesc_main(int argc, char *argv[]){
    pid_t pid;
    int p_fd;
    if((p_fd = open("parent.log", O_CREAT|O_WRONLY)) < 0){
        perror("in parent open log faild \n");
    }
    write(p_fd, "i am parent \n", strlen("i am parent \n"));
    if((pid = fork()) < 0)
        return;
    else if(pid > 0){
        //parent
        write(p_fd, "after fork i am parent \n", strlen("after fork i am parent \n"));
        struct stat file_s;
        fstat(p_fd, &file_s);
        printf("parent file_s.st_ino %d", file_s.st_ino);
        fflush(stdout);
        sleep(6);
        close(p_fd);
        wait();
        write(p_fd, "guess child already write\n", strlen("guess child already write\n"));
        perror("");
    }
    else{
           sleep(10);
           struct stat file_s;
           fstat(p_fd, &file_s);
           printf("child file_s.st_ino %d", file_s.st_ino);
           fflush(stdout);
           
           write(p_fd, "i am child\n", strlen("i am child\n"));            
       }
    return 0;
}
/**
* 测试进程间通讯
* 弥补pipe 通讯有方向性
*/
#if 0

/* Shared memory control operation.  */
extern int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf) __THROW;

/* Get shared memory segment.  */
extern int shmget (key_t __key, size_t __size, int __shmflg) __THROW;

/* Attach shared memory segment.  */
extern void *shmat (int __shmid, __const void *__shmaddr, int __shmflg)
     __THROW;

/* Detach shared memory segment.  */
extern int shmdt (__const void *__shmaddr) __THROW;
#endif//0
typedef struct proc_msg_s{
    int cmd;
    char data[256];
}proc_msg_t,*proc_msg_ptr;
int main1(int argc, char *argv[]){
    int shm_id;
    pid_t pid;
    proc_msg_ptr pmsg = NULL;

    //创建共享内存
    if ((shm_id = shmget (IPC_PRIVATE, 1024, 600)) == -1){
        perror("");
        return -1;
    }
    if((pmsg = shmat (shm_id, 0, SHM_RND)) == NULL){
        perror("");
        return -1;        
    }
    pmsg->cmd = 100;
    strcpy(pmsg->data, "xuyifei");
    if((pid = fork()) <0){
        perror("");
    }else if(pid > 0){
        wait();
        if(shmctl (shm_id, IPC_RMID, NULL) !=0)
            perror("IPC_RMID faild");
        exit(0);
    }else{
         proc_msg_ptr pmsg1 = NULL;
         char buff[256] = {0};
         if((pmsg1 = shmat (shm_id, 0, SHM_RND)) == NULL){
            perror("");
            return -1;        
        }
        printf("i am child data: %s, cmd: %d\n", pmsg1->data, pmsg1->cmd);
        while(fgets(buff, 256, stdin) != NULL){
            printf("%s size:%d", buff, strlen(buff));
            buff[strlen(buff) - 1] = '\0';
            if ( strcmp(buff, "quit") == 0){
                shmdt(shm_id);
                exit(0);
            }
            
        }
        
    }
    return 0;
    
}
int s_shm_id = -1;

int proc_testmain(int argc, char *argv[]){
    pid_t pid;
    int shm_id =             create_shmt(SHMT_MAX_SIZE, 600);;
    if((pid = fork()) < 0)
           return;
    if(pid > 0){
           //parent
           char buff[1024] = {0};
            s_shm_id = shm_id;
            printf("parnet pid:%d\n", pid);
            init_lock();
            
            while(fgets(buff, 1024, stdin) != NULL){
                buff[strlen(buff) - 1] = '0';
                if(strcmp(buff, "quit") == 0){
                    exit(0);
                    //parent exit
                }else if(strcmp(buff, "cquit")){
                    //send cmd child exit
                    send_msg(pid, msg_req, quit, "quit", strlen("quit"));
                }
            }
        }else{
        sleep(2);
        s_shm_id = shm_id;
        fflush(stdout);        
        shmt_msg_header *h;
        shmt_msg *msg;
        while ((recv_msg(&h, &msg) == 0)&&get_lock(h))
            {
                printf("msg:%s type[req 0 ack1]:%d, cmd:%d", msg->msgbody, h->type, h->opt);
                if( h->opt == quit)
                  {
                    printf("parent send quit cmd\n");                  
                    exit(0);
                }
            }
       }

    return 0;

}

