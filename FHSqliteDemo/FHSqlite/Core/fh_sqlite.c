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

FH_INLINE void __FHDBCallbackRelease(void *value) {
    
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
