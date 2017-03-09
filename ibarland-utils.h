/** ibarland-utils.h
 * Helper functions for common C tasks.
 * @author Ian Barland, ibarland@radford.edu
 * @version 2017-Mar-08
 *
 * Macros/typedefs provided:
 *   ALLOC
 *   ALLOC_ARRAY
 *   DPRINTF      (N.B. To enable debugging, `#define DEBUG` in a file BEFORE `#include`ing this .h.)
 *   SIZEOF_ARRAY (N.B. good only for local, stack-allocated arrays, not pointers)
 *   stringConst
 *   uint  // TODO: remove; use uint -- more C-ish
 *    byte
 *   ubyte
 *   uchar
 *   ushort
 *   uint
 *   ulong
 *   ulonglong

 *   MIN   // macros for computing the min,max
 *   MAX
 *   MINF  // handle nan w/o a type-error
 *   MAXF  
 *   swap
 *   
 * Functions/constants provided:
 *    sgn
 *    modPos
 *    lmodPos
 *    M_TAU     = 2*M_PI
 *    degToRad
 *    radToDeg
 *    isinfinite
 *    approxEquals
 * 
 *    time_usec
 *    intToString    (N.B. Caller must free the returned-string.)
 *    longToString   (N.B. Caller must free the returned-string.)
 *    newStrCat      (N.B. Caller must free the returned-string.)
 *
 *    streq
 *    strdiff
 *    strempty
 *
 *    arrB_toString
 *    arrC_toString
 *    arrI_toString
 *    arrF_toString
 *    arrLi_toString
 *    arrLf_toString
 *    
 *    testStr
 *    testChar
 *    testInt
 *    testBool
 *    printTestMsg
 *    printTestSummary
 *    resetTestSummary
 *
 *    pid_t forkAndExec( stringConst cmd );
 */


/* To use these functions:
 * As with any .h file:
 * In your program, include the .h file.  
 * And when compiling(linking) your program, add 'ibarland-utils.o' to the `gcc` line,
 *   e.g.     gcc -Wall myProg.c -o myProg   ibarland-utils.o
 * If that .o file doesn't exist, first create it with `gcc -Wall -c ibarland-utils.c`.
 *
 * Btw, to include the .h file, put the filename in quotes, not angle-brackets (which'd
 * indicate to search system-standard directories).  If the .h file isn't in the same
 * dir as the program you're compiling, see `gcc -I` for adding dirs to gcc's include-path.
 */

#ifndef IBARLAND_UTILS_H
#define IBARLAND_UTILS_H

#include <stdbool.h> // for testBool
#include <unistd.h> // for pid_t
#include <stdio.h>  // for fprintf

typedef const char * const stringConst;

typedef   signed char         byte;
typedef unsigned char        ubyte;
typedef unsigned char        uchar;
typedef unsigned short       ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;
typedef unsigned long long   ulonglong;


#define ALLOC(typ)               (typ *) (malloc(sizeof( typ )))
#define ALLOC_ARRAY(n, typ)      (typ *) (calloc((unsigned) n, sizeof( typ )))  // N.B. calloc init's the memory to 0.

#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))
/* CAUTION: `SIZEOF_ARRAY` works ONLY with arrays declared with a `[]` 
 * (NOT as a pointer, via malloc), AND when we're in the same scope where it was declared.
 * Otherwise we're seeing the array as a pointer, whose sizeof is 1word, regardless of array size).
 */



#define MIN(X,Y)  (((X) <= (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) >= (Y)) ? (X) : (Y))
#define MINF(X,Y)  (((X) <= (Y)) ? (X) : (isnan(X) ? NAN : Y))
#define MAXF(X,Y)  (((X) >= (Y)) ? (X) : (isnan(X) ? NAN : Y))

void swap_b (  bool  *a, bool   *b );
void swap_c (  char  *a, char   *b );
void swap_i (  int   *a, int    *b );
void swap_u (  uint  *a, uint   *b );
void swap_l (  long  *a, long   *b );
void swap_ul(  ulong *a, ulong  *b );
void swap_f ( float  *a, float  *b );
void swap_d ( double *a, double *b );

/* 'signum', the sign of a number (+1, 0, or -1). */
float sgn( long double const x );

/* modPos is like %, except that return val is in [0, b), not (-b, b) */
int   modPos( int const n,  int const b );
long lmodPos( long const n, long const b );

