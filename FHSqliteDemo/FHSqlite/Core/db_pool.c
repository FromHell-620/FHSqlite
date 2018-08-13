//
//  db_pool.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/24.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "db_pool.h"
#include <stdlib.h>
//pool_node

FH_INLINE pool_node *node_create(bool isTemp) {
    pool_node *node = calloc(1, sizeof(pool_node));
    if (node == NULL) return NULL;
    node->_is_temp = isTemp ? 1 : 0;
    node->_db = NULL;
    return node;
}

//pool
db_pool *pool_create(uint32_t max_count,void(*db_release)(void *pool_node)) {
    if (max_count <6) max_count = 6;
    db_pool *pool = calloc(1, sizeof(db_pool));
    if (pool == NULL) return NULL;
    linkListNodeCallback callback = {db_release,NULL};
    linkList *list = linkListify(&callback);
    pool->_pool = list;
    pool->_max_count = max_count;
    pool->_pool_count = 0;
    pool->_inused_count = 0;
    pthread_mutex_init(&pool->_lock, NULL);
    return pool;
}

linkNode *pool_node_query(db_pool *pool) {
    if (pool == NULL) return NULL;
    pthread_mutex_lock(&pool->_lock);
    linkNode *linkNode = NULL;
    if (pool->_pool_count == pool->_max_count) {
        if (pool->_inused_count == pool->_pool_count) {
            linkNode = linkNodeify(node_create(true));
        }else {
            linkNode = pool->_pool->tail;
            pool->_inused_count ++;
        }
    }else {
        if (pool->_inused_count < pool->_pool_count) {
            linkNode = pool->_pool->tail;
            pool->_inused_count ++;
        }else {
            linkNode = linkNodeify(node_create(false));
            pool->_inused_count ++;
            pool->_pool_count ++;
        }
    }
    if (linkNode->next == NULL && linkNode->prev == NULL) linkListAddHeadWithNode(pool->_pool, linkNode);
    else
        linkListTailToHead(pool->_pool);
    pthread_mutex_unlock(&pool->_lock);
    return linkNode;
}

void pool_node_unuse(db_pool *pool,linkNode *node) {
    if (pool == NULL || node == NULL) return;
    pthread_mutex_lock(&pool->_lock);
    linkListMoveToTail(pool->_pool, node);
    pool->_inused_count -- ;
    pthread_mutex_unlock(&pool->_lock);
}

void pool_node_release_ifneed(db_pool *pool,linkNode *node) {
    if (pool == NULL || node == NULL) return;
    pool_node *pool_node = node->value;
    if (pool_node->_is_temp == 0) return;
    if (pool->_pool->node_release) pool->_pool->node_release(pool_node);
    free(pool_node);
    pool_node = NULL;
    linkNodeRelease(node);
}

void pool_release(db_pool *pool) {
    if (pool == NULL) return;
    pthread_mutex_destroy(&pool->_lock);
    linkListRelease(pool->_pool);
    free(pool);
    pool = NULL;
}
