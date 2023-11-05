#ifndef __LIBPRETTY_H__
#define __LIBPRETTY_H__

#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if __STDC_NO_THREADS__
#else
#include <threads.h>
#endif

// Missing yet useful.
#define eq ==
// Fixing inconsistent bit operation names.
#define bitnot ~
#define bitxor ^

// Lua/Lisp nil.
#define nil NULL

// Ternaries.
#define when
#define unless not
#define then ?
#define otherwise :
#define notherwise : nil
#define only : nil

// Loops and blocks. Lisp, Lua, Ruby.
#define until(...) while(not (__VA_ARGS__))
#define repeat do
#define begin {
#define end }

// For each loop from basically every language.
#define foreach(var, type, length, ...)                 \
        for (type *init ## __LINE__ = (__VA_ARGS__),    \
                     *var = init ## __LINE__;           \
             var < (init ## __LINE__ + (length));       \
             var++)

// Ranges from INIT to TARGET. Python range() syntax.
#define forrangeby(var, type, init, target, by) \
        for (type var = (init);                 \
             (when ((init) >= (target))         \
              then (var > target)               \
              otherwise (var < target));        \
             (when ((init) >= (target))         \
              then (var = var - (by))           \
              otherwise (var = var + (by))))

// Ranges from INIT to TARGET. Python.
#define forrange(var, init, target)                     \
        forrangeby(long long int, var, init, target, 1)

// Repeat X times. Lisp, Lua
#define fortimes(var, times)                    \
        forrange(var, 0, times)

// A block that runs only once.
#define once                                    \
        for (bool flag_ ## __LINE__ = false;    \
             !flag_ ## __LINE__;                \
             flag_ ## __LINE__ = true)

// Tracking and freeing resources. Lisp, Python.
#define with(close, var, ...)                                   \
        for (void *flag_ ## __LINE__ = (void *) 0,              \
                     *var = (void *) (__VA_ARGS__);             \
             !flag_ ## __LINE__;                                \
             (close)(var), flag_ ## __LINE__ = (void *) 1)

//Arbitrary blocks that are break-able and goto-able. Lisp.
#define block(name)                                             \
        name: once

// Go defer, but rather block scoped and with arbitrary code in it.
#define defer(...)                                              \
        for (void *flag_ ## __LINE__ = (void *) 0;              \
             !flag_ ## __LINE__;                                \
             flag_ ## __LINE__ = (void *) 1, __VA_ARGS__)

void *_allocpy (size_t size, void *contents)
{
        void *allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

// Easy resource allocation akin to C++.
#define new(type, ...) _allocpy(sizeof(type), &((type) {__VA_ARGS__}))

// Easy array allocation. C++ vector, but more primitive.
#define vector(length, type, ...)                                       \
        _allocpy(sizeof(type) * length, (type[length]){__VA_ARGS__})

#if __STDC_NO_THREADS__
#else
// Go-style coroutine syntax.
thrd_t *go(thrd_start_t fn, void *arg)
{
        thrd_t *thrd = malloc(sizeof(thrd_t));
        thrd_create(thrd, fn, arg);
        return thrd;
}
#endif

#define try errno = 0; once
#define catch switch (errno)
#define NOERROR 0
#define NOERR 0

#endif /* __LIBPRETTY_H__ */
