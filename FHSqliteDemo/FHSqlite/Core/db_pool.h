//
//  db_pool.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/24.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef db_pool_h
#define db_pool_h

#include <stdio.h>
#include <stdbool.h>
#include <CoreFoundation/CoreFoundation.h>

typedef struct _pool_node *pool_node;

struct _pool_node {
    bool _used;
    void *_db;//a sqlite_db object
    pool_node next;
    pool_node prev;
    void(^node_did_finish_used_callback)(pool_node node);
};

struct _pool_temp_node {
    void *_db;
    pool_node next;
    void(^node_did_finish_used_callback)(pool_node node);
};

typedef struct _db_pool *db_pool;

struct _db_pool {
    uint32_t _pool_count;
    uint32_t _max_count;
    uint32_t _temp_count;
    dispatch_semaphore_t _lock;
    pool_node _thefirst;
    pool_node _thelast;
    struct _pool_temp_node _temp;
};


db_pool pool_create(uint32_t max_count);

pool_node pool_query(db_pool pool);

#endif /* db_pool_h */
