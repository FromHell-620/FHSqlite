//
//  fh_hash.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/12.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_hash.h"
#include <string.h>
#include <stdlib.h>

#if !defined(__LP64__)
#define __LP64__ 0
#endif

#define FHTypeRef const void *

static const FHIndex __FHHashTableCapacities[64] = {
    7, 13, 19, 31, 47, 69, 97, 117, 133, 155, 237, 390, 672, 1065,
    1732, 2795, 4543, 7391, 12019, 19302, 31324, 50629, 81956,
    132580, 214215, 346784, 561026, 907847, 1468567, 2376414,
    3844982, 6221390, 10066379, 16287773, 26354132, 42641916,
    68996399, 111638327, 180634415, 292272755,
    472907503UL, 765180257UL, 1238087439UL, 2003267722UL, 3241355160UL,
    5244622578UL, 8485977737UL, 13730600347UL, 22216578100UL,
    35947178453UL, 58163756541UL, 94110935011UL, 152274691274UL,
    246385626296UL, 398660317578UL, 645045943559UL, 1043706261135UL,
    1688752204693UL, 2732458465840UL, 4421210670552UL,
    7153669136706UL, 11574879807265UL, 18728548943682UL
};

static float const __FHHashResizeFactor = 0.75;

static const void *__FHHashStringRetainCallback(const void *key) {
    return strdup(key);
}

static void __FHHashStringReleaseCallback(const void *key) {
    free((void *)key);
}

static FHBoolean __FHHashStringEqualCallback(const void *key1,const void *key2) {
    return strcmp(key1, key2) == 0;
}

