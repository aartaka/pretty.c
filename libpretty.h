#ifndef LIBPRETTY_H
#define LIBPRETTY_H

#include <iso646.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Missing yet useful.
#define eq ==
// Fixing inconsistent bit operation names.
#define bitnot ~
#define bitxor ^

// Frequently used types.
typedef char* string;
typedef char  byte;
typedef byte* bytes;
typedef void* any;
// Suckless, and consistent with uint32_t etc.
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

// Small macros.
#undef min
#define min(a, b) (when (a) below (b) \
                   then (a)           \
                   otherwise (b))

#undef max
#define max(a, b) (when (a) above (b)           \
                   then (a)                     \
                   otherwise (b))

#undef len
#if __STDC_VERSION__ >= 201112
#define len(...) _Generic((__VA_ARGS__),                                \
                          string: strlen((string)(__VA_ARGS__)),        \
                          default: (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__)[0]))
#else
#define len(...) (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__)[0])
#endif

// Comparion operators. Lisp loop macro keywords.
#define below <
#define above >
#define upto <=
#define downto >=
#define positive 0 <
#define negative 0 >
#define ispos 0 <
#define isneg 0 >
#define iszero 0 ==
#define isnull nil ==

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
#define always while(true)
#define never while(false)
#define repeat do

// Tracking and freeing resources. Lisp, Python.
#define with(close, var, ...)                                   \
        for (void *flag_ ## __LINE__ = (any) 1,                 \
                     *var = (any) (__VA_ARGS__);                \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (any) 0)

// Ranges from INIT to TARGET. Python range() syntax.
#define forrangeby(var, type, init, target, by)         \
        for (type var = (type)(init);                   \
             (when ((init) >= ((type) target))          \
              then (var > ((type) target))              \
              otherwise (var < ((type) target)));       \
             (when ((init) >= ((type) target))          \
              then (var = var - (by))                   \
              otherwise (var = var + (by))))

#define forrange(var, init, target)             \
        forrangeby(var, int, init, target, 1)

// Repeat X times. Lisp, Lua
#define fortimes(var, ...)                                      \
        for (int var = 0, result_ ## __LINE__ = (__VA_ARGS__);  \
             var < result_ ## __LINE__;                         \
             ++var)

// Local/lexical bindings.
#define let(var, type, ...)                             \
        for (type var = (__VA_ARGS__),                  \
                     *flag_ ## __LINE__ = (any) 1;      \
             ((any) flag_ ## __LINE__);                 \
             flag_ ## __LINE__ = (any) 0)
#define local(var, type, ...)                   \
        let (var, type, __VA_ARGS__)

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

static any
allocpy (int size, any contents)
{
        bytes allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

// Easy resource allocation akin to C++.
#define new(type, ...)                                          \
        (type *) allocpy(sizeof(type), &((type) {__VA_ARGS__}))

// Easy array allocation. C++ vector, but more primitive.
// FIXME: Enforce array type somehow?
#define vector(length, type, ...)                       \
        (type*) allocpy(sizeof(type) * length,          \
                        (type[length]){__VA_ARGS__})

// Go defer, but rather block scoped and with arbitrary code in it.
#define defer(...)                                      \
        for (bool flag_ ## __LINE__ = true;             \
             flag_ ## __LINE__;                         \
             flag_ ## __LINE__ = false, (__VA_ARGS__))

#define try                                     \
        errno = 0;                              \
        for (bool flag_ ## __LINE__ = true;     \
             flag_ ## __LINE__;                 \
             flag_ ## __LINE__ = false)


static bool
err_part_of (int err, size_t length, int *errs)
{
        foreach(i, int, length, errs)
                if (err eq *i)
                        return true;
        return false;
}

#define catch(...)                                                      \
        if (err_part_of(errno,                                          \
                        len((int[]){__VA_ARGS__}),                      \
                        (int[]){__VA_ARGS__}))
#define NOERROR 0
#define NOERR 0

#endif /* LIBPRETTY_H */
