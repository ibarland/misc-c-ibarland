/* See .h file for general-info. */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h> // for strcmp
#include <sys/time.h>
#include "ibarland-utils.h"

// string-equal and string-different -- using `strcmp` in boolean expressions goofs me up otherwise.
bool streq( const char* const s1, const char* const s2 ) { return strcmp(s1,s2)==0; }
bool strdiff( const char* const s1, const char* const s2 ) { return !streq(s1,s2); }
// Note that we use the name 'strdiff' rather than 'strneq', since that's ambiguous with 'streq up to n chars'.
bool strempty( const char* const s ) { return streq(s,""); }

// A flag for whether successful test-cases should print a very-short indicator.
bool print_on_test_success = true;

int testCount = 0;
int testFailCount = 0;
const int TEST_INDICATOR_GROUP_SIZE = 5;

void resetTestSummary() {
    testCount = 0;
    testFailCount = 0;
    printTestMsg("\n");
    }


stringConst FAIL_HEADER = "\n***TEST FAILED***\n";


void printTestSuccess() {
    if (print_on_test_success) printTestMsg(".");
    if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) printTestMsg(" ");
    }

#define printTestFailure( act, exp, typeFormat, surrounder ) \
    ++testFailCount; \
    printTestMsg( "%s" "actual: " surrounder typeFormat surrounder "\nexpect: " surrounder typeFormat surrounder "\n", FAIL_HEADER, act, exp )
    // Use a macro so that we don't have to repeat for each type.

void printTestSummary() {
    printTestMsg( "\n" );
    printTestMsg( "vvvvvvvvvvvvvvvvvvv\n" );
    printTestMsg( "%5d tests run.\n", testCount );
    if (testFailCount==0) { printTestMsg( "      All passed!\n" ); }
    else { printTestMsg( "%5d tests passed;\n%5d tests FAILED (%f%%)\n", testCount-testFailCount, testFailCount, (100.0*testFailCount/testCount) ); }
    printTestMsg( "^^^^^^^^^^^^^^^^^^^\n" );
    }

/* Are two strings the same (or, both null)?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testStr( stringConst actual, stringConst expected ) {
    ++testCount;
    if (actual==expected || (actual != NULL && expected != NULL && streq(actual,expected))) {
        printTestSuccess();
        }
    else { printTestFailure(actual, expected, "%s", "\""); }
    }

/* Are two chars the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testChar( char const actual, char const expected ) {
    ++testCount;
    if (actual==expected) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%c", "'"); }
    }

/* Are two doubles the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testDouble( double const actual, double const expected ) {
    ++testCount;
    if (approxEquals(actual,expected) || (isnan(actual) && isnan(expected))) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%lf", ""); }
    }


/* Are two ints the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testInt( int const actual, int const expected ) {
    ++testCount;
    if (actual==expected) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%i", ""); }
    }

/* Are two nats (unsigned ints) the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testUInt( uint const actual, uint const expected ) {
    ++testCount;
    if (actual==expected) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%u", ""); }
    }


/* Are two longs the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testLong( long const actual, long const expected ) {
    ++testCount;
    if (actual==expected) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%li", ""); }
    }

/* Are two bools the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testBool( bool const actual, bool const expected ) {
    ++testCount;
    if (actual==expected) { printTestSuccess(); }
    else { printTestFailure(actual, expected, "%i", ""); }
    }


/* Return the #microseconds since the standard epoch. */
ulong time_usec() {
  struct timeval now;
  gettimeofday(&now,NULL);
  return (ulong) (now.tv_sec*1000000L + now.tv_usec);
  }

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int const n ) {
  // We could just allocate enough space for 11-ish digits, which currently works, but only if sizeof(int) <= 4.
  // We could allocate enough space based on sizeof(int):   ((int)ceil(8*sizeof(int) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  int nSameDigits = (n==0) ? 1 : (abs(n) < 0 ? abs(n+1) : abs(n)); // A version of n that has the same #digits.
  uint numDigits = (uint)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%i", n );
  return nAsStr;
  }

