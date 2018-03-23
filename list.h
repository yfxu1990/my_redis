#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>
#define UNVALIDE_DATA (void*)(0)

typedef struct node_s{
    void* data;
    struct node_s *next;
}node_t, head_t;

/**/
void init_list(head_t *h);

void list_append(head_t *h, node_t *d);
/*头插*/
void list_front(head_t *h, node_t *d);

/*列表的大小*/
int list_size(head_t *h);

void list_del(head_t *h, node_t *d);
void destroy_list(head_t *h);

void* alloc_node();
void free_node();

#endif//_LIST_H
