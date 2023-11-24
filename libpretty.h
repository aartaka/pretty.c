#ifndef LIBPRETTY_H
#define LIBPRETTY_H

#if __STDC_VERSION__ < 199901L
#error "Libpretty only works on C99+. Switch to it if you didn't yet."
#endif

#include <complex.h>
#include <errno.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Missing yet useful.
#define eq ==
// SQL.
#define is ==
// Fixing inconsistent bit operation names.
#define bitnot ~
#define bitxor ^

// Frequently used types. Suckless and Go.
typedef char*          string;
typedef char           byte;
typedef char*          bytes;
typedef void*          any;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

// Small macros. Mostly from Suckless.
#define default(a, b) ((a) ? (a) : (b))
#define min(a, b)     ((a) < (b) ? (a) : (b))
#define max(a, b)     ((a) > (b) ? (a) : (b))
#if __STDC_VERSION__ >= 201112
#define len(...) _Generic((__VA_ARGS__),                                \
                          char*: strlen((char*)(__VA_ARGS__)),          \
                          default: (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__)[0]))
#else
#define len(...) (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__)[0])
#endif

// Lua/Lisp nil.
#define nil NULL

// Comparion operators. Lisp loop macro and SQL keywords.
#define below    <
#define above    >
#define upto     <=
#define downto   >=
#define positive 0 <
#define negative 0 >

// Ternaries and conditionals.
#define elif      else if
#define when
#define unless    not
#define then      ?
#define otherwise :
#define otherwhen :
#define only      : NULL

// Dynamically-typed variable declarations
#if defined(__GNUC__) || defined(__GNUG__)
#define var   __auto_type
#define let   __auto_type
#define local __auto_type
#elif __STDC_VERSION__ >= 202311L
#define var   auto
#define let   auto
#define local auto
#else
#define var
#define let
#define local
#endif

// Loops and blocks. Lisp, Lua, Ruby.
#define until(...) while(!(__VA_ARGS__))
#define always     while(true)
#define never      while(false)
#define repeat     do

// Tracking and freeing resources. Lisp, Python.
#if (__STDC_VERSION__ >= 202311L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#define with(close, var, ...)                                   \
        for (typeof((__VA_ARGS__)) var = (__VA_ARGS__)          \
                     *flag_ ## __LINE__ = (void*) true;         \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (void*) false)
#else
#define with(close, var, ...)                                   \
        for (void *var = (void*) (__VA_ARGS__),                 \
                     *flag_ ## __LINE__ = (void*) true;         \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (void*) false)
#endif

// Ranges from INIT to TARGET. Python range() syntax.
#define forrangeby(var, type, init, target, by)         \
        for (type var = (type)(init);                   \
             (((init) >= ((type) target))               \
              ? (var > ((type) target))                 \
              : (var < ((type) target)));               \
             (((init) >= ((type) target))               \
              ? (var -= (by))                      \
              : (var += (by))))

#if (__STDC_VERSION__ >= 202311L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#define forrange(var, init, ...)                                        \
        for (typeof((init)+(__VA_ARGS__)) init_ ## __LINE__ = (init),   \
                     var = init_ ## __LINE__,                           \
                     target_ ## __LINE__ = (__VA_ARGS__);               \
             ((init_ ## __LINE__ >= target_ ## __LINE__)                \
              ? (var > target_ ## __LINE__)                             \
              : (var < target_ ## __LINE__));                           \
             var += ((init_ ## __LINE__ >= target_ ## __LINE__) ? -1 : +1))
#else
#define forrange(var, init, ...)                                        \
        for (int init_ ## __LINE__ = (init),                            \
                     var = init_ ## __LINE__,                           \
                     target_ ## __LINE__ = (__VA_ARGS__);               \
             var != target_ ## __LINE__;                                \
             var += ((init_ ## __LINE__ >= target_ ## __LINE__) ? -1 : +1))
#endif

// Repeat X times. Lisp, Lua
#if  (__STDC_VERSION__ >= 202311L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#define fortimes(var, ...)                                              \
        for (typeof((__VA_ARGS__)) result_ ## __LINE__ = (__VA_ARGS__), \
                     var = (typeof((__VA_ARGS__))) 0;                   \
             var < result_ ## __LINE__;                                 \
             var += 1)
#else
#define fortimes(var, ...)                                      \
        for (int var = 0, result_ ## __LINE__ = (__VA_ARGS__);  \
             var < result_ ## __LINE__;                         \
             var += 1)
#endif

// For each loop from basically every language.
#define foreach(var, type, length, ...)                                 \
        for (type *init ## __LINE__ = (__VA_ARGS__),                    \
                     *flag_ ## __LINE__ = (void*) true;                 \
             flag_ ## __LINE__;                                         \
             flag_ ## __LINE__ = (void*) false)                         \
                for (int offset ## __LINE__ = 0;                        \
                     offset ## __LINE__ < length;                       \
                     offset ## __LINE__ += 1)                           \
                        for (type *var = (init ## __LINE__ + offset ## __LINE__), \
                                     *flag2_ ## __LINE__ = (void*) true; \
                             flag2_ ## __LINE__;                         \
                             flag2_ ## __LINE__ = (void*) false)

// Loop over the provided arguments.
#define forthese(var, type, ...)                                        \
        for (type *init ## __LINE__ = (type[]){__VA_ARGS__},            \
                     *flag_ ## __LINE__ = (void*) true;                 \
             flag_ ## __LINE__;                                         \
             flag_ ## __LINE__ = (void*) false)                         \
                for (int offset ## __LINE__ = 0;                        \
                     offset ## __LINE__ < (sizeof((type[]){__VA_ARGS__}) \
                                           / sizeof(type));             \
                     offset ## __LINE__ += 1)                           \
                        for (type var = init ## __LINE__ [offset ## __LINE__], \
                                     *flag2_ ## __LINE__ = (void*) true; \
                             flag2_ ## __LINE__;                        \
                             flag2_ ## __LINE__ = (void*) false)

static void*
allocpy (int size, void *contents)
{
        char* allocated = malloc(size);
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

// TODO: A macro to allocate struct + flexible array member.

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
        for (int i = 0; i < length; ++i)
                if (err == errs[i])
                        return true;
        return false;
}

#define catch(...)                                                      \
        if (err_part_of(errno,                                          \
                        sizeof ((int[]){__VA_ARGS__}) / sizeof(int),    \
                        (int[]){__VA_ARGS__}))
#define NOERROR 0
#define NOERR 0

#if defined(__clang__)
#define lambda(ret, name, ...) void * name = (void*) ^ ret (__VA_ARGS__)
#elif defined(__GNUC__) || defined(__GNUG__)
#define lambda(ret, name, ...) ret name (__VA_ARGS__)
#elif defined(__cplusplus)
#define lambda(ret, name, ...) auto name = [](__VA_ARGS__)
#endif

#define print(x)                                        \
        printf(_Generic((x),                            \
                        char*: "%s\n",                  \
                        wchar_t*: "%ls\n",              \
                        signed char: "%hhi\n",          \
                        short: "%hi\n",                 \
                        int: "%i\n",                    \
                        long: "%li\n",                  \
                        long long: "%lli\n",            \
                        unsigned char: "%hhu\n",          \
                        unsigned short: "%hi\n",        \
                        unsigned int: "%u\n",           \
                        unsigned long: "%lu\n",         \
                        unsigned long long: "%llu\n",   \
                        float: "%g\n",                  \
                        double: "%g\n",                 \
                        long double: "%Lg\n",           \
                        default: "%p\n"),               \
               x)

#endif /* LIBPRETTY_H */
