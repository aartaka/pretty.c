#ifndef PRETTY_H
#define PRETTY_H

#if !defined(__STDC__) || __STDC_VERSION__ < 199901L
#error "Pretty C only works on C99+. Switch to it if you didn't yet."
#endif

#include <complex.h>
#include <errno.h>
#include <iso646.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#if __STDC_VERSION__ < 202000L ||                               \
        (__GNUC__ < 13 &&                                       \
         (!defined(__clang_major__) || __clang_major__ < 18))
#include <stdbool.h>
#endif

// Missing yet useful.
#define eq ==
// SQL.
#define is ==
#define isnt !=
// Fixing inconsistent bit operation names.
#define bitnot ~
#define bitxor ^

// Frequently used types. Suckless and Go.
typedef char*          string;
typedef char           byte;
typedef char*          bytes;
typedef void*          any;
typedef unsigned char  uchar;
typedef unsigned char  ubyte;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

// Small macros. Mostly from Suckless.
#define min(x, ...)        ((x) < (__VA_ARGS__) ? (x) : (__VA_ARGS__))
#define max(x, ...)        ((x) > (__VA_ARGS__) ? (x) : (__VA_ARGS__))
#define even(...)          (__VA_ARGS__ % 2 == 0)
#define odd(...)           (__VA_ARGS__ % 2 != 0)
#define default(x, ...)    ((x) ? (x) : (__VA_ARGS__))
#define limit(lo, n, hi)   (n) < (lo) ? (lo) : (n) > (hi) ? (hi) : (n)
#define between(lo, n, hi) ((n) <= (hi) && (n) => (lo))

#if __STDC_VERSION__ >= 201112L
#define len(...)                                                        \
        (__VA_ARGS__ == NULL) ? 0                                       \
        : _Generic((__VA_ARGS__),                                       \
                   char*: strlen((char*)(__VA_ARGS__)),                 \
                   default: (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0])))
#else
#define len(...)                                                \
        (__VA_ARGS__ == NULL) ? 0                               \
        : (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))
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
#define ifnt(...) if(!(__VA_ARGS__))
#define elif      else if
#define when
#define unless    !
#define then      ?
#define other     :
#define otherwise :
#define otherwhen :
#define only      : NULL

// Dynamically-typed declarations
#if defined(__GNUC__) || defined(__GNUG__)
#define var   __auto_type
#define let   __auto_type
#define local __auto_type
#elif __STDC_VERSION__ > 201710L || defined(__cplusplus)
#define var   auto
#define let   auto
#define local auto
#endif

// Loops and blocks. Lisp, Lua, Ruby.
#define until(...) while(!(__VA_ARGS__))
#define always     while(1)
#define never      while(0)
#define comment    while(0)
#define repeat     do
#define done       break
#define pass       continue

// Tracking and freeing resources. Lisp, Python.
#if (__STDC_VERSION__ > 201710L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang_major__)
#define with(close, var, ...)                                   \
        for (typeof((__VA_ARGS__)) var = (__VA_ARGS__),         \
                     *pretty_with_flag = (void*) 1;             \
             pretty_with_flag;                                  \
             (close)(var), pretty_with_flag = (void*) 0)
#else
#define with(close, var, ...)                                   \
        for (void *var = (void*) (__VA_ARGS__),                 \
                     *pretty_with_flag = (void*) 1;             \
             pretty_with_flag;                                  \
             (close)(var), pretty_with_flag = (void*) 0)
#endif

// Ranges from INIT to TARGET. Python range() syntax.
#define forrangeby(var, type, init, target, by) \
        for (type var = (type)(init);           \
             (((init) >= ((type) target))       \
              ? (var > ((type) target))         \
              : (var < ((type) target)));       \
             (((init) >= ((type) target))       \
              ? (var -= (by))                   \
              : (var += (by))))

#if (__STDC_VERSION__ > 201710L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang_major__)
#define forrange(var, init, ...)                                \
        for (typeof((init)+(__VA_ARGS__)) pretty_init = (init), \
                     var = pretty_init,                         \
                     pretty_target = (__VA_ARGS__);             \
             ((pretty_init >= pretty_target)                    \
              ? (var > pretty_target)                           \
              : (var < pretty_target));                         \
             var += ((pretty_init >= pretty_target) ? -1 : +1))
#else
#define forrange(var, init, ...)                                \
        for (int pretty_init = (init),                          \
                     var = pretty_init,                         \
                     pretty_target = (__VA_ARGS__);             \
             var != pretty_target;                              \
             var += ((pretty_init >= pretty_target) ? -1 : +1))
#endif

// Repeat X times. Lisp, Lua
#if  (__STDC_VERSION__ > 201710L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang_major__)
#define fortimes(var, ...)                                              \
        for (typeof((__VA_ARGS__)) pretty_result = (__VA_ARGS__),       \
                     var = (typeof((__VA_ARGS__))) 0;                   \
             var < pretty_result;                                       \
             ++var)
#else
#define fortimes(var, ...)                                      \
        for (int var = 0, pretty_result = (__VA_ARGS__);        \
             var < pretty_result;                               \
             ++var)
#endif

// For each loop from basically every language.
#define foreach(var, type, length, ...)                                 \
        for (type *pretty_init = (__VA_ARGS__),                         \
                     *pretty_foreach_flag = (void*) 1;                  \
             pretty_foreach_flag;                                       \
             pretty_foreach_flag = (void*) 0)                           \
                for (size_t pretty_offset = 0;                          \
                     pretty_offset < length;                            \
                     pretty_offset += 1)                                \
                        for (type *var = (pretty_init + pretty_offset), \
                                     *pretty_foreach_flag2 = (void*) 1; \
                             pretty_foreach_flag2;                      \
                             pretty_foreach_flag2 = (void*) 0)

