#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/**/
void init_list(head_t *h){
    h->data = UNVALIDE_DATA;
    h->next = NULL;
}

void list_append(head_t *h, node_t *d){
    head_t *tmp = h;
    while (tmp->next){
        tmp = tmp->next;
    }
    tmp->next = d;
    d->next = NULL;  
}
/*头插*/
void list_front(head_t *h, node_t *d){
    d->next = h->next;
    h->next = d;
}
/*列表的大小*/
int list_size(head_t *h){
    head_t *tmp = h;
    int i = 0;
    while(tmp->next){
        ++i;
        tmp = tmp->next;
    }
    return i;
}
void destroy_list(head_t *h){
    head_t *tmp = h->next;
    while(tmp){
        free_node(tmp);
        tmp = tmp->next;
    }
    return;
}

void* alloc_node(){
    node_t *n = NULL;
    n = calloc(1, sizeof(node_t));
    if(n == NULL){
        perror("");
        printf("[error] calloc faild \n");
    }
    return n;
}

void free_node(node_t * n){
    if(n){
        free(n);
        n = NULL;
    }
}



