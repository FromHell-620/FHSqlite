//
//  fh_sqlite.c
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_sqlite.h"
#include <pthread/pthread.h>

CF_INLINE void sqlite_lock_initialize(SqliteRef sqlite) {
    pthread_mutex_init(&(sqlite->_sqlite_lock), NULL);
}

CF_INLINE void sqlite_lock_destory(SqliteRef sqlite) {
    pthread_mutex_destroy(&(sqlite->_sqlite_lock));
}

CF_INLINE void sqlite_lock(SqliteRef sqlite) {
    pthread_mutex_lock(&(sqlite->_sqlite_lock));
}
                       
CF_INLINE void sqlite_unlock(SqliteRef sqlite) {
    pthread_mutex_unlock(&(sqlite->_sqlite_lock));
}

CF_INLINE void sqlite_db_cache_value_release(CFAllocatorRef allocator, const void* value) {
    sqlite_db db = (sqlite_db)value;
    sqlite_db_delloc(db);
}

CF_INLINE void sqlite_db_stmt_cache_release(CFAllocatorRef allocator,const void* value) {
    sqlite3_stmt* stmt = (sqlite3_stmt*)value;
    sqlite3_finalize(stmt);
    stmt = NULL;
}

void pool_insert(fh_pool *pool,fh_db *db) {
    if (pool == NULL || db == NULL) return;
    linkList *result = NULL;
    if (pool->pool == NULL) {
        linkList *list = NULL;
        if ((list = linkListify(pool->callback)) == NULL) return ;
        pool->pool = result = list;
        pool->used_count += 1;
    }
    if (pool->used_count >= pool->max_count) {
        if (pool->temp_pool == NULL) {
            linkList *list = NULL;
            if ((list = linkListify(pool->callback)) == NULL) return;
            pool->temp_pool = list;
        }
        result = pool->temp_pool;
    }
    linkListAddHead(result, db);
}

fh_pool *pool_create(unsigned long max_count,linkListNodeCallback *callback) {
    fh_pool *pool = NULL;
    if ((pool = malloc(sizeof(*pool))) == NULL) return NULL;
    pool->max_count = max_count;
    pool->used_count = 0;
    pool->pool = pool->temp_pool = NULL;
    return pool;
}




