//
//  FHSqliteProtocol.m
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/5.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import "FHSqliteProtocol.h"

@interface FHSqliteConstraintDefaultInt : FHSqliteConstraintDefault

@property (nonatomic,assign,readonly) int value;

+ (instancetype)defaultWithValue:(int)value;

@end

@implementation FHSqliteConstraintDefaultInt

+ (instancetype)defaultWithValue:(int)value {
    FHSqliteConstraintDefaultInt *d = [FHSqliteConstraintDefaultInt new];
    d->_value = value;
    return d;
}

@end

@interface FHSqliteConstraintDefaultFloat : FHSqliteConstraintDefault

@property (nonatomic,assign,readonly) float value;

+ (instancetype)defaultWithValue:(float)value;

@end

@implementation FHSqliteConstraintDefaultFloat

+ (instancetype)defaultWithValue:(float)value {
    FHSqliteConstraintDefaultFloat *f = [FHSqliteConstraintDefaultFloat new];
    f->_value = value;
    return f;
}

@end

@interface FHSqliteConstraintDefaultCString : FHSqliteConstraintDefault

@property (nonatomic,readonly) const char *value;

+ (instancetype)defaultWithValue:(const char *)value;

@end

@implementation FHSqliteConstraintDefaultCString

+ (instancetype)defaultWithValue:(const char *)value {
    NSCParameterAssert(value);
    FHSqliteConstraintDefaultCString *c = [FHSqliteConstraintDefaultCString new];
    c->_value = strdup(value);
    return c;
}

- (void)dealloc {
    free((void *)_value);
}

@end

@interface FHSqliteConstraintDefaultObject : FHSqliteConstraintDefault

@property (nonatomic,strong,readonly) NSObject *value;

+ (instancetype)defaultWithValue:(NSObject *)value;

@end

@implementation FHSqliteConstraintDefaultObject

+ (instancetype)defaultWithValue:(NSObject *)value {
    FHSqliteConstraintDefaultObject *o = [FHSqliteConstraintDefaultObject new];
    o->_value = value;
    return o;
}

@end

@implementation FHSqliteConstraintDefault

+ (instancetype)defaultWithInt:(int)i {
    return [FHSqliteConstraintDefaultInt defaultWithValue:i];
}

+ (instancetype)defaultWithFloat:(float)f {
    return [FHSqliteConstraintDefaultFloat defaultWithValue:f];
}

+ (instancetype)defaultWithCString:(const char *)cStr {
    return [FHSqliteConstraintDefaultCString defaultWithValue:cStr];
}

+ (instancetype)defaultWithObject:(NSObject *)obj {
    return [FHSqliteConstraintDefaultObject defaultWithValue:obj];
}

- (int)intValue {
    return ((FHSqliteConstraintDefaultInt *)self).value;
}

- (float)floatValue {
    return ((FHSqliteConstraintDefaultFloat *)self).value;
}

- (const char *)cStringValue {
    return ((FHSqliteConstraintDefaultCString *)self).value;
}

- (NSObject *)objectValue {
    return ((FHSqliteConstraintDefaultObject *)self).value;
}

@end

@implementation FHSqliteConstraint

+ (instancetype)constraintWithColumnName:(NSString *)columnName
                                   state:(FHSqliteConstraintState)state {
    FHSqliteConstraint *constraint = [[FHSqliteConstraint alloc] init];
    constraint.columnName = columnName;
    constraint.constraintState = state;
    return constraint;
}

@end