/* Return a string numeral, for the given long.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* longToString( long const n ) {
  // We could just allocate enough space for 20 digits, which currently works, but only if sizeof(long) <= 8.
  // We could allocate enough space based on sizeof(long):   ((int)ceil(8*sizeof(long) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  long nSameDigits = (n==0) ? 1 : (labs(n) < 0 ? labs(n+1) : labs(n)); // A version of n that has the same #digits.
  uint numDigits = (uint)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%ld", n );
  return nAsStr;
  }

/* Return a new string which is the two arguments concatenated.
 * strA, strB should both be non-null.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* newStrCat( stringConst strA, stringConst strB ) {
  if (strA==NULL && strB==NULL) { return NULL; }
  else if (strA==NULL) { return strB; }
  else if (strB==NULL) { return strA; }
  else {
    char* rslt = (char*) malloc( strlen(strA) + strlen(strB) + 1 );
    rslt[0] = '\0';
    strcat(rslt,strA);
    strcat(rslt,strB);
    return rslt;
    }
  }



/* 'signum', the sign of a number (+1, 0, or -1).
 * For a templated C++ verison, see: http://stackoverflow.com/a/4609795/320830
 * or use a macro:   #define SGN(x)  (x)>0 ? 1 : ((x)<0 ? -1 : 0)
 * This version relies on implicit casting up to a long double.
 */
float sgn( long double const x ) { return isnan(x) ? NAN : ((x > 0) ? 1.0 : ((x < 0) ? -1.0 : 0.0)); }

/* a-b, with a floor of 0.  Helpful for unsgiend arithmetic. */
// Should make this int,int -> uint ?   ->int?
int monus( int const a, int const b ) { return a>=b ? a-b : 0; }
uint monus_u( uint const a, uint const b ) { return a>=b ? a-b : 0; }

double M_TAU = 2* M_PI; // hmm, misleading to name it "M_", since it's not actually in math.h?
double degToRad(double const theta) { return theta/360 * M_TAU; }
double radToDeg(double const theta) { return theta/M_TAU * 360; }


/* modPos is like %, except that return val is in [0, b), not (-b, b) */
int modPos( int const n, int const b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : (n%b + b);
  }
long lmodPos( long int const n, long int const b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : n%b + b;
  }

bool isinfinite( double x ) {
    return !isfinite(x) && !isnan(x);
    }

// Based on:  http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// Key observation: the bit-patterns for floats are lexicographic.
// So to compare floats, cast the bit-patterns to int and check that the difference is small!
// Only glitch is to handle two's-complement ... and NaNs.
//
// Tolerance is, crudely: within 1 in 1,000,000  (2^24 ulps / 10)
//
bool approxEqualsUlps(double const x, double const y, ulong const maxUlps) {
    assert(sizeof(double)==sizeof(long int));
    if (isnan(x) || isnan(y)) return false;
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    // Treat the bit-patterns as ints:
    long int const xLong = *(const long int* const)&x;
    long int const yLong = *(const long int* const)&y;
    DPRINTF( "x,y as ints: %i,%i.\n", xInt, yInt );
    // Make lexicographically ordered as a twos-complement int
    // TODO: should this be 0x80000000 - xInt ?
    long int const xLongPos = labs(xLong);
    long int const yLongPos = labs(yLong);
    DPRINTF( "x,y as complemented ints: %i,%i.\n", xIntPos, yIntPos );
    return (((ulong) labs(xLongPos - yLongPos)) <= maxUlps);
    }

// Are two doubles approximately-equal?  
bool approxEqualsRel(double x, double y, double relativeTolerance, double absoluteTolerance) {
    if (isnan(x) || isnan(y)) return false;
    if (isinfinite(x) && isinfinite(y)) return (sgn(x)==sgn(y));
    if (isinfinite(x) || isinfinite(y)) return false;
    if (fabs(y-x) <= absoluteTolerance) return true;
    double const size = MAXF(fabs(x),fabs(y));
    return fabs(x-y) <= size*relativeTolerance;
    }

bool approxEquals(double const x, double const y) { 
    return approxEqualsRel(x,y,0.00001, 1e-9);
    }


