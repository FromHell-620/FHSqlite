//
//  fh_linked.c
//  linked list implementation.
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_linked.h"
#include <stdlib.h>

linkNode *linkNodeify(void *value) {
    linkNode *node;
    if ((node = malloc(sizeof(*node))) == NULL) return NULL;
    node->value = value;
    node->next = node->prev = NULL;
    return node;
}

linkIter *linkIterify(linkList *list,iterDirection dicection) {
    if (list == NULL) return NULL;
    linkIter *iter;
    if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;
    iter->direction = dicection;
    iter->next = dicection == iterDirectionHead?list->head:list->tail;
    return iter;
}

linkNode *linkIterNext(linkIter *iter) {
    linkNode *current = iter->next;
    if (current == NULL) return NULL;
    if (iter->direction == iterDirectionHead)
        iter->next = current->next;
    else
        iter->next = current->prev;
    
    return current;
}

linkList *linkListify(linkListNodeCallback callback) {
    linkList *list;
    if ((list = malloc(sizeof(*list))) == NULL) return NULL;
    list->head = list->tail = NULL;
    list->node_release = callback.node_release;
    list->node_match = callback.node_match;
    return list;
}
