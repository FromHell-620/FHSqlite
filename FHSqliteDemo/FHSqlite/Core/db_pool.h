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
#include <mach/mach.h>
#include "fh_linked.h"

typedef struct _pool_node {
    bool _used;
    bool _is_temp;
    void *_db;//a sqlite_db object
} pool_node;

typedef struct _db_pool {
    uint32_t _pool_count;
    uint32_t _max_count;
    uint32_t _inused_count;
    uint32_t _unused_count;
    linkList *_pool;
} db_pool;


db_pool *pool_create(uint32_t max_count,void(*db_release)(void *db));

pool_node *pool_node_query(db_pool *pool);

void pool_node_unusedify(db_pool *pool,pool_node *node);

void pool_release(db_pool *pool);

#endif /* db_pool_h */
