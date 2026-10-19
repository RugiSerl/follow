#pragma once

typedef struct _list_node s_node;
struct _list_node{
    void * data;
    struct _list_node * next;
};

s_node * list_create();
void *list_get_data(s_node * node);
void list_set_data(s_node * node, void * data);
s_node * list_insert(s_node * head, void * data);
