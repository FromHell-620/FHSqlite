//
//  fh_define.h
//  FHSqliteDemo
//
//  Created by imac on 2018/8/13.
//  Copyright © 2018年 FromHell. All rights reserved.
//

#ifndef fh_define_h
#define fh_define_h
#include <stdbool.h>

#ifdef __cplusplus
#define FH_EXTERN        extern "C" __attribute__((visibility ("default")))
#else
#define FH_EXTERN            extern __attribute__((visibility ("default")))
#endif


#if !defined(FH_INLINE)
    #if defined(__GNUC__) && (__GNUC__ == 4) && !defined(DEBUG)
        #define FH_INLINE static __inline__ __attribute__((always_inline))
    #elif defined(__GNUC__)
        #define FH_INLINE static __inline__
    #elif defined(__cplusplus)
        #define FH_INLINE static inline
    #elif defined(_MSC_VER)
        #define FH_INLINE static __inline
    #elif YM_OS_WINDOWS
        #define FH_INLINE static __inline__
    #else
        #define FH_INLINE static inline
    #endif
#endif

typedef bool FHBoolean;
typedef unsigned long FHIndex;

#endif /* fh_define_h */
