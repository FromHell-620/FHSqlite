//
//  SQLGenerator.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/7.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef SQLGenerator_h
#define SQLGenerator_h

#include <stdio.h>

typedef struct {
    const char *class_name;
}SQLGenerator;

SQLGenerator *generatorInitialize(const char *cls_name);

#endif /* SQLGenerator_h */
