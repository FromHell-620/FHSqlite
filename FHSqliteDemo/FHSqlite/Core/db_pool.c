//
//  db_pool.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/24.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "db_pool.h"

db_pool pool_create(uint32_t max_count) {
    db_pool pool = malloc(sizeof(_db_pool));
    if (pool == NULL) return NULL;
    pool->_max_count = max_count;
    pool->_pool_count = 0;
    pool->_temp_count = 0;
    pool->_thefirst = NULL;
    pool->_thelast = NULL;
    pool->_lock = dispatch_semaphore_create(1);
    pool->_temp = NULL;
    return pool;
}

pool_node pool_node_create(void) {
    pool_node node = malloc(sizeof(struct _pool_node));
    if (node == NULL) return node;
    node->_used = false;
    node->next = NULL;
    node->prev = NULL;
    node->_db = NULL;
    return node;
}

void pool_node_usedify(db_pool pool,pool_node node) {
    if (pool == NULL) return;
    pool->_thefirst = (node->next == NULL)?node:node->next;
    node->next = NULL;
    pool->_thelast->next = node;
    node->prev = pool->_thelast;
    pool->_thelast = node;
    node->_used = true;
}

pool_node pool_query(db_pool pool) {
    if (pool == NULL) return NULL;
    dispatch_semaphore_wait(pool->_lock, DISPATCH_TIME_FOREVER);
    pool_node node = pool->_thefirst;
    if (node == NULL) { //the first
        node = pool_node_create();
        node->_used = true;
        pool->_thefirst = node;
        pool->_thelast = node;
        dispatch_semaphore_signal(pool->_lock);
        return node;
    }
    if (node->_used == false) {
        pool_node_usedify(pool,node);
        dispatch_semaphore_signal(pool->_lock);
        return node;
    }
}
