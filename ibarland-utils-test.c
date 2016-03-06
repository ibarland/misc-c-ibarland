#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // for INT_MAX etc
#include "ibarland-utils.h"

int main() {

    testStr("abc","abc");
    testStr("","");
    testStr(NULL,NULL);

    char* stringOnHeap;
    stringOnHeap = intToString(0);             testStr(stringOnHeap, "0" );           free(stringOnHeap);
    stringOnHeap = intToString(1);             testStr(stringOnHeap, "1" );           free(stringOnHeap);
    stringOnHeap = intToString( 33);           testStr(stringOnHeap,  "33" );         free(stringOnHeap);
    stringOnHeap = intToString(-33);           testStr(stringOnHeap, "-33" );         free(stringOnHeap);
    stringOnHeap = intToString( 2147483647);   testStr(stringOnHeap, "2147483647" );  free(stringOnHeap);
    stringOnHeap = intToString(-2147483648);   testStr(stringOnHeap, "-2147483648" ); free(stringOnHeap);
    stringOnHeap = intToString( INT_MAX );     testStr(stringOnHeap, "2147483647" );  free(stringOnHeap);
    stringOnHeap = intToString( INT_MIN );     testStr(stringOnHeap, "-2147483648" ); free(stringOnHeap);
    // TODO: for INT_MAX, LONG_MIN, etc:  print them into a clearly-large-enough buffer, and then make sure the results match.
    
    stringOnHeap = longToString( 0L);          testStr(stringOnHeap, "0" );           free(stringOnHeap);
    stringOnHeap = longToString( 1L);          testStr(stringOnHeap, "1" );           free(stringOnHeap);
    stringOnHeap = longToString(-33L);         testStr(stringOnHeap, "-33" );         free(stringOnHeap);
    stringOnHeap = longToString( 33L);         testStr(stringOnHeap,  "33" );         free(stringOnHeap);
    stringOnHeap = longToString( 100L);        testStr(stringOnHeap, "100" );         free(stringOnHeap);
    stringOnHeap = longToString(-100L);        testStr(stringOnHeap, "-100" );        free(stringOnHeap);
    stringOnHeap = longToString( 2147483647L); testStr(stringOnHeap,  "2147483647" ); free(stringOnHeap);
    stringOnHeap = longToString(-2147483648L); testStr(stringOnHeap, "-2147483648" ); free(stringOnHeap);
    stringOnHeap = longToString( 1000000000L); testStr(stringOnHeap,  "1000000000" ); free(stringOnHeap);
    stringOnHeap = longToString(-1000000000L); testStr(stringOnHeap, "-1000000000" ); free(stringOnHeap);
    stringOnHeap = longToString( 3000000000L); testStr(stringOnHeap,  "3000000000" ); free(stringOnHeap);
    stringOnHeap = longToString(-3000000000L); testStr(stringOnHeap, "-3000000000" ); free(stringOnHeap);
    stringOnHeap = longToString( 9223372036854775807L); testStr(stringOnHeap,  "9223372036854775807" ); free(stringOnHeap);
    // ?? compiler-error?  I get a warning 'too big for long' if I try to use -9223372036854775808L, even though that ==LONG_MIN.
    stringOnHeap = longToString( LONG_MAX );   testStr(stringOnHeap,  "9223372036854775807" ); free(stringOnHeap);
    stringOnHeap = longToString( LONG_MIN );   testStr(stringOnHeap, "-9223372036854775808" ); free(stringOnHeap);

    
    testBool( time_usec() > 1451606400L*1000000L, true );  // > 2016 AD
    testBool( time_usec() < 3786912000L*1000000L, true );  // < 2090 AD
    printf("\nCurrent time in ms is %ld. (2016=%ld, 2017=%ld).\n", time_usec(), 1451606400L*1000000L, 1483228800L*1000000L );
    
    return 0;
    }
