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

pool_node *node_create(bool isTemp) {
    pool_node *node = calloc(1, sizeof(pool_node));
    if (node == NULL) return NULL;
    node->_used = 
}

//pool
db_pool *pool_create(uint32_t max_count,void(*db_release)(void *db)) {
    db_pool *pool = calloc(1, sizeof(db_pool));
    if (pool == NULL) return NULL;
    linkListNodeCallback callback = {db_release,NULL};
    linkList *list = linkListify(&callback);
    pool->_pool = list;
    pool->_max_count = max_count;
    pool->_pool_count = 0;
    pool->_inused_count = pool->_unused_count = 0;
    return pool;
}

pool_node *pool_node_query(db_pool pool) {
    if (pool == NULL) return NULL;
    dispatch_semaphore_wait(pool->_lock, DISPATCH_TIME_FOREVER);
    pool_node node = pool->_thefirst;
    if (node == NULL) { //the first
        node = pool_node_create();
        node->_used = true;
        pool->_thefirst = node;
        pool->_thelast = node;
        pool->_pool_count += 1;
        dispatch_semaphore_signal(pool->_lock);
        return node;
    }
    if (node->_used == false) {
        pool_node_usedify(pool,node);
        dispatch_semaphore_signal(pool->_lock);
        return node;
    }
    
    if (pool->_pool_count < pool->_max_count) {
        pool_node new_node = pool_node_create();
        pool_node_usedify(pool, new_node);
        pool->_pool_count += 1;
        dispatch_semaphore_signal(pool->_lock);
        return new_node;
    }else {
        pool_node temp_node = pool_node_create_option(true);
        temp_node->next = pool->_temp;
        pool->_temp = temp_node;
        pool->_temp_count += 1;
        dispatch_semaphore_signal(pool->_lock);
        return temp_node;
    }
}

void pool_node_unusedify(db_pool pool,pool_node node) {
    if (pool == NULL || node == NULL) return;
    dispatch_semaphore_wait(pool->_lock, DISPATCH_TIME_FOREVER);
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = NULL;
    node->next = pool->_thefirst;
    pool->_thefirst->prev = node;
    pool->_thefirst = node;
    node->_used = false;
    dispatch_semaphore_signal(pool->_lock);
}

void pool_node_release(db_pool pool,pool_node node) {
    
}


