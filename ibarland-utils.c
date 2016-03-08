/* See .h file for general-info. */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h> // for strcmp
#include <sys/time.h>
#include "ibarland-utils.h"

// string-equal and string-different -- using `strcmp` in boolean expressions goofs me up otherwise.
bool streq( char* s1, char* s2 ) { return strcmp(s1,s2)==0; }
bool strdiff( char* s1, char* s2 ) { return !streq(s1,s2); }
// Note that we use the name 'strdiff' rather than 'strneq', since that's ambiguous with 'streq up to n chars'.
bool strempty( char* s ) { return streq(s,""); }

// A flag for whether successful test-cases should print a very-short indicator.
bool print_on_test_success = true;

int testCount = 0;
int testFailCount = 0;
const int TEST_INDICATOR_GROUP_SIZE = 5;

/* Are two strings the same (or, both null)?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testStr( char* actual, char* expected ) {
    ++testCount;
    if (actual==expected || (actual != NULL && expected != NULL && streq(actual,expected))) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "\n**test-fail:\nactual: \"%s\"\nexpect: \"%s\".\n", actual, expected);
        ++testFailCount;
        }
    }

/* Are two chars the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testChar( char actual, char expected ) {
    ++testCount;
    if (actual==expected) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail:\nactual: '%c'\nexpect: '%c'.\n", actual, expected);
        ++testFailCount;
        }
    }

/* Are two doubles the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testDouble( double actual, double expected ) {
    ++testCount;
    if (actual==expected) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail:\nactual: %lf\nexpect: %lf.\n", actual, expected);
        ++testFailCount;
        }
    }









/* Are two ints the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testInt( int actual, int expected ) {
    ++testCount;
    if (actual==expected) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail:\nactual: %i\nexpect: %i.\n", actual, expected);
        ++testFailCount;
        }
    }

/* Are two bools the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testBool( bool actual, bool expected ) {
    ++testCount;
    if ((actual&&expected) || (!actual && !expected)) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail (as bools):\nactual: %i\nexpect: %i.\n", actual, expected);
        ++testFailCount;
        }
    }


/* Return the #microseconds since the standard epoch. */
long time_usec() {
  struct timeval now;
  gettimeofday(&now,NULL);
  return now.tv_sec*1000000L + now.tv_usec;
  }

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int n ) {
  // We could just allocate enough space for 11-ish digits, which currently works, but only if sizeof(int) <= 4.
  // We could allocate enough space based on sizeof(int):   ((int)ceil(8*sizeof(int) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  int nSameDigits = (n==0) ? 1 : (abs(n) < 0 ? abs(n+1) : abs(n)); // A version of n that has the same #digits.
  int numDigits = (int)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%i", n );
  return nAsStr;
  }

/* Return a string numeral, for the given long.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* longToString( long n ) {
  // We could just allocate enough space for 20 digits, which currently works, but only if sizeof(long) <= 8.
  // We could allocate enough space based on sizeof(long):   ((int)ceil(8*sizeof(long) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  long nSameDigits = (n==0) ? 1 : (labs(n) < 0 ? labs(n+1) : labs(n)); // A version of n that has the same #digits.
  int numDigits = (int)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%ld", n );
  return nAsStr;
  }

/* 'signum', the sign of a number (+1, 0, or -1).
 * For a templated C++ verison, see: http://stackoverflow.com/a/4609795/320830
 * or use a macro:   #define SGN(x)  (x)>0 ? 1 : ((x)<0 ? -1 : 0)
 * This version relies on implicit casting up to a long double.
 */
int sgn( long double x ) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

double M_TAU = 2* M_PI; // hmm, misleading to name it "M_", since it's not actually in math.h?
double degToRad(double theta) { return theta/360 * M_TAU; }
double radToDeg(double theta) { return theta/M_TAU * 360; }


/* modPos is like %, except that return val is in [0, b), not (-b, b) */
int modPos( int n, int b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : (n%b + b);
  }
long lmodPos( long n, long b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : n%b + b;
  }

// Based on:  http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// Key observation: the bit-patterns for floats are lexicographic.
// So to compare floats, cast the bit-patterns to int and check that the difference is small!
// Only glitch is to handle two's-complement ... and NaNs.
//
bool approxEqualsUlps(float x, float y, int maxUlps) {
    if (isnan(x) || isnan(y)) return false;
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    // Treat the bit-patterns as ints:
    int xInt = *(int*)&x;
    int yInt = *(int*)&y;
    DPRINTF( "x,y as ints: %i,%i.\n", xInt, yInt );
    // Make lexicographically ordered as a twos-complement int
    if (xInt < 0) xInt = 0x80000000 - xInt;
    if (yInt < 0) yInt = 0x80000000 - yInt;
    DPRINTF( "x,y as complemented ints: %i,%i.\n", xInt, yInt );
    return (abs(xInt - yInt) <= maxUlps);
    }

// Are two doubles approximately-equal?  
// Tolerance is, crudely: within 1 in 10billion  (10^16 approx. double precision / 1_000_000)
bool approxEquals(float x, float y) { return approxEqualsUlps(x,y,1000); }