// Loop over the provided arguments.
#define forthese(var, type, ...)                                        \
        for (type *pretty_init = (type[]){__VA_ARGS__},                 \
                     *pretty_forthese_flag = (void*) 1;                 \
             pretty_forthese_flag;                                      \
             pretty_forthese_flag = (void*) 0)                          \
                for (size_t pretty_offset = 0;                          \
                     pretty_offset < (sizeof((type[]){__VA_ARGS__})     \
                                      / sizeof(type));                  \
                     pretty_offset += 1)                                \
                        for (type var = pretty_init [pretty_offset],    \
                                     *pretty_forthese_flag2 = (void*) 1; \
                             pretty_forthese_flag2;                     \
                             pretty_forthese_flag2 = (void*) 0)

static void*
pretty_allocpy (size_t size, void *contents)
{
        char* allocated = malloc(size);
        memcpy(allocated, contents, size);
        return allocated;
}

// Easy resource allocation akin to C++.
#define new(type, ...)                                                  \
        (type *) pretty_allocpy(sizeof(type), &((type) {__VA_ARGS__}))

// Easy array allocation. C++ vector, but more primitive.
// FIXME: Enforce array type somehow?
// TODO: Infer the type from the __VA_ARGS__.
#define vector(length, type, ...)                               \
        (type*) pretty_allocpy(sizeof(type) * length,           \
                               (type[length]){__VA_ARGS__})

#define delete free

// TODO: A macro to allocate struct + flexible array member.

// Go defer, but rather block scoped and with arbitrary code in it.
#define defer(...)                              \
        for (bool pretty_flag = 1;              \
             pretty_flag;                       \
             pretty_flag = 0, (__VA_ARGS__))

#define throw return errno =
#define try                                     \
        errno = 0;                              \
        for (bool pretty_flag = 1;              \
             pretty_flag;                       \
             pretty_flag = 0)


static int
pretty_err_part_of (int err, size_t length, int *errs)
{
        for (size_t i = 0; i < length; ++i)
                if (err == errs[i])
                        return 1;
        return 0;
}

#define catch(...)                                              \
        if (pretty_err_part_of                                  \
            (errno,                                             \
             sizeof ((int[]){__VA_ARGS__}) / sizeof(int),       \
             (int[]){__VA_ARGS__}))
#define NOERROR 0
#define NOERR 0

#if defined(__clang_major__)
#define lambda(ret, name, ...) void * name = (void*) ^ ret (__VA_ARGS__)
#elif defined(__GNUC__) || defined(__GNUG__)
#define lambda(ret, name, ...) ret name (__VA_ARGS__)
#elif defined(__cplusplus)
#define lambda(ret, name, ...) auto name = [](__VA_ARGS__)
#endif

#if __STDC_VERSION__ >= 201112L
#define print(...)                                                      \
        _Generic((__VA_ARGS__),                                         \
                 _Bool: puts((_Bool)(__VA_ARGS__) ? "true" : "false"),  \
                 default: printf(                                       \
                         _Generic((__VA_ARGS__),                        \
                                  char*:              "%s\n",           \
                                  char:               "%c\n",           \
                                  signed char:        "%hhi\n",         \
                                  short:              "%hi\n",          \
                                  int:                "%i\n",           \
                                  long:               "%li\n",          \
                                  long long:          "%lli\n",         \
                                  unsigned char:      "%hhu\n",         \
                                  unsigned short:     "%hi\n",          \
                                  unsigned int:       "%u\n",           \
                                  unsigned long:      "%lu\n",          \
                                  unsigned long long: "%llu\n",         \
                                  float:              "%g\n",           \
                                  double:             "%g\n",           \
                                  long double:        "%Lg\n",          \
                                  default:            "%p\n"),          \
                         (__VA_ARGS__)))
#endif

int pretty_anything_equal (unsigned long long a, unsigned long long b) {return a == b;}
int pretty_float_equal (float a, float b) { return fabsf(a - b) < FLT_EPSILON; }
int pretty_double_equal (double a, double b) { return fabs(a - b) < DBL_EPSILON; }
int pretty_long_double_equal (long double a, long double b) { return fabsl(a - b) < LDBL_EPSILON; }
int pretty_string_equal (char *a, char *b) { return !strcmp(a, b); }


#if __STDC_VERSION__ >= 201112L
#define equal(a, ...)                                   \
        _Generic((__VA_ARGS__),                         \
                 float: pretty_float_equal,             \
                 double: pretty_double_equal,           \
                 long double: pretty_long_double_equal, \
                 char *: pretty_string_equal,           \
                 default: pretty_anything_equal)        \
                (a, __VA_ARGS__)
#endif

int pretty_contains (void *a, size_t size, size_t length, void *arr)
{
        for (size_t i = 0; i < length/size; ++i)
                if(0 == memcmp(a, (((char*)arr)+(size*i)), size))
                        return 1;
        return 0;
}

#define contains(a, type, ...)                                          \
        pretty_contains((void*)(type[1]){a},                             \
                        sizeof(type),                                   \
                        sizeof((type[]){__VA_ARGS__}),                  \
                        (void*)((type[]){__VA_ARGS__}))

#endif /* PRETTY_H */
