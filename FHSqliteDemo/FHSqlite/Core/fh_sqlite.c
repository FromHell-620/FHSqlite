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
#include "db_pool.h"
#include "fh_define.h"

typedef struct fh_db {
    sqlite3 *db;
    FHHashRef stmt_cache;
} fh_db;

typedef struct fh_sqlite {
    const char *path;
    struct sqlite_configuration congiguration;
    FHDBPoolRef db_pool;
} fh_sqlite;

FHSqliteRef FHSqliteCreate(const char *path) {
    struct sqlite_configuration configuration = {1,1,6};
    return FHSqliteCreateWithOptions(path, configuration);
}

FHSqliteRef FHSqliteCreateWithOptions(const char *path,struct sqlite_configuration congifuration) {
    FHSqliteRef sqlite;
}
