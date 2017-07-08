//
//  fh_linked.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef fh_linked_h
#define fh_linked_h

#include <stdio.h>
#include <stdbool.h>

typedef enum iterDirection {
    iterDirectionHead,
    iterDirectionTail
} iterDirection;

typedef struct linkListNodeCallback {
    void(*node_release)(void *ptr);
    int(*node_match)(void *pth,void *key);
} linkListNodeCallback;

typedef struct linkNode {
    struct linkNode *next;
    struct linkNode *prev;
    void *value;
} linkNode;

typedef struct linkIter {
    linkNode *next;
    iterDirection direction;
} linkIter;

typedef struct linkList {
    linkNode *head;
    linkNode *tail;
    void(*node_release)(void *ptr);
    int(*node_match)(void *pth,void *key);
    unsigned long len;
} linkList;

linkNode *linkNodeify(void *value);

void linkNodeRelease(linkNode *node);

linkIter *linkIterify(linkList *list,iterDirection dicection);

linkNode *linkIterNext(linkIter *iter);

linkList *linkListify(linkListNodeCallback* callback);

linkList *linkListEmpty(linkList *list);

void linkListRelease(linkList *list);

#endif /* fh_linked_h */
