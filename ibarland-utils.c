/* See .h file for general-info. */

#include <stdlib.h>
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
    printTestMsg( "\nvvvvvvvvvvvv\n" );
    printTestMsg( "%5d tests run.\n", testCount );
    if (testFailCount==0) {
        printTestMsg( "      All passed!\n" );
        }
    else {
        printTestMsg( "%5d tests passed;\n%5d tests FAILED (%f%%)\n", testCount-testFailCount, testFailCount, (100.0*testFailCount/testCount) );
        }
    printTestMsg( "^^^^^^^^^^^^\n" );
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
    if (actual==expected) { printTestSuccess(); }
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

/* Are two natNums (unsigned ints) the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testNatNum( natNum const actual, natNum const expected ) {
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
long time_usec() {
  struct timeval now;
  gettimeofday(&now,NULL);
  return now.tv_sec*1000000L + now.tv_usec;
  }

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int const n ) {
  // We could just allocate enough space for 11-ish digits, which currently works, but only if sizeof(int) <= 4.
  // We could allocate enough space based on sizeof(int):   ((int)ceil(8*sizeof(int) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  int nSameDigits = (n==0) ? 1 : (abs(n) < 0 ? abs(n+1) : abs(n)); // A version of n that has the same #digits.
  natNum numDigits = (natNum)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
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
  natNum numDigits = (natNum)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%ld", n );
  return nAsStr;
  }

/* 'signum', the sign of a number (+1, 0, or -1).
 * For a templated C++ verison, see: http://stackoverflow.com/a/4609795/320830
 * or use a macro:   #define SGN(x)  (x)>0 ? 1 : ((x)<0 ? -1 : 0)
 * This version relies on implicit casting up to a long double.
 */
int sgn( long double const x ) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }

/* a-b, with a floor of 0.  Helpful for unsgiend arithmetic. */
// Should make this int,int -> natNum ?   ->int?
int monus( int const a, int const b ) { return a>=b ? a-b : 0; }
natNum monus_u( natNum const a, natNum const b ) { return a>=b ? a-b : 0; }

double M_TAU = 2* M_PI; // hmm, misleading to name it "M_", since it's not actually in math.h?
double degToRad(double const theta) { return theta/360 * M_TAU; }
double radToDeg(double const theta) { return theta/M_TAU * 360; }


/* modPos is like %, except that return val is in [0, b), not (-b, b) */
int modPos( int const n, int const b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : (n%b + b);
  }
long lmodPos( long const n, long const b ) {
  return (sgn(n%b) != -sgn(b)) ? n%b : n%b + b;
  }

// Based on:  http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
// Key observation: the bit-patterns for floats are lexicographic.
// So to compare floats, cast the bit-patterns to int and check that the difference is small!
// Only glitch is to handle two's-complement ... and NaNs.
//
bool approxEqualsUlps(double const x, double const y, natNum const maxUlps) {
    assert(sizeof(float)==sizeof(int));
    if (isnan(x) || isnan(y)) return false;
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    // Treat the bit-patterns as ints:
    int const xInt = *(const int* const)&x;
    int const yInt = *(const int* const)&y;
    DPRINTF( "x,y as ints: %i,%i.\n", xInt, yInt );
    // Make lexicographically ordered as a twos-complement int
    // TODO: should this be 0x80000000 - xInt ?
    int const xIntPos = abs(xInt);
    int const yIntPos = abs(yInt);
    DPRINTF( "x,y as complemented ints: %i,%i.\n", xIntPos, yIntPos );
    return (((unsigned int) abs(xIntPos - yIntPos)) <= maxUlps);
    }

// Are two doubles approximately-equal?  
// Tolerance is, crudely: within 1 in 1,000,000  (2^24 ulps / 10)
bool approxEquals(double const x, double const y) { return approxEqualsUlps(x,y,10u); }
//bool approxEqualsRel(double x, double y, double relativeTolerance) {
//  }