pid_t forkAndExec( stringConst cmd ) {
    pid_t proc;

    proc = fork();
    if (proc==0) { 
        execl( cmd, cmd, NULL); 
        fprintf( stdout, "%s: ", cmd );
        perror("exec'ing failed"); 
        exit(errno);
        }
    else if (proc < 1) {
        fprintf(stderr, "forking %s failed.", cmd );
        /* This function should really be provided a cleanup-callback, if doing this properly. */
        exit(proc);
        }
    return proc;
    }




/* Return a string representation of an array, arr[0]..arr[sz-1].
 * arr -- the beginning of the array
 * sz -- the number of elements of the array
 * open -- a string to start the array with.  If NULL, `"["` is used.
 * formatSpec -- the printf-style format string to use for a single element.
 * between -- a printf-compatible string for putting between each element (not after the last).  If NULL, `","` is used.
 * close -- a string to start the array with.  If NULL, `"]"` is used.
 *
 * The result will be a string of length: strlen(open) + (n-1)*strlen(between) + strlen(close)
 *   plus the characters needed for the actual data (which depends on the data and the formatSpec).
 * BUG: If an individual element needs more then MAX_ELT_LEN characters, it gets truncated.
 */
#define MAX_ELT_LEN  1024
static char* _nextElt = NULL;

#define MAKE_SPRINTF_ARR_FUNC_BODY(typ,defaultFormatSpec)\
( typ* arr, int sz, \
  stringConst _open, stringConst _formatSpec, stringConst _between, stringConst _close ) { \
    if (_nextElt == NULL) _nextElt = (char*)malloc(MAX_ELT_LEN);  /* one-time static init */ \
    stringConst open       = (_open      ==NULL  ?  "["   :  _open      ); \
    stringConst formatSpec = (_formatSpec==NULL  ?  defaultFormatSpec  :  _formatSpec); \
    stringConst between    = (_between   ==NULL  ?  ","   :  _between   ); \
    stringConst close      = (_close     ==NULL  ?  "]"   :  _close     ); \
 \
    char const * ssf = (open==NULL  ?  "["  :  open); \
    for (int i=0;  i<sz;  ++i) { \
        snprintf(_nextElt, MAX_ELT_LEN, formatSpec, arr[i]); \
        ssf = newStrCat(ssf,_nextElt); \
        if (i+1 != sz) ssf = newStrCat(ssf, (between==NULL  ?  ","  :  between)); \
        } \
    ssf = newStrCat(ssf, (close==NULL  ?  "]"  :  close)); \
    return ssf; \
    }

stringConst sprintf_arrb  MAKE_SPRINTF_ARR_FUNC_BODY(bool,"%i")
stringConst sprintf_arrc  MAKE_SPRINTF_ARR_FUNC_BODY(char,"%c")
stringConst sprintf_arri  MAKE_SPRINTF_ARR_FUNC_BODY(int,"%i")
stringConst sprintf_arrf  MAKE_SPRINTF_ARR_FUNC_BODY(float,"%f")
stringConst sprintf_arrli MAKE_SPRINTF_ARR_FUNC_BODY(long int,"%li")
stringConst sprintf_arrlf MAKE_SPRINTF_ARR_FUNC_BODY(double,"%lf")

/* N.B. I tried making a generic verion of the above, passing in
 * sizeOfElement -- the sizeof a single array-element (e.g., pass in sizeof(long))
 * but this doesn't work because when dereferencing, 'void*' isn't allowed (nor would
 * it work with sizeofElt < sizeof(void*)), so I cast arr to a char*.
 * But then when printf is given a value, it used only the first byte.
 */

#define SWAP_BODY(typ)\
( typ *a, typ *b ) { \
    typ tmp = *a; \
    *a = *b; \
    *b = tmp; \
    }

void swap_b SWAP_BODY(bool)
void swap_c SWAP_BODY(char)
void swap_i SWAP_BODY(int)
void swap_u SWAP_BODY(uint)
void swap_l SWAP_BODY(long)
void swap_ul SWAP_BODY(ulong)
void swap_f SWAP_BODY(float)
void swap_d SWAP_BODY(double)
