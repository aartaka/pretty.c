/* SPDX-FileCopyrightText: Artyom Bologov
   SPDX-License-Identifier: BSD-2 Clause */

#ifndef __LIBPRETTY_H__
#define __LIBPRETTY_H__

#include <iso646.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

/* Missing yet useful. */
#define eq ==
/* Fixing inconsistent bit operations. */
#define bitnot ~
#define bitxor ^

#define when
#define then ?
#define otherwise :

#define foreach(type, var, length, ...)                                 \
        for (type *init ## __LINE__ = (__VA_ARGS__),                    \
                     *var = init ## __LINE__;                           \
             var < (init ## __LINE__ + length);                         \
             var++)

#define forrange(var, init, target)                                     \
        for (long long int var = init;                                  \
             var != target;                                             \
             (when (init > target)                                      \
              then var--                                                \
              otherwise var++))

#define fortimes(var, times)                    \
        forrange(var, 0, times)

#define with(close, var, ...)                                    \
        for (void *flag_##__LINE__ = (void *) 0,                 \
                     *var = (void *) (__VA_ARGS__);              \
             !flag_##__LINE__;                                  \
             (close)(var), flag_##__LINE__ = (void *) 1)

void *_new (size_t size, void *contents)
{
        void *allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

#define new(type, ...) _new(sizeof(type), &((type) {__VA_ARGS__}))

#define type (val) _Generic(val,                \
                            char: TYPE_CHAR,    \
                            unsigned char: TYPE_UCHAR,\
                            signed char: TYPE_SCHAR,\
                            short: TYPE_SHRT,\
                            unsigned short: TYPE_USHRT,\
                            int, TYPE_INT,\
                            unsigned int, TYPE_UINT\
                            long: TYPE_LONG,\
                            unsigned long: TYPE_ULONG,\
                            long long: TYPE_LLONG,\
                            unsigned long long: TYPE_ULLONG,    \
                            float: TYPE_FLT,\
                            double: TYPE_DBL, \
                            long double: TYPE_LDBL,\
                            bool: TYPE_BOOL,\
                            intmax_t: TYPE_INTMAX,\
                            uintmax_t: TYPE_UINTMAX,\
                            int8_t: TYPE_INT8,\
                            uint8_t: TYPE_UINT8,\
                            int16_t: TYPE_INT16,\
                            uint16_t: TYPE_UINT16,\
                            int32_t: TYPE_INT32,\
                            uint32_t: TYPE_UINT32,\
                            int64_t: TYPE_INT64,\
                            uint64_t: TYPE_UINT64,\
                            int_least8_t: TYPE_INT_LEAST8,\
                            uint_least8_t: TYPE_UINT_LEAST8,  \
                            int_least16_t: TYPE_INT_LEAST16,\
                            uint_least16_t: TYPE_UINT_LEAST16,  \
                            int_least32_t: TYPE_INT_LEAST32,\
                            uint_least32_t: TYPE_UINT_LEAST32,  \
                            int_least64_t: TYPE_INT_LEAST64,\
                            uint_least64_t: TYPE_UINT_LEAST64,  \
                            int_fast8_t: TYPE_INT_FAST8,\
                            uint_fast8_t: TYPE_UINT_FAST8,  \
                            int_fast16_t: TYPE_INT_FAST16,\
                            uint_fast16_t: TYPE_UINT_FAST16,  \
                            int_fast32_t: TYPE_INT_FAST32,\
                            uint_fast32_t: TYPE_UINT_FAST32,  \
                            int_fast64_t: TYPE_INT_FAST64,\
                            uint_fast64_t: TYPE_UINT_FAST64,  \
                            intptr_t: TYPE_INTPTR, \
                            uintptr_t: TYPE_UINTPTR,    \
                            size_t: TYPE_SIZE,          \
                            ptrdiff_t: TYPE_PTRDIFF,    \
                            sig_atomic_t: TYPE_SIG_ATOMIC,\
                            wchar_t: TYPE_WCHAR,          \
                            wint_t: TYPE_WINT,            \
                            nullptr_t: TYPE_NULLPTR       \
                            /* TODO. */                   \
                )

#endif /* __LIBPRETTY_H__ */
