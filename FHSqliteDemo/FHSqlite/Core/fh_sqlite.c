//
//  fh_sqlite.c
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_sqlite.h"
#include <sqlite3.h>
#include "fh_hash.h"
#include <pthread/pthread.h>
#include "fh_pool.h"
#include "fh_define.h"

typedef struct fh_db {
    sqlite3 *db;
    FHHashRef stmt_cache;
} fh_db;

typedef struct fh_sqlite {
    const char *path;
    struct sqlite_configuration *congiguration;
    FHPoolRef db_pool;
} fh_sqlite;

FH_INLINE void __SqliteOpen(FHDBRef db,const char *path) {
    if (db == NULL || db->db != NULL || strlen(path) == 0) return;
    int result = sqlite3_open(path, &db->db);
    if (result != SQLITE_OK) {
        printf("sqlite open failed  %d  %s",result,sqlite3_errmsg(db->db));
        db->db = NULL;
        return;
    }
}

FH_INLINE void __SqliteClose(FHDBRef db) {
    if (db == NULL || db->db == NULL) return;
    unsigned int count = 3;
    FHBoolean triedFinalizingOpenStatements = false;
    FHBoolean retry = true;
    do {
        count --;
        retry = false;
        int result = sqlite3_close(db->db);
        if (result == SQLITE_BUSY || result == SQLITE_LOCKED) {
            if (!triedFinalizingOpenStatements) {
                triedFinalizingOpenStatements = true;
                sqlite3_stmt *stmt;
                while ((stmt = sqlite3_next_stmt(db->db, nil)) != 0) {
                    sqlite3_finalize(stmt);
                    retry = true;
                }
            }
            
        }else if (result != SQLITE_OK) {
            count = 0;
            printf("sqlite close failed  %d   %s",result,sqlite3_errmsg(db->db));
        }
    } while (retry && count > 0);
}

FH_INLINE void __FHDBCallbackRelease(void *value) {
    FHDBRef db = value;
    if (db == NULL) return;
    FHHashRelease(db->stmt_cache);
    __SqliteClose(db);
    free(db);
    db = NULL;
}

FHDBRef FHDBCreate(void) {
    FHDBRef db;
    if ((db = calloc(1, sizeof(fh_db))) == NULL) return NULL;
    FHHashValueCallback callback = {NULL,(FHHashReleaseCallback)__FHDBCallbackRelease,NULL};
    db->stmt_cache = FHHashCreateWithOptions(10, &kFHCopyStringKeyCallback, &callback);
    if (db->stmt_cache == NULL) {
        free(db);
        db = NULL;
    }
    return db;
}

FHSqliteRef FHSqliteCreate(const char *path) {
    struct sqlite_configuration configuration = {1,6,NULL};
    return FHSqliteCreateWithOptions(path, &configuration);
}

FHSqliteRef FHSqliteCreateWithOptions(const char *path,SqliteConfigurationref congifuration) {
    if (path == NULL || strlen(path) == 0) return NULL;
    FHSqliteRef sqlite;
    if ((sqlite = calloc(1, sizeof(fh_sqlite))) == NULL) return NULL;
    sqlite->path = strdup(path);
    if (congifuration == NULL) {
        struct sqlite_configuration config = {1,6,NULL};
        congifuration = &config;
    };
    sqlite->congiguration = congifuration;
    FHPoolValueCallback callback = {NULL,__FHDBCallbackRelease};
    sqlite->db_pool = FHPoolCreateWithCallback(congifuration->pool_size, &callback, congifuration->callback);
    return sqlite;
}

void FHSqliteRelease(FHSqliteRef sqlite) {
    if (sqlite == NULL) return;
    sqlite->congiguration = NULL;
    FHPoolRelease(sqlite->db_pool);
    free((void *)sqlite->path);
    free(sqlite);
    sqlite = NULL;
}
