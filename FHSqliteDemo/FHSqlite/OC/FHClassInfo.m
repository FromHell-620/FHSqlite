//
//  FHClassInfo.m
//  FHClassInfoDemo
//
//  Created by 李浩 on 2017/6/3.
//  Copyright © 2017年 GodL. All rights reserved.
//

#import "FHClassInfo.h"
#import <objc/runtime.h>

FHPropertyEncodingType FHPropertyEncodingTypeWithType(const char *type) {
    if (type == NULL || strlen(type) <= 0) return FHPropertyEncodingTypeUnknow;
    char c = *type;
    switch (c) {
        case 'B':
            return FHPropertyEncodingTypeBool;
        case 'c':
        case 'C':
        case 's':
        case 'S':
        case 'i':
        case 'I':
        case 'l':
        case 'L':
            return FHPropertyEncodingTypeInt;
        case 'q':
        case 'Q':
            return FHPropertyEncodingTypeLong;
        case 'f':
            return FHPropertyEncodingTypeFloat;
        case 'd':
        case 'D':
            return FHPropertyEncodingTypeDouble;
        case '*':
            return FHPropertyEncodingTypeCString;
        case '@':
            return FHPropertyEncodingTypeObject;
        default:
            return FHPropertyEncodingTypeUnknow;
    }
}

FHPropertyObjectEncodingType FHPropertyObjectEncodingTypeWithType(const char *type) {
    if (type == NULL||strlen(type) <= 0) return FHPropertyObjectEncodingTypeUnkonw;
    type = type+2;
    if (strncmp(type, "NSString", 8) == 0)
        return FHPropertyObjectEncodingTypeNSString;
    else if (strncmp(type, "NSNumber", 8) == 0)
        return FHPropertyObjectEncodingTypeNSNumber;
    else if (strncmp(type, "NSData", 6) == 0)
        return FHPropertyObjectEncodingTypeNSData;
    else if (strncmp(type, "NSDate", 6) == 0)
        return FHPropertyObjectEncodingTypeNSDate;
    else if (strncmp(type, "NSArray", 7) == 0)
        return FHPropertyObjectEncodingTypeNSArray;
    else if (strncmp(type, "NSDictionary", 12) == 0)
        return FHPropertyObjectEncodingTypeNSDictionary;
    else if (strncmp(type, "NSURL", 5) == 0)
        return FHPropertyObjectEncodingTypeNSURL;
    else if (strncmp(type, "UIImage", 7) == 0)
        return FHPropertyObjectEncodingTypeUIImage;
    return FHPropertyObjectEncodingTypeNonsupport;
}

@implementation FHPropertyInfo

- (instancetype)initWithProperty:(objc_property_t)property {
    if (property == NULL) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"this method should receive a property" userInfo:nil];
        return nil;
    }
    self = [super init];
    if (self) {
        _objc_property = property;
        _name = [NSString stringWithUTF8String:property_getName(property)];
        uint32_t attribute_count = 0;
        objc_property_attribute_t *property_attributes = property_copyAttributeList(property, &attribute_count);
        for (int i=0; i<attribute_count; i++) {
            objc_property_attribute_t property_attribute = property_attributes[i];
            switch (property_attribute.name[0]) {
                case 'T': {
                    _type = [NSString stringWithUTF8String:property_attribute.value];
                    _typeEncoding = FHPropertyEncodingTypeWithType(property_attribute.value);
                    if (_typeEncoding == FHPropertyEncodingTypeObject) {
                        _isEncodingTypeObject = YES;
                        _objectTypeEncoding = FHPropertyObjectEncodingTypeWithType(property_attribute.value);
                        const char *value = property_attribute.value;
                        char *class_name = calloc(strlen(value)-2, sizeof(char));
                        memcpy(class_name, value+2, strlen(value)-3);
                        class_name[strlen(value)-3] = '\0';
                        _cls = objc_getClass(class_name);
                        free(class_name);
                    }
                } break;
                
                case 'R': {
                    _typeEncoding = FHPropertyEncodingTypeReadOnly;
                } break;
                    
                case 'G': {
                    _getter = sel_registerName(property_attribute.value);
                } break;
                
                case 'S': {
                    _setter = sel_registerName(property_attribute.value);
                } break;
                    
                default:
                    break;
            }
        }
        free(property_attributes);
    }
    if (_getter == nil) _getter = sel_registerName(_name.UTF8String);
    if (_setter == nil&&_typeEncoding != FHPropertyEncodingTypeReadOnly) _setter = sel_registerName([NSString stringWithFormat:@"set%@%@:",[[_name substringToIndex:1] uppercaseString],[_name substringFromIndex:1]].UTF8String);
    
    return self;
}

