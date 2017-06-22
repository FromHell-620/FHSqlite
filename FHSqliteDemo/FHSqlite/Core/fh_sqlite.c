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

SqliteRef sqlite_initialize(const char* sqlite_path) {
    if (sqlite_path == NULL || strlen(sqlite_path) <= 0) return NULL;
    
    SqliteRef sqlite = malloc(sizeof(_fh_sqlite));
    if (sqlite == NULL) return NULL;
    sqlite->pool = pool_create(20);
    sqlite->_sqlite_path = strdup(sqlite_path);
    sqlite_lock_initialize(sqlite);
    return sqlite;
}

sqlite_db sqlite_db_initialize(SqliteRef sqlite,pthread_t thread) {
    if (thread == NULL || sqlite == NULL) return NULL;
    sqlite_lock(sqlite);
    sqlite_db db = malloc(sizeof(_sqlite_db));
    if (db == NULL) {
        sqlite_unlock(sqlite);
        return NULL;
    }
    sqlite_unlock(sqlite);
    db->_thread = thread;
    db->_sqlite_path = strdup(sqlite->_sqlite_path);
    CFDictionaryValueCallBacks value = {0,0,&sqlite_db_stmt_cache_release,0,0};
    db->_stmt_cache = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 10, &kCFCopyStringDictionaryKeyCallBacks, &value);
    return db;
}

sqlite_db sqlite_open(SqliteRef sqlite) {
    if (sqlite == NULL) return NULL;

    if (_db ) return _db;
    
    _db = sqlite_db_initialize(sqlite, pthread_self());
    
    sqlite3* db = NULL;
    if (sqlite3_open(sqlite->_sqlite_path, &db) == SQLITE_OK) {
        _db->_db = db;
        CFDictionarySetValue(sqlite->_db_cache, pthread_self(), _db);
    }else {
        sqlite_db_delloc(_db);
    }
    return _db;
}

sqlite_db db_with_thread(SqliteRef sqlite,pthread_t thread) {
    if (sqlite == NULL || thread == NULL) return NULL;
    return (sqlite_db)CFDictionaryGetValue(sqlite->_db_cache, thread);
}

sqlite_db db_get_current(SqliteRef sqlite) {
    return db_with_thread(sqlite, pthread_self());
}

bool db_is_in(sqlite_db db) {
    if (db == NULL) return false;
    return db->_in;
}

void sqlite_delloc(SqliteRef sqlite) {
    if (sqlite == NULL) return;
    free((void*)(sqlite->_sqlite_path));
    CFRelease(sqlite->_db_cache);
    sqlite_lock_destory(sqlite);
    free(sqlite);
    sqlite = NULL;
}

void sqlite_db_delloc(sqlite_db _db) {
    if (_db == NULL) return;
    free((void*)(_db->_sqlite_path));
    CFRelease(_db->_stmt_cache);
    sqlite3_close(_db->_db);
    _db = NULL;
}

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
    pool_node node = malloc(sizeof(_pool_node));
    if (node == NULL) return node;
    node->_used = false;
    node->next = NULL;
    node->prev = NULL;
    node->_db = NULL;
    return node;
}

void pool_node_usedify(db_pool pool) {
    if (pool == NULL) return;
    pool->_thefirst = node->next == NULL?node:node->next;
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
        pool_node_usedify(pool);
        dispatch_semaphore_signal(pool->_lock)'
        return node;
    }
    
    
    
}

