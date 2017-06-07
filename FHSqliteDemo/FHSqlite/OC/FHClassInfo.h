//
//  FHClassInfo.h
//  FHClassInfoDemo
//
//  Created by 李浩 on 2017/6/3.
//  Copyright © 2017年 GodL. All rights reserved.
//

#import <Foundation/Foundation.h>
@import ObjectiveC.runtime;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger,FHPropertyEncodingType) {
    FHPropertyEncodingTypeUnknow,
    FHPropertyEncodingTypeBool,
    FHPropertyEncodingTypeInt,
    FHPropertyEncodingTypeLong,
    FHPropertyEncodingTypeFloat,
    FHPropertyEncodingTypeDouble,
    FHPropertyEncodingTypeCString,
    FHPropertyEncodingTypeObject,
    FHPropertyEncodingTypeReadOnly = 0xFF-1
};

typedef NS_ENUM(NSInteger,FHPropertyObjectEncodingType) {
    FHPropertyObjectEncodingTypeUnkonw = 0xFF,
    FHPropertyObjectEncodingTypeNonsupport,
    FHPropertyObjectEncodingTypeNSString,
    FHPropertyObjectEncodingTypeNSNumber,
    FHPropertyObjectEncodingTypeNSData,
    FHPropertyObjectEncodingTypeNSDate,
    FHPropertyObjectEncodingTypeNSArray,
    FHPropertyObjectEncodingTypeNSDictionary,
    FHPropertyObjectEncodingTypeNSURL,
    FHPropertyObjectEncodingTypeUIImage
};

@interface FHPropertyInfo : NSObject

@property (nonatomic,assign,readonly) objc_property_t objc_property;

@property (nonatomic,copy,readonly) NSString *name;

@property (nonatomic,copy,readonly) NSString *type;

@property (nonatomic,assign,readonly) BOOL isEncodingTypeObject;

@property (nonatomic,assign,readonly,nullable) Class cls;

@property (nonatomic,assign,readonly) FHPropertyEncodingType typeEncoding;

@property (nonatomic,assign,readonly) FHPropertyObjectEncodingType objectTypeEncoding;

@property (nonatomic,assign,readonly) SEL getter;

@property (nonatomic,assign,readonly,nullable) SEL setter;

- (instancetype)initWithProperty:(objc_property_t)propertyNS_DESIGNATED_INITIALIZER;

@end

@interface FHClassInfo : NSObject

@property (nonatomic,assign,readonly) Class cls;

@property (nonatomic,assign,readonly) Class superClass;

@property (nonatomic,copy,readonly) NSArray<NSString *> *propertys;

@property (nonatomic,copy,readonly) NSArray<NSString *> *propertysExceptReadonly;

@property (nonatomic,copy,readonly) NSArray<NSString *> *superClassPropertys;

@property (nonatomic,copy,readonly) NSArray<NSString *> *protocols;

@property (nonatomic,copy,readonly) NSDictionary<NSString *,FHPropertyInfo *> *propertysInfo;

- (instancetype)initWithClass:(Class)cls NS_DESIGNATED_INITIALIZER;

+ (instancetype)infoWithClass:(Class)cls;

+ (instancetype)infoWithClassName:(NSString*)name;

@end

NS_ASSUME_NONNULL_END