@end

@implementation FHClassInfo

@synthesize propertys = _propertys;
@synthesize superClassPropertys = _superClassPropertys;
@synthesize protocols = _protocols;

- (instancetype)init {
    @throw [NSException exceptionWithName:NSInvalidArgumentException reason:@"this method should receive a class object" userInfo:nil];
    return [self initWithClass:nil];
}

- (instancetype)initWithClass:(Class)cls {
    if (cls == nil) return nil;
    self = [super init];
    if (self) {
        _cls = cls;
        _superClass = class_getSuperclass(cls);
        _propertysInfo = [self propertysInfoWithClass:cls];
    }
    return self;
}

- (NSArray<NSString *> *)propertys {
    if (!_propertys) {
        _propertys = [self propertyListWithClass:self.cls];
    }
    return _propertys;
}

- (NSArray<NSString *> *)superClassPropertys {
    if (!_superClassPropertys) {
        _superClassPropertys = [self protocolListWithClass:self.superClass];
    }
    return _superClassPropertys;
}

- (NSArray<NSString *> *)protocols {
    if (!_protocols) {
        _protocols = [self protocolListWithClass:self.cls];
    }
    return _protocols;
}

- (NSDictionary<NSString *,FHPropertyInfo *> *)propertysInfoWithClass:(Class)cls {
    uint32_t property_count = 0;
    NSMutableDictionary *propertysInfo = [NSMutableDictionary dictionary];
    objc_property_t *propertys = class_copyPropertyList(cls, &property_count);
    for (int i=0; i<property_count; i++) {
        objc_property_t property = propertys[i];
        FHPropertyInfo *propertyInfo = [[FHPropertyInfo alloc] initWithProperty:property];
        NSParameterAssert(propertyInfo);
        [propertysInfo setObject:propertyInfo forKey:[NSString stringWithUTF8String:property_getName(property)]];
    }
    free(propertys);
    return [propertysInfo copy];
}

- (NSArray<NSString *> *)propertyListWithClass:(Class)cls {
    uint32_t property_count = 0;
    objc_property_t *propertys = class_copyPropertyList(cls, &property_count);
    NSMutableArray<NSString *> *propertyNames = [NSMutableArray arrayWithCapacity:property_count];
    for (int i=0; i<property_count; i++) {
        objc_property_t property = propertys[i];
        NSString *propertyName = [NSString stringWithUTF8String:property_getName(property)];
        NSParameterAssert(propertyName);
        [propertyNames addObject:propertyName];
    }
    free(propertys);
    return [propertyNames copy];
}

- (NSArray<NSString *> *)protocolListWithClass:(Class)cls {
    uint32_t protocol_count = 0;
    Protocol * __unsafe_unretained *protocols = class_copyProtocolList(cls, &protocol_count);
    NSMutableArray<NSString *> *protocolNames = [NSMutableArray arrayWithCapacity:protocol_count];
    for (int i=0; i<protocol_count; i++) {
        Protocol *protocol = protocols[i];
        NSString *protocolName = [NSString stringWithUTF8String:protocol_getName(protocol)];
        NSParameterAssert(protocolName);
        [protocolNames addObject:protocolName];
    }
    free(protocols);
    return [protocolNames copy];
}

+ (instancetype)infoWithClass:(Class)cls {
    static CFMutableDictionaryRef classInfoCache = nil;
    static dispatch_semaphore_t lock;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        classInfoCache = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        lock = dispatch_semaphore_create(1);
    });
    FHClassInfo *classInfo = CFDictionaryGetValue(classInfoCache, (__bridge const void *)(cls));
    if (classInfo == nil) {
        dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
        classInfo = [[self alloc] initWithClass:cls];
        NSParameterAssert(classInfo);
        CFDictionarySetValue(classInfoCache, (__bridge const void *)(cls), (__bridge const void *)(classInfo));
        dispatch_semaphore_signal(lock);
    }
    return classInfo;
}

+ (instancetype)infoWithClassName:(NSString *)name {
    return [self infoWithClass:NSClassFromString(name)];
}

@end
