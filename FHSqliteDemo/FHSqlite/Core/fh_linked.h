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
#include "fh_define.h"

typedef enum iterDirection {
    iterDirectionHead,
    iterDirectionTail
} iterDirection;

typedef struct linkListNodeCallback {
    void * (*node_retain)(void *ptr);
    void (*node_release)(void *ptr);
    int (*node_match)(void *pth,void *key);
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
    linkListNodeCallback callback;
    unsigned long len;
} linkList;

#pragma mark- linkNode

FH_EXTERN linkNode *linkNodeify(void *value);

FH_EXTERN void linkNodeRelease(linkNode *node);

#pragma mark- linkIter

FH_EXTERN linkIter *linkIterify(linkList *list,iterDirection dicection);

FH_EXTERN linkNode *linkIterNext(linkIter *iter);

#pragma mark- linkList

FH_EXTERN linkList *linkListify(linkListNodeCallback* callback);

FH_EXTERN linkList *linkListAddHeadWithNode(linkList *list,linkNode *node);

FH_EXTERN linkList *linkListAddHead(linkList *list,void *value);

FH_EXTERN linkList *linkListAddTail(linkList *list,void *value);

FH_EXTERN linkNode *linkListQueryNode(linkList *list,unsigned long index);

FH_EXTERN void *linkListQueryValue(linkList *list,unsigned long index);

FH_EXTERN linkList *linkListInsert(linkList *list,void *value,unsigned long index);

FH_EXTERN linkList *linkListDelWithIndex(linkList *list,unsigned long index);

FH_EXTERN linkList *linkListDelWithValue(linkList *list,void *value);

FH_EXTERN linkList *linkListDelTTail(linkList *list);

FH_EXTERN linkList *linkListEmpty(linkList *list);

FH_EXTERN linkList *linkListHeadToTail(linkList *list);

FH_EXTERN linkList *linkListTailToHead(linkList *list);

FH_EXTERN linkList *linkListMoveToTail(linkList *list,linkNode *node);

FH_EXTERN void linkListRelease(linkList *list);

#endif /* fh_linked_h */
