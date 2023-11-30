#ifndef PRETTY_H
#define PRETTY_H

#if __STDC_VERSION__ < 199901L
#error "Pretty C only works on C99+. Switch to it if you didn't yet."
#endif

#include <complex.h>
#include <errno.h>
#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

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
typedef unsigned char  ubyte;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

// Small macros. Mostly from Suckless.
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#if __STDC_VERSION__ >= 201112L
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
#define unless    !
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
#endif

// Loops and blocks. Lisp, Lua, Ruby.
#define until(...) while(!(__VA_ARGS__))
#define always     while(1)
#define never      while(0)
#define repeat     do

// Tracking and freeing resources. Lisp, Python.
#if (__STDC_VERSION__ >= 202311L || defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#define with(close, var, ...)                                   \
        for (typeof((__VA_ARGS__)) var = (__VA_ARGS__),         \
                     *flag_ ## __LINE__ = (void*) 1;            \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (void*) 0)
#else
#define with(close, var, ...)                                   \
        for (void *var = (void*) (__VA_ARGS__),                 \
                     *flag_ ## __LINE__ = (void*) 1;            \
             flag_ ## __LINE__;                                 \
             (close)(var), flag_ ## __LINE__ = (void*) 0)
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
                     *flag_ ## __LINE__ = (void*) 1;                    \
             flag_ ## __LINE__;                                         \
             flag_ ## __LINE__ = (void*) 0)                             \
                for (size_t offset ## __LINE__ = 0;                     \
                     offset ## __LINE__ < length;                       \
                     offset ## __LINE__ += 1)                           \
                        for (type *var = (init ## __LINE__ + offset ## __LINE__), \
                                     *flag2_ ## __LINE__ = (void*) 1;   \
                             flag2_ ## __LINE__;                        \
                             flag2_ ## __LINE__ = (void*) 0)

// Loop over the provided arguments.
#define forthese(var, type, ...)                                        \
        for (type *init ## __LINE__ = (type[]){__VA_ARGS__},            \
                     *flag_ ## __LINE__ = (void*) 1;                    \
             flag_ ## __LINE__;                                         \
             flag_ ## __LINE__ = (void*) 0)                             \
                for (size_t offset ## __LINE__ = 0;                     \
                     offset ## __LINE__ < (sizeof((type[]){__VA_ARGS__}) \
                                           / sizeof(type));             \
                     offset ## __LINE__ += 1)                           \
                        for (type var = init ## __LINE__ [offset ## __LINE__], \
                                     *flag2_ ## __LINE__ = (void*) 1;   \
                             flag2_ ## __LINE__;                        \
                             flag2_ ## __LINE__ = (void*) 0)

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
#define vector(length, type, ...)                               \
        (type*) pretty_allocpy(sizeof(type) * length,           \
                               (type[length]){__VA_ARGS__})

// TODO: A macro to allocate struct + flexible array member.

// Go defer, but rather block scoped and with arbitrary code in it.
#define defer(...)                                      \
        for (bool flag_ ## __LINE__ = 1;                \
             flag_ ## __LINE__;                         \
             flag_ ## __LINE__ = 0, (__VA_ARGS__))

#define try                                     \
        errno = 0;                              \
        for (bool flag_ ## __LINE__ = 1;        \
             flag_ ## __LINE__;                 \
             flag_ ## __LINE__ = 0)


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

#if defined(__clang__)
#define lambda(ret, name, ...) void * name = (void*) ^ ret (__VA_ARGS__)
#elif defined(__GNUC__) || defined(__GNUG__)
#define lambda(ret, name, ...) ret name (__VA_ARGS__)
#elif defined(__cplusplus)
#define lambda(ret, name, ...) auto name = [](__VA_ARGS__)
#endif

union float_uint {
  uintmax_t i;
  long double f;
};

static char *
pretty_tostring (char *format, union float_uint thing)
{
        if (!strcmp("%s", format))
                return (char *) thing.i;
        char *buffer = (char *) malloc(sizeof(char) * 1000);
        size_t written
          = ((!strcmp("%Lg", format))
             ? snprintf(buffer, 1000, format, thing.f)
             : snprintf(buffer, 1000, format, (void*) thing.i));
        return (char *) realloc(buffer, (written + 1) * sizeof(char));
}

#if __STDC_VERSION__ >= 201112L
#define tostring(...)                                           \
        _Generic((__VA_ARGS__),                                 \
                 _Bool: (__VA_ARGS__ ? "true" : "false"),       \
                 default: pretty_tostring(                      \
                         _Generic((__VA_ARGS__),                \
                                  char:               "%c",     \
                                  char*:              "%s",     \
                                  signed char:        "%hhi",   \
                                  short:              "%hi",    \
                                  int:                "%i",     \
                                  long:               "%li",    \
                                  long long:          "%lli",   \
                                  unsigned char:      "%hhu",   \
                                  unsigned short:     "%hi",    \
                                  unsigned int:       "%u",     \
                                  unsigned long:      "%lu",    \
                                  unsigned long long: "%llu",   \
                                  float:              "%Lg",    \
                                  double:             "%Lg",    \
                                  long double:        "%Lg",    \
                                  default:            "%p"),    \
                         (union float_uint) \
                         _Generic((__VA_ARGS__),                        \
                                  float: (long double) (__VA_ARGS__),   \
                                  double: (long double) (__VA_ARGS__),  \
                                  long double: (long double) (__VA_ARGS__), \
                                  default: (uintmax_t) (__VA_ARGS__))))
#endif

#if __STDC_VERSION__ >= 201112L
#define print(...)                                              \
        _Generic((__VA_ARGS__),                                 \
                 _Bool: puts((__VA_ARGS__) ? "true" : "false"), \
                 default: printf(                               \
                         _Generic((__VA_ARGS__),                \
                                  char*:              "%s\n",   \
                                  char:               "%c\n",   \
                                  signed char:        "%hhi\n", \
                                  short:              "%hi\n",  \
                                  int:                "%i\n",   \
                                  long:               "%li\n",  \
                                  long long:          "%lli\n", \
                                  unsigned char:      "%hhu\n", \
                                  unsigned short:     "%hi\n",  \
                                  unsigned int:       "%u\n",   \
                                  unsigned long:      "%lu\n",  \
                                  unsigned long long: "%llu\n", \
                                  float:              "%g\n",   \
                                  double:             "%g\n",   \
                                  long double:        "%Lg\n",  \
                                  default:            "%p\n"),  \
                         (__VA_ARGS__)))
#endif

static int
pretty_fltcmp (union float_uint this, union float_uint other, long double epsilon)
{
  return fabsl(this.f - other.f) < epsilon;
}

static int
pretty_strcmp (union float_uint this, union float_uint other, long double epsilon)
{
  return !strcmp((char*) this.i, (char*) other.i);
}

static int
pretty_cmp (union float_uint this, union float_uint other, long double epsilon)
{
  return this.i == other.i;
}


#if __STDC_VERSION__ >= 201112L
#define equal(val, ...)                                 \
  _Generic((val),                                       \
           float: pretty_fltcmp,                        \
           double: pretty_fltcmp,                       \
           long double: pretty_fltcmp,                  \
           char *: pretty_strcmp,                       \
           default: pretty_cmp)                         \
    ((union float_uint)                                 \
     _Generic((val),                                    \
              float: (long double) (val),               \
              double: (long double) (val),              \
              long double: (long double) (val),         \
              default: (uintmax_t) (val)),              \
     (union float_uint)                                 \
     _Generic((val),                                    \
              float: (long double) (__VA_ARGS__),       \
              double: (long double) (__VA_ARGS__),      \
              long double: (long double) (__VA_ARGS__), \
              default: (uintmax_t) (__VA_ARGS__)),      \
     _Generic((val),                                    \
              float: FLT_EPSILON,                       \
              double: DBL_EPSILON,                      \
              long double: DBL_EPSILON,                 \
              char *: 0.0,                                \
              default: 0.0))
#endif

#endif /* PRETTY_H */
