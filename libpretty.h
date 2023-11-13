#ifndef LIBPRETTY_H
#define LIBPRETTY_H

#include <iso646.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

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
#define positive 0 <
#define negative 0 >

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
#define forrangeby(var, type, init, target, by)         \
        for (type var = (type)(init);                   \
             (when ((init) >= ((type) target))          \
              then (var > ((type) target))              \
              otherwise (var < ((type) target)));       \
             (when ((init) >= ((type) target))          \
              then (var = var - (by))                   \
              otherwise (var = var + (by))))

// Ranges from INIT to TARGET. Python.
#define forrange(var, init, target)             \
        forrangeby(var, int, init, target, 1)

// Repeat X times. Lisp, Lua
#define fortimes(var, times)                             \
        for (int var = 0, result_ ## __LINE__ = times;   \
             var < result_ ## __LINE__;                  \
             ++var)

#define let(var, type, ...)                                             \
        for (type var = (__VA_ARGS__),                                  \
                     *flag_ ## __LINE__ = (void *) 1;                   \
             ((void *) flag_ ## __LINE__);                              \
             flag_ ## __LINE__ = (void *) 0)

#define local(var, type, ...)                   \
        let (var, type, __VA_ARGS__)

// Tracking and freeing resources. Lisp, Python.
#define with(close, var, ...)                                   \
        for (void *flag_ ## __LINE__ = (void *) 1,              \
                     *var = (void *) (__VA_ARGS__);             \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (void *) 0)

static void *
allocpy (size_t size, void *contents)
{
        char *allocated = malloc(size);
        char *char_contents = (char *) contents;
        for (int i = 0; i < size; ++i)
                allocated[i] = char_contents[i];
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
                        sizeof((int[]){__VA_ARGS__}) / sizeof(int),     \
                        (int[]){__VA_ARGS__}))
#define NOERROR 0
#define NOERR 0

#endif /* LIBPRETTY_H */
