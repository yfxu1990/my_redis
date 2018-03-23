#include "redis.h"
#include "stdlib.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
/**
模拟shell控制台
*/
char * linenoise(char *promat){
    	char buff[2048] = {0};
        int len = 0;
    	printf ( "%s",promat );
        fflush(stdout);
        if ( fgets ( buff, 2048, stdin ) == NULL )
            return NULL;
        len = strlen(buff);
        while(len &&(buff[len-1] == '\n' || buff[len-1] == '\r')){
            --len;
            buff[len] = '\0';
        }
        return strdup(buff);
}

int redis_main ( int argc, char* argv [] )
{
	char* promat = "127.0.0.1:6379>";
    char * line;
    int cmd_fd, cmd_1,bk_out;
    if((bk_out = dup(STDOUT_FILENO)) <0 ){
        perror("");
        return -1;
    }
        
	//daemon(0, 0);
	cmd_fd = open("cmd.log", O_CREAT|O_WRONLY|O_APPEND);
    if(cmd_fd < 0){
        perror("");
        return -1;
    }
    cmd_1 = dup2(cmd_fd, STDOUT_FILENO);
    while((line = linenoise(promat)) != NULL){
        printf("%s\n", line);
        if(strcmp("quit", line) == 0){
            free(line);
            break;
        }
        //system(line);
        free(line);
    }
    dup2(bk_out,cmd_1);
    printf("recovery stdout success \n");
	close(cmd_fd);
	return 0;
}

