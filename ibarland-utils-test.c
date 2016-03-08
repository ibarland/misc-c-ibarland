#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // for INT_MAX etc
#include <float.h>  // for DBL_MAX etc
#include <math.h>  // for M_PI
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
    
    testInt( sgn(0), 0 );
    testInt( sgn(-0), 0 );
    testInt( sgn(0.0), 0 );
    testInt( sgn(-0.0), 0 );
    testInt( sgn(0.01), 1 );
    testInt( sgn(1.0), 1 );
    testInt( sgn(30), 1 );
    testInt( sgn(DBL_MIN), 1 );
    testInt( sgn(DBL_MAX), 1 );


    testInt( modPos(0,3), 0 );
    testInt( modPos(3,3), 0 );
    testInt( modPos(6,3), 0 );
    testInt( modPos(8,3), 2 );
    testInt( modPos(INT_MAX,2), 1 );
    testInt( modPos(INT_MAX-1, 2), 0 );

    testInt( modPos(-3,3), 0 );
    testInt( modPos(-6,3), 0 );
    testInt( modPos(-8,3), 1 );
    testInt( modPos(INT_MIN, 2), 0 );
    testInt( modPos(INT_MIN+1, 2), 1 );

    testInt( modPos( 3,-3), 0 );
    testInt( modPos( 6,-3), 0 );
    testInt( modPos( 8,-3), -1 );
    testInt( modPos(INT_MAX, -2), -1 );
    testInt( modPos(INT_MAX-1, -2), 0 );

    testInt( modPos(-3,-3), 0 );
    testInt( modPos(-6,-3), 0 );
    testInt( modPos(-8,-3), -2 );
    testInt( modPos(INT_MIN, -2), 0 );
    testInt( modPos(INT_MIN+1, -2), -1 );

    testInt( lmodPos(0,3), 0 );
    testInt( lmodPos(3,3), 0 );
    testInt( lmodPos(6,3), 0 );
    testInt( lmodPos(8,3), 2 );
    testInt( lmodPos(INT_MAX,2), 1 );
    testInt( lmodPos(INT_MAX-1, 2), 0 );
    testInt( lmodPos(LONG_MAX,2), 1 );
    testInt( lmodPos(LONG_MAX-1, 2), 0 );

    testInt( lmodPos(-3,3), 0 );
    testInt( lmodPos(-6,3), 0 );
    testInt( lmodPos(-8,3), 1 );
    testInt( lmodPos(INT_MIN, 2), 0 );
    testInt( lmodPos(INT_MIN+1, 2), 1 );
    testInt( lmodPos(LONG_MIN, 2), 0 );
    testInt( lmodPos(LONG_MIN+1, 2), 1 );

    testInt( lmodPos( 3,-3), 0 );
    testInt( lmodPos( 6,-3), 0 );
    testInt( lmodPos( 8,-3), -1 );
    testInt( lmodPos(INT_MAX, -2), -1 );
    testInt( lmodPos(INT_MAX-1, -2), 0 );
    testInt( lmodPos(LONG_MAX, -2), -1 );
    testInt( lmodPos(LONG_MAX-1, -2), 0 );

    testInt( lmodPos(-3,-3), 0 );
    testInt( lmodPos(-6,-3), 0 );
    testInt( lmodPos(-8,-3), -2 );
    testInt( lmodPos(INT_MIN, -2), 0 );
    testInt( lmodPos(INT_MIN+1, -2), -1 );
    testInt( lmodPos(LONG_MIN, -2), 0 );
    testInt( lmodPos(LONG_MIN+1, -2), -1 );


   testDouble( degToRad(0), 0 );
   testDouble( degToRad(360), M_TAU );
   testDouble( degToRad(180), M_PI );
   testDouble( degToRad(30), M_PI/6 );

   testDouble( radToDeg(0), 0.0 );
   testDouble( radToDeg(M_TAU), 360.0 );
   testDouble( radToDeg(M_PI),  180.0 );
   testDouble( radToDeg(M_PI/6), 30.0 );
   
   testBool( approxEquals(2.0, 2.0), true );
   testBool( approxEquals(2.0, 3.0), false );
   testBool( approxEquals(2.0, 2.0e128), false );
   testBool( approxEquals(2.0e128, 2.0000001e128), true );
   testBool( approxEquals(2.0, 2.1), false );
   testBool( approxEquals(2.0, 2.000000001), true );

   testBool( approxEquals(NAN, NAN), false );

    
    return 0;
    }
