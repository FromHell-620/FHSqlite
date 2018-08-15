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
#include <mach/mach.h>
#include <pthread/pthread.h>
#include "fh_linked.h"
#include "fh_define.h"

typedef struct _pool_node {
    uint _is_temp : 1 ;
    void *_db;//a sqlite_db object
} pool_node;

typedef struct _db_pool {
    uint32_t _pool_count;
    uint32_t _max_count;
    uint32_t _inused_count;
    pthread_mutex_t _lock;
    linkList *_pool;
} db_pool;

typedef struct _db_pool * FHDBPoolRef;

FH_EXTERN db_pool *pool_create(uint32_t max_count,void(*db_release)(void *pool_node));

FH_EXTERN linkNode *pool_node_query(db_pool *pool);

FH_EXTERN void pool_node_unuse(db_pool *pool,linkNode *node);

FH_EXTERN void pool_node_release_ifneed(db_pool *pool,linkNode *node);

FH_EXTERN void pool_release(db_pool *pool);

#endif /* db_pool_h */
