//
//  fh_sqlite.c
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_sqlite.h"
#include <pthread/pthread.h>

CF_INLINE void sqlite_lock_initialize(SqliteRef sqlite)
{
    pthread_mutex_init(&(sqlite->_sqlite_lock), NULL);
}

CF_INLINE void sqlite_lock_destory(SqliteRef sqlite)
{
    pthread_mutex_destroy(&(sqlite->_sqlite_lock));
}

CF_INLINE void sqlite_lock(SqliteRef sqlite)
{
    pthread_mutex_lock(&(sqlite->_sqlite_lock));
}
                       
CF_INLINE void sqlite_unlock(SqliteRef sqlite)
{
    pthread_mutex_unlock(&(sqlite->_sqlite_lock));
}

CF_INLINE void sqlite_db_cache_value_release(CFAllocatorRef allocator, const void* value)
{
    sqlite_db db = (sqlite_db)value;
    sqlite_db_delloc(db);
}

CF_INLINE void sqlite_db_stmt_cache_release(CFAllocatorRef allocator,const void* value)
{
    sqlite3_stmt* stmt = (sqlite3_stmt*)value;
    sqlite3_finalize(stmt);
    stmt = NULL;
}


SqliteRef sqlite_initialize(const char* sqlite_path)
{
    if (sqlite_path == NULL || strlen(sqlite_path) <= 0) return NULL;
    
    SqliteRef sqlite = malloc(sizeof(_fh_sqlite));
    if (sqlite == NULL) return NULL;
    CFDictionaryValueCallBacks value = {0,0,&sqlite_db_cache_value_release,0,0};
    sqlite->_db_cache = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 10, NULL, &value);
    sqlite->_sqlite_path = strdup(sqlite_path);
    sqlite_lock_initialize(sqlite);
    return sqlite;
}

sqlite_db sqlite_db_initialize(SqliteRef sqlite,pthread_t thread)
{
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

sqlite_db db_with_thread(SqliteRef sqlite,pthread_t thread)
{
    if (sqlite == NULL || thread == NULL) return NULL;
    return (sqlite_db)CFDictionaryGetValue(sqlite->_db_cache, thread);
}

sqlite_db db_get_current(SqliteRef sqlite)
{
    return db_with_thread(sqlite, pthread_self());
}

void sqlite_delloc(SqliteRef sqlite)
{
    if (sqlite == NULL) return;
    free((void*)(sqlite->_sqlite_path));
    CFRelease(sqlite->_db_cache);
    sqlite_lock_destory(sqlite);
    free(sqlite);
    sqlite = NULL;
}

void sqlite_db_delloc(sqlite_db _db)
{
    if (_db == NULL) return;
    free((void*)(_db->_sqlite_path));
    CFRelease(_db->_stmt_cache);
    sqlite3_close(_db->_db);
    _db = NULL;
}
