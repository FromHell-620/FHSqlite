//
//  db_pool.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/24.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "db_pool.h"

static inline void pool_node_finish_used_callback(void *context,pool_node node) {
    
}

pool_node pool_node_create_option(bool is_temp) {
    pool_node node = malloc(sizeof(struct _pool_node));
    if (node == NULL) return node;
    node->_used = false;
    node->next = NULL;
    node->prev = NULL;
    node->_db = NULL;
    node->_is_temp = is_temp;
    return node;
}

pool_node pool_node_create(void) {
    return pool_node_create_option(false);
}

void pool_node_usedify(db_pool pool,pool_node node) {
    if (pool == NULL || node == NULL) return;
    pool->_thefirst = (node->next == NULL)?node:node->next;
    node->next = NULL;
    pool->_thelast->next = node;
    node->prev = pool->_thelast;
    pool->_thelast = node;
    node->_used = true;
}

db_pool pool_create(uint32_t max_count) {
    db_pool pool = malloc(sizeof(struct _pool_node));
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

pool_node pool_node_query(db_pool pool) {
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


