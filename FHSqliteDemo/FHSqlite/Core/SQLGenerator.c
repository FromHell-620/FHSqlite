//
//  SQLGenerator.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/7.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "SQLGenerator.h"
#include <string.h>
#include <stdlib.h>

SQLGenerator *generatorInitialize(const char *cls_name) {
    if (cls_name == NULL || strlen(cls_name) == 0) return NULL;
    SQLGenerator *g = malloc(sizeof(SQLGenerator));
    if (g == NULL) return NULL;
    g->class_name = strdup(cls_name);
    return g;
}
