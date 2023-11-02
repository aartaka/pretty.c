/* SPDX-FileCopyrightText: Artyom Bologov
   SPDX-License-Identifier: BSD-2 Clause */

#ifndef __LIBPRETTY_H__
#define __LIBPRETTY_H__

#include <iso646.h>
#include <stdbool.h>

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

#define new(type, ...) &((type){__VA_ARGS__})

#endif /* __LIBPRETTY_H__ */