/* a-b, with a floor of 0.  Helpful for both signed & unsigned arithmetic. */
int monus( int a, int b );
uint monus_u( uint a, uint b );


extern double M_TAU;  // tau = 2*pi
double degToRad(double const theta);
double radToDeg(double const theta);
bool isinfinite( double x );
bool approxEquals(double const x, double const y);

// string-equal and string-different -- using `strcmp` in boolean expressions goofs me up otherwise.
bool streq( stringConst s1, stringConst s2 );
bool strdiff( stringConst s1, stringConst s2 );
// Note that we use the name 'strdiff' rather than 'strneq', since that's ambiguous with 'streq up to n chars'.
bool strempty( stringConst s );



/* Return a string representation of an array, arr[0]..arr[sz-1].
 * arr -- the beginning of the array
 * sz -- the number of elements of the array
 * open -- a string to start the array with.  If NULL, `"["` is used.
 * formatSpec -- the printf-style format string to use for a single element. 
 *               If NULL, use the appropriate default %i or %li or ... .
 * between -- a printf-compatible string for putting between each element (not after the last).  If NULL, `","` is used.
 * close -- a string to start the array with.  If NULL, `"]"` is used.
 *
 * The result will be a string of length: strlen(open) + (n-1)*strlen(interpserse)
 */
stringConst arrB_toString(  const bool* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );
stringConst arrC_toString(  const char* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );
stringConst arrI_toString(  const int* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );
stringConst arrF_toString(  const float* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );
stringConst arrLi_toString( const long int* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );
stringConst arrLf_toString( const double* const arr, const int sz,
                            stringConst open, stringConst formatSpec, stringConst between, stringConst close );


/** Return an array of `sz` ints (sz>0), uninitialized.
 * It is the responsibility of the caller to free this memory.
 */
int* newArrayI_uninit( uint sz );


/** Return an array of `sz` ints (sz>0), initialized to `val`. 
 * It is the responsibility of the caller to free this memory.
 */
int* newArrayI( uint sz, int val );

/** Return an array of `sz` ints (sz>0), initialized with random values in [lo,hi), where hi>lo.
 * It is the responsibility of the caller to free this memory.
 * Result depends on, and changes, the state of `random`.
 */
int* newArrayI_rand( uint sz, int lo, int hi );


/* Fill arr[0,sz) with `val`.  Returns `arr` as a convenience. */
int* fillArrayI( int* arr, uint sz, int val );


/* Fill arr[0,sz) with values random from [lo,hi).
 * Returns `arr`, as a convenience.
 * Result depends on, and changes, the state of `random`.
 */
int* fillArrayI_rand( int* arr, uint sz, int lo, int hi );

























/* Use 'DPRINTF' for debug printing: */
/* Taken from: http://stackoverflow.com/questions/1941307/c-debug-print-macros, by Tom K. */
/* If your C-compiler doesn't like this, try Aidan Cully's simpler version on same page. */

#ifdef DEBUG  // To enable debug, `#define DEBUG` BEFORE you #include this .h
  #define DPRINTF(fmt, args...) \
    fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args)
#else
  #define DPRINTF(fmt, args...) /* macro expands to: nothing! */
#endif



// A flag for whether successful test-cases should print a very-short indicator.
bool print_on_test_success;


/* Are two values the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testStr( stringConst actual, stringConst expected ); // actual==expected==null passes.
void testChar( char const actual, char const expected );
void testInt( int const actual, int const expected );
void testUInt( uint actual, uint expected );
void testLong( long const actual, long const expected );
void testDouble( double const actual, double const expected );
void testBool( bool const actual, bool const expected );

// Print any message to the error-log file:
#define printTestMsg( fmt, args ... ) fprintf(stdout, fmt, ##args)

// Print summary statistics of tests completed/passed.
void printTestSummary();
  

/* Return the #microseconds since the standard epoch. */
ulong time_usec();

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int const n );

/* Return a string numeral, for the given long.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* longToString( long const n );

/* Return a new string which is the two arguments concatenated.
 * strA, strB should both be non-null.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* newStrCat( stringConst strA, stringConst strB );


/* Fork and exec the indicated command; returns the fork'd child's ID.
 * No arguments are provided to the exec'd process.
 */
pid_t forkAndExec( stringConst cmd );

#endif
