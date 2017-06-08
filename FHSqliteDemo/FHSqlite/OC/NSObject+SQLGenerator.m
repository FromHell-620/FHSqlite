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

FOUNDATION_STATIC_INLINE NSString * SqliteTypeFromPropertyType(FHPropertyInfo *info) {
    switch (info.typeEncoding) {
        case FHPropertyEncodingTypeInt:
            return @"INT";
        case FHPropertyEncodingTypeBool:
            return @"BOOL";
        case FHPropertyEncodingTypeLong:
            return @"LONG";
        case FHPropertyEncodingTypeFloat:
            return @"FLOAT";
        case FHPropertyEncodingTypeDouble:
            return @"DOUBLE";
        case FHPropertyEncodingTypeCString:
            return @"TEXT";
        case FHPropertyEncodingTypeObject: {
            switch (info.objectTypeEncoding) {
                case FHPropertyObjectEncodingTypeNSString:
                case FHPropertyObjectEncodingTypeNSURL:
                case FHPropertyObjectEncodingTypeNSDate:
                case FHPropertyObjectEncodingTypeNSNumber:
                    return @"TEXT";
                case FHPropertyObjectEncodingTypeNSData:
                case FHPropertyObjectEncodingTypeNSArray:
                case FHPropertyObjectEncodingTypeUIImage:
                case FHPropertyObjectEncodingTypeNSDictionary:
                    return @"BLOB";
                    
                default:
                    return nil;
            }
        }
            
        default:
            return nil;
    }
}

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
    
    NSString *(^columnItemSql)(NSString *,NSString *,FHSqliteConstraint *) = ^NSString *(NSString *columnName,NSString *columnType,FHSqliteConstraint *constraint) {
        if (columnType == nil) return nil;
        NSMutableString *item_sql = [NSMutableString stringWithFormat:@"%@ %@",columnName,columnType];
        if (FHSqliteConstraintStatePrimaryKey & constraint.constraintState) {
            [item_sql appendString:@" PRIMARY KEY"];
        }
        if (FHSqliteConstraintStateNonNull & constraint.constraintState) {
            [item_sql appendString:@" NOT NULL"];
        }
        if (FHSqliteConstraintStateUnique & constraint.constraintState) {
            [item_sql appendString:@" UNIQUE"];
        }
        return [item_sql copy];
    };
    [columnNames enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        FHPropertyInfo *info = [classInfo.propertysInfo objectForKey:obj];
        NSString *column_type = SqliteTypeFromPropertyType(info);
        FHSqliteConstraint *constraint = [constraints objectForKey:obj];
        NSString *sub_sql = columnItemSql(obj,column_type,constraint);
        if (sub_sql == nil) return ;
        [sql appendFormat:@"(%@,);",sub_sql];
    }];
    [sql deleteCharactersInRange:NSMakeRange(sql.length - 3, 1)];
    return [sql copy];
}

@end
