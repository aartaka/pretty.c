/* SPDX-FileCopyrightText: Artyom Bologov
   SPDX-License-Identifier: BSD-2 Clause */

#ifndef __LIBPRETTY_H__
#define __LIBPRETTY_H__

#include <iso646.h>

#define when
#define then ?
#define otherwise :

#define for_each(var, type, length, ...)                        \
        for (type *init##__LINE__ = (type *) (__VA_ARGS__),     \
                     *var = init##__LINE__;                     \
             var < (init##__LINE__ + length);                   \
             var++)

// for each (i, int, 10, array)

#define for_range(var, init, target)            \
        for (int var = init;                    \
             var != target;                     \
             (when (init > target)              \
              then var--                        \
              otherwise var++))

#define for_times(var, times)                   \
        for_range(var, 0, times)



#define with(close, var, ...)                           \
        for (int i_##__LINE__ = 0,                      \
                     void *var = (void *) __VA_ARGS__;  \
             i_##__LINE__ < 1;                          \
             i_##__LINE__ ++, (close)(var))

void *_new (size_t size, void *contents)
{
        void *allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

#define new(type, ...) _new(sizeof(type), &((type){__VA_ARGS__}))

#endif /* __LIBPRETTY_H__ */
