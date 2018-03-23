#include "list.h"
/*

void init_list(head_t *h);

void list_append(head_t *h, node_t *d);

void list_front(head_t *h, node_t *d);


int list_size(head_t *h);

void list_del(head_t *h, node_t *d);
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
    }
}
*/
head_t g_list;
head_t *list_ptr = &g_list;
int main(int argc, char * argv [ ]){
    node_t *th;
    init_list(list_ptr);
    int i = 0;
    node_t * pt = NULL;
    for (i = 0; i < 10; ++i){
        pt = alloc_node();
        pt->data = i;
        list_append(list_ptr, pt);
    }
    
    th = list_ptr->next;
    while(th){
        printf("th->data: %d\n", th->data);
        th = th->next;
    }
    
    destroy_list(list_ptr);
    return 0;
}
