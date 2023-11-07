#ifndef __LIBPRETTY_H__
#define __LIBPRETTY_H__

#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Missing yet useful.
#define eq ==
// Fixing inconsistent bit operation names.
#define bitnot ~
#define bitxor ^

// Comparion operators. Lisp loop macro keywords.
#define below <
#define above >
#define upto <=
#define downto >=

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
#define foreach(var, type, length, ...)                                 \
        let (init ## __LINE__, type*, (__VA_ARGS__))                    \
                fortimes (offset ## __LINE__, length)                   \
                let (var, type*, (init ## __LINE__ + offset ## __LINE__))

// Loop over the provided arguments.
#define forthese(var, type, ...)                                        \
        let (init ## __LINE__, type*, (type[]){__VA_ARGS__})            \
                fortimes (offset ## __LINE__,                           \
                          (sizeof((type[]){__VA_ARGS__})                \
                           / sizeof(type)))                             \
                let (var, type, *(init ## __LINE__ + offset ## __LINE__))

// Ranges from INIT to TARGET. Python range() syntax.
#define forrangeby(var, type, init, target, by)               \
        for (type var = (type)(init);                         \
             (when ((init) >= ((type) target))                \
              then (var > ((type) target))                    \
              otherwise (var < ((type) target)));             \
             (when ((init) >= ((type) target))                \
              then (var = var - (by))                         \
              otherwise (var = var + (by))))

// Ranges from INIT to TARGET. Python.
#define forrange(var, init, target)                     \
        forrangeby(var, long long int, init, target, 1)

// Dispatch over integer/value constant types

// no suffix
/* int */
/* long int */
/* unsigned long int (until C99) */
/* long long int (since C99) */

// U
/* unsigned int */
/* unsigned long int */
/* unsigned long long int(since C99) */

// L
/* long int */
/* unsigned long int(until C99) */
/* long long int(since C99) */

// LU
/* unsigned long int */
/* unsigned long long int(since C99) */

// LL
/* long long int(since C99) */

// LLU
/* unsigned long long int(since C99)  */

// Repeat X times. Lisp, Lua
#define fortimes(var, times)                    \
        forrange(var, 0, times)

#define let(var, type, ...)                                             \
        for (type var = (__VA_ARGS__),                                  \
                     *flag_ ## __LINE__ = (void *) 0;                   \
             !((void *) flag_ ## __LINE__);                             \
             flag_ ## __LINE__ = (void *) 1)

#define local(var, type, ...)                   \
        let (var, type, __VA_ARGS__)

// Tracking and freeing resources. Lisp, Python.
#define with(close, var, ...)                                   \
        for (void *flag_ ## __LINE__ = (void *) 0,              \
                     *var = (void *) (__VA_ARGS__);             \
             !flag_ ## __LINE__;                                \
             (close)(var), flag_ ## __LINE__ = (void *) 1)

void *
_allocpy (size_t size, void *contents)
{
        void *allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

// Easy resource allocation akin to C++.
#define new(type, ...) (type *) \
        _allocpy(sizeof(type), &((type) {__VA_ARGS__}))

// Easy array allocation. C++ vector, but more primitive.
// FIXME: Enforce array type somehow?
#define vector(length, type, ...)                                       \
        (type*) _allocpy(sizeof(type) * length,                         \
                                (type[length]){__VA_ARGS__})

// Go defer, but rather block scoped and with arbitrary code in it.
#define defer(...)                                              \
        for (void *flag_ ## __LINE__ = (void *) 0;              \
             !flag_ ## __LINE__;                                \
             flag_ ## __LINE__ = (void *) 1, __VA_ARGS__)

#define try                                     \
        errno = 0;                              \
        for (bool flag_ ## __LINE__ = false;    \
             !flag_ ## __LINE__;                \
             flag_ ## __LINE__ = true)


bool _err_part_of (int err, size_t length, int *errs)
{
        foreach(i, int, length, errs)
                if (err eq *i)
                        return true;
        return false;
}

#define catch(...)                                                 \
        if (_err_part_of(errno,                                    \
                         sizeof((int[]){__VA_ARGS__}) / sizeof(int),    \
                         (int[]){__VA_ARGS__}))
#define NOERROR 0
#define NOERR 0

#endif /* __LIBPRETTY_H__ */