static FHHashCode __FHHashCodeDefaultCallback(const void *key) {
    char *c = (char *)key;
    unsigned long ret = 0;
    long n;
    unsigned long v;
    int r;
    
    if ((c == NULL) || (*c == '\0'))
        return (ret);
    
    n = 0x100;
    while (*c)
    {
        v = n | (*c);
        n += 0x100;
        r = (int)((v >> 2) ^ v) & 0x0f;
        ret = (ret << r) | (ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        c ++;
    }
    return((ret >> 16) ^ ret);
}

struct FHHash {
    FHIndex count;
    FHIndex bucketCount;
    FHHashEntryRef *buckets;
    FHHashKeyCallback keyCallback;
    FHHashValueCallback valueCallback;
};

const FHHashKeyCallback kFHCopyStringKeyCallback = {
    __FHHashStringRetainCallback,
    __FHHashStringReleaseCallback,
    __FHHashStringEqualCallback,
    __FHHashCodeDefaultCallback
};

const FHHashValueCallback kFHCopyStringValueCallback = {
    __FHHashStringRetainCallback,
    __FHHashStringReleaseCallback,
    __FHHashStringEqualCallback
};

FH_INLINE FHIndex __FHBucketCountWithCapacity(FHIndex capacity) {
    unsigned int index = 0;
    while (__FHHashTableCapacities[index] < capacity) {
        index ++;
    }
    return __FHHashTableCapacities[index];
}

FH_INLINE FHIndex __FHBucketIndexWithHashCode(FHHashRef hash,FHHashCode hashCode) {
    return hashCode & hash->bucketCount;
}

FH_INLINE FHBoolean __FHBucketInsertHashEntry(FHHashEntryRef *buckets,FHHashEntryRef hashEntry,FHIndex index,FHBoolean shouldCompare) {
    FHHashEntryRef existEntry = buckets[index];
    FHBoolean boolean = false;
    if (existEntry == NULL) {
        buckets[index] = hashEntry;
    }else {
        while (existEntry != NULL) {
            if (shouldCompare && existEntry->hash == hashEntry->hash) {
                existEntry->value = hashEntry->value;
                boolean = true;
                break;
            }
            if (existEntry->next == NULL) {
                existEntry->next = hashEntry;
                hashEntry->next = NULL;
                break;
            }
            existEntry = existEntry->next;
        }
    }
    return boolean;
}

FH_INLINE void __FHHashRehash(FHHashRef hash) {
    FHIndex bucketCount = __FHBucketCountWithCapacity(hash->bucketCount);
    FHHashEntryRef *oldBuckets = hash->buckets;
    if ((hash->buckets = calloc(bucketCount, sizeof(FHHashEntryRef))) == NULL) {
        hash->buckets = oldBuckets;
        return;
    }
    for (FHIndex i=0,tempCount=0; i<hash->bucketCount; i++) {
        if (tempCount == hash->count) break;
        FHHashEntryRef hashEntry = oldBuckets[i];
        FHHashEntryRef nextEntry = NULL;
        while (hashEntry != NULL) {
            nextEntry = hashEntry->next;
            FHIndex bucketIndex = hashEntry->hash & bucketCount;
            __FHBucketInsertHashEntry(hash->buckets, hashEntry, bucketIndex, false);
            hashEntry = nextEntry;
        }
    }
    free(oldBuckets);
    hash->bucketCount = bucketCount;
}

FHHashRef FHHashCreate() {
    return FHHashCreateWithOptions(0, NULL, NULL);
}

FHHashRef FHHashCreateWithOptions(FHIndex capacity,const FHHashKeyCallback *keyCallback,const FHHashValueCallback *valueCallback) {
    FHHashRef hash ;
    if ((hash = calloc(1, sizeof(struct FHHash))) == NULL) return NULL;
    FHIndex bucketCount = __FHBucketCountWithCapacity(capacity);
    if ((hash->buckets = calloc(bucketCount, sizeof(FHHashEntry *))) == NULL) {
        free(hash);
        return NULL;
    }
    hash->count = 0;
    hash->bucketCount = bucketCount;
    if (keyCallback) hash->keyCallback = *keyCallback;
    if (valueCallback) hash->valueCallback = *valueCallback;
    if (hash->keyCallback.hash == NULL) hash->keyCallback.hash = __FHHashCodeDefaultCallback;
    return hash;
}

FHIndex FHHashGetCount(FHHashRef hash) {
    return hash == NULL ? 0 : hash->count;
}

FHBoolean FHHashContainsKey(FHHashRef hash,const void *key) {
    if (hash == NULL || key == NULL) return false;
    FHHashCode hashCode = hash->keyCallback.hash(key);
    FHIndex bucketIndex = __FHBucketIndexWithHashCode(hash, hashCode);
    FHHashEntryRef hashEntry = hash->buckets[bucketIndex];
    FHBoolean boolean = false;
    while (hashEntry != NULL) {
        boolean = hash->keyCallback.equal ? hash->keyCallback.equal(key,hashEntry->key) : hashEntry->hash == hashCode;
        if (boolean) break;
        hashEntry = hashEntry->next;
    }
    return boolean;
}

FHBoolean FHHashContainsValue(FHHashRef hash,const void *value) {
    if (hash == NULL || value == NULL) return false;
    FHBoolean boolean = false;
    for (FHIndex i=0,tempCount=0; i<hash->bucketCount; i++) {
        if (tempCount == hash->count) break;
        FHHashEntryRef bucket = hash->buckets[i];
        if (bucket == NULL) continue;
        while (bucket != NULL) {
            boolean = hash->valueCallback.equal ? hash->valueCallback.equal(bucket->value,value) : bucket->value == value;
            if (boolean) break;
            bucket = bucket->next;
            ++ tempCount;
        }
        if (boolean) break;
    }
    return boolean;
}

const void *FHHashGetValue(FHHashRef hash,const void *key) {
    if (hash == NULL || key == NULL) return NULL;
    FHHashCode hashCode = hash->keyCallback.hash(key);
    FHIndex bucketIndex = __FHBucketIndexWithHashCode(hash, hashCode);
    FHHashEntryRef hashEntry = hash->buckets[bucketIndex];
    FHTypeRef value = NULL;
    while (hashEntry != NULL) {
        if (hashCode == hashEntry->hash) {
            value = hashEntry->value;
            break;
        }
        hashEntry = hashEntry->next;
    }
    return value;
}

void FHHashSetValue(FHHashRef hash,const void *key,const void *value) {
    if (hash == NULL || key == NULL || value == NULL) return ;
    FHHashCode hashCode = hash->keyCallback.hash(key);
    FHIndex bucketIndex = __FHBucketIndexWithHashCode(hash, hashCode);
    FHHashEntryRef insertEntry;
    if ((insertEntry = calloc(1, sizeof(FHHashEntry))) == NULL) return;
    insertEntry->key = hash->keyCallback.retain ? hash->keyCallback.retain(key) : key;
    insertEntry->value = hash->valueCallback.retain ? hash->valueCallback.retain(value) : value;
    insertEntry->hash = hashCode;
    __FHBucketInsertHashEntry(hash->buckets, insertEntry, bucketIndex, true) ? free(insertEntry) : hash->count ++;
    if (hash->count >= hash->bucketCount * __FHHashResizeFactor) __FHHashRehash(hash);
}

void FHHashRemoveValue(FHHashRef hash,const void *key) {
    if (hash == NULL || key == NULL) return;
    FHHashCode hashCode = hash->keyCallback.hash(key);
    FHIndex bucketIndex = __FHBucketIndexWithHashCode(hash, hashCode);
    FHHashEntryRef hashEntry = hash->buckets[bucketIndex];
    FHHashEntryRef prveEntry = NULL;
    while (hashEntry) {
        if (hash->keyCallback.equal ? hash->keyCallback.equal(hashEntry->key,key) : hashEntry->hash == hashCode) break;
        prveEntry = hashEntry;
        hashEntry = hashEntry->next;
    }
    if (hashEntry == NULL) return ;
    if (prveEntry == NULL) *hash->buckets = hashEntry->next;
    else    prveEntry->next = hashEntry->next;
    if (hash->keyCallback.release) hash->keyCallback.release(hashEntry->key);
    if (hash->valueCallback.release) hash->keyCallback.release(hashEntry->value);
    free(hashEntry);
    hash->count -- ;
}

void FHHashClean(FHHashRef hash) {
    if (hash == NULL || FHHashGetCount(hash) == 0) return;
    for (FHIndex i=0,tempCount=0; i<hash->bucketCount; i++) {
        if (tempCount == hash->count) break;
        FHHashEntryRef hashEntry = hash->buckets[i];
        while (hashEntry) {
            if (hash->keyCallback.release) hash->keyCallback.release(hashEntry->key);
            if (hash->valueCallback.release) hash->valueCallback.release(hashEntry->value);
            FHHashEntryRef nextEntry = hashEntry->next;
            free(hashEntry);
            hashEntry = nextEntry;
            ++ tempCount;
        }
        hash->buckets[i] = NULL;
    }
    hash->count = 0;
}

void FHHashApplyFuncation(FHHashRef hash,FHHashApplierFunction applier,void *context) {
    if (hash == NULL || applier == NULL) return;
    for (FHIndex i=0,tempCount=0; i<hash->count; i++) {
        if (tempCount == hash->count) return;
        FHHashEntryRef hashEntry = hash->buckets[i];
        while (hashEntry) {
            applier(hashEntry->key,hashEntry->value,context);
            hashEntry = hashEntry->next;
            ++ tempCount;
        }
    }
}
