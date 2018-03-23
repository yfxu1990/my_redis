#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>
#define UNVALIDE_DATA (void*)(0)

typedef struct node_s{
    void* data;
    struct node_s *next;
}node_t, head_t;

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

void list_front(head_t *h, node_t *d){
    d->next = h->next;
    h->next = d;
}

int list_size(head_t *h){
    head_t *tmp = h;
    int i = 0;
    while(tmp->next){
        ++i;
        tmp = tmp->next;
    }
    return i;
}

void list_del(head_t *h, node_t *d){
    
}
#endif//_LIST_H
