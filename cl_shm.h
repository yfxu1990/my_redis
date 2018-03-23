#ifndef _CL_SHM_H
#define _CL_SHM_H
#include <pthread.h>
/**
* 共享内存操作
*/
#define MSG_HEADER_SIZE (sizeof(shmt_msg_header))
/*a packet max len*/
#define MSG_MAX_LEN   3071
#define SHMT_MAX_SIZE 4096
#define SHMT_KEY  IPC_PRIVATE
#define SHMT_MODEL 0600
typedef enum msg_type{
    msg_req = 0,
    msg_ack,
    msg_notify     
}msg_type;
    
typedef enum msg_opt{
    unvalid = 0,
    echo,
    hit,
    quit
}msg_opt;

typedef struct shmt_msg_header{
        msg_type type;
        msg_opt opt;
        int len;
        pid_t send;
        pid_t recv;  
        pthread_mutex_t mutex;
}shmt_msg_header;

typedef struct shmt_msg{
    char msgbody[MSG_MAX_LEN];

}shmt_msg;


/**
返回shimt,id
*/
extern int s_shm_id;
int create_shmt(int size, int model){
   
    if ((s_shm_id = shmget (SHMT_KEY, size, model)) == -1){
        perror("");
        printf("create_shmt faild \n");
        return -1;
   }
    printf("[create_shmt]s_shm_id: %d \n", s_shm_id);
    return s_shm_id;
}

void _push_header(shmt_msg_header* h, pid_t send, pid_t recv, msg_type type, msg_opt ipt){
    h->recv = recv;
    h->send = send;
    h->type = type;
    h->opt = ipt;
}
void _set_header_len(shmt_msg_header* h, int len){
    h->len = len;
}

void init_lock(){
    char *pmsg;
    if((pmsg = shmat (s_shm_id, 0, SHM_RND)) == NULL){
        perror("");
        return -2;        
    }
    shmt_msg_header*p = (shmt_msg_header*)pmsg;
    pthread_mutex_init(&p->mutex, NULL);
    shmdt(s_shm_id);
}

int get_lock(shmt_msg_header* h){
    pthread_mutex_lock(&h->mutex);
    return 1;
}

void un_lock(shmt_msg_header *h){
    pthread_mutex_unlock(&h->mutex);
}
int send_msg(pid_t recv, msg_type type, msg_opt opt, char* msg, int len){
    char *pmsg= NULL;
    int total = 0;
    shmt_msg_header header;
    shmt_msg snd_msg;
    if((MSG_MAX_LEN < len)||!msg)
        return -1;
    //attach process
    if((pmsg = shmat (s_shm_id, 0, SHM_RND)) == NULL){
        perror("");
        return -2;        
    }
    // push data
    _push_header(&header, getpid(), recv, type, opt);
    strncpy(snd_msg.msgbody, msg, MSG_MAX_LEN);
    total = len + MSG_HEADER_SIZE;
    _set_header_len(&header, total);
    memcpy(pmsg, &header, MSG_HEADER_SIZE);
    get_lock((shmt_msg_header *)pmsg);
    memcpy(pmsg + MSG_HEADER_SIZE, &snd_msg, MSG_MAX_LEN);
    shmdt(s_shm_id);
    return total;
}
/**
out buff readonly 
*/
int recv_msg(shmt_msg_header **h, shmt_msg **msg)
{       
    printf("111111111[recv_msg]s_shm_id:%d \n", s_shm_id);
    char *pmsg = NULL;
    if((pmsg = shmat (s_shm_id, 0, 0)) == -1){
        perror("");
        printf("error \n");
        system("touch err.log");
        return -1;        
    }
     *h = (shmt_msg_header*)pmsg;
     *msg = (shmt_msg*)(pmsg + MSG_HEADER_SIZE);
     printf("22222222recv_msag \n");
     printf("len: %d, opt: %d, type:%d msg: %s\n", (*h)->len, (*h)->opt, (*h)->type, (*msg)->msgbody);
     shmdt(s_shm_id);
     return 0;
}
void destry_shmt(int shmt_id)
{
    shmctl(s_shm_id, IPC_RMID, NULL);
 }

#endif//_CL_SHM_H