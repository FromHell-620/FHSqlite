//
//  NSObject+SQLGenerator.m
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import "NSObject+SQLGenerator.h"
#import "FHSqliteProtocol.h"
#import "FHClassInfo.h"
#import <objc/runtime.h>

@implementation NSObject (SQLGenerator)

+ (NSString *)sql_initializeTable {
    Class<FHSqliteProtocol> cls = self;
    NSString *tableName = nil;
    if (class_respondsToSelector(self, @selector(tableName))) {
        tableName = [cls tableName];
    }
    if (tableName.length <= 0) {
        tableName = NSStringFromClass(self);
    }
    FHClassInfo *classInfo = [FHClassInfo infoWithClass:self];
    if (classInfo == nil) return nil;
    NSArray<NSString *> *columnNames = nil;
    if (class_respondsToSelector(self, @selector(columnNames))) {
        columnNames = [cls columnNames];
    }
    if (columnNames.count == 0) {
        columnNames = classInfo.propertysExceptReadonly;
    }
    if (columnNames == nil || columnNames.count == 0) return nil;
    
    NSDictionary<NSString *,FHSqliteConstraint *> *constraints = nil;
    if (class_respondsToSelector(self, @selector(constraints))) {
        constraints = [cls constraints];
    }
    NSMutableString *sql = [NSMutableString stringWithFormat:@"CREATE TABLE IF NOT EXISTS %@",tableName];
    [columnNames enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        
    }];
    return [sql copy];
}

@end
