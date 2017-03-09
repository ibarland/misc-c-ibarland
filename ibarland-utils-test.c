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
    testDouble(NAN,NAN);
    testInt(2+2,4);
    testInt(-2-2,-4);
    testInt(INT_MAX,INT_MAX);
    testInt(INT_MAX+1, INT_MIN);

    testUInt(2+2,4);
    testUInt(UINT_MAX,UINT_MAX);
    testUInt(UINT_MAX+1,0);

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
    stringOnHeap = uintToString(0);             testStr(stringOnHeap, "0" );           free(stringOnHeap);
    stringOnHeap = uintToString(1);             testStr(stringOnHeap, "1" );           free(stringOnHeap);
    stringOnHeap = uintToString( 33);           testStr(stringOnHeap,  "33" );         free(stringOnHeap);
    stringOnHeap = uintToString( 2147483647);   testStr(stringOnHeap, "2147483647" );  free(stringOnHeap);
    stringOnHeap = uintToString( 2147483648);   testStr(stringOnHeap, "2147483648" );  free(stringOnHeap);
    stringOnHeap = uintToString( UINT_MAX );    testStr(stringOnHeap, "4294967295" );  free(stringOnHeap);
    
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
    //   Ah -- I think it has to do with the fact that C doesn't have negative literals; 
    //   it takes the positive and applies unary-minus, which may trigger implicit conversions (!) 
    stringOnHeap = longToString( LONG_MAX );   testStr(stringOnHeap,  "9223372036854775807" ); free(stringOnHeap);
    stringOnHeap = longToString( LONG_MIN );   testStr(stringOnHeap, "-9223372036854775808" ); free(stringOnHeap);
    
    
    stringOnHeap = newStrCat("","");           testStr(stringOnHeap, "" );               free(stringOnHeap);
    stringOnHeap = newStrCat("abc","");        testStr(stringOnHeap, "abc" );            free(stringOnHeap);
    stringOnHeap = newStrCat("","def");        testStr(stringOnHeap, "def" );            free(stringOnHeap);
    stringOnHeap = newStrCat("abc","def");     testStr(stringOnHeap, "abcdef" );         free(stringOnHeap);

    
    testBool( time_usec() > 1451606400L*1000000L, true );  // > 2016 AD
    testBool( time_usec() < 3786912000L*1000000L, true );  // < 2090 AD
    printTestMsg("\nCurrent time in ms is %ld. (2016=%ld, 2017=%ld).\n", time_usec(), 1451606400L*1000000L, 1483228800L*1000000L );
    
    testInt( strtoi_or_die(  "0", "test-pass-a"),   0 );
    testInt( strtoi_or_die( "23", "test-pass-b"),  23 );
    testInt( strtoi_or_die( "    23 ", "test-pass-b"),  23 );
    testInt( strtoi_or_die("+23", "test-pass-c"), +23 );
    testInt( strtoi_or_die("-23", "test-pass-d"), -23 );
    testInt( strtoi_or_die( "2000000000", "test-pass-e"), +2000000000 );
    testInt( strtoi_or_die("-2000000000", "test-pass-f"), -2000000000 );
    testInt( strtoi_or_die(intToString(INT_MAX), "test-pass-g"), INT_MAX);
    testInt( strtoi_or_die(intToString(INT_MIN), "test-pass-h"), INT_MIN);
    
    //testInt( strtoi_or_die( "", "oops!a" ), 0 );  // will exit program
    //testInt( strtoi_or_die( "hi", "oops!b" ), 0 );  // will exit program
    testInt( strtoi_or_die( "3hi", "oops!c" ), 3 );  // HEY -- this passes!  TODO: use the 2nd arg to strtol
    //testInt( strtoi_or_die( "3000000000", "oops!d" ), 0 );  // will exit program
    //testInt( strtoi_or_die( "-3000000000", "oops!e" ), 0 );  // will exit program
    
    testUInt( strtou_or_die(  "0", "test-pass-a"),   0 );
    testUInt( strtou_or_die( "23", "test-pass-b"),  23 );
    testUInt( strtou_or_die( "    23 ", "test-pass-b"),  23 );
    testUInt( strtou_or_die("+23", "test-pass-c"), +23 );
    testUInt( strtou_or_die( "2000000000", "test-pass-e"), +2000000000 );
    testUInt( strtou_or_die( "3000000000", "test-pass-f" ), +3000000000 );
    testUInt( strtou_or_die(uintToString(UINT_MAX), "test-pass-g"), UINT_MAX);
    
    //testUInt( strtou_or_die( "", "oops!a" ), 0 );  // will exit program
    //testUInt( strtou_or_die( "hi", "oops!b" ), 0 );  // will exit program
    testUInt( strtou_or_die( "3hi", "oops!c" ), 3 );  // HEY -- this passes!  TODO: use the 2nd arg to strtol
    //testUInt( strtou_or_die( "7000000000", "oops!d" ), 0 );  // will exit program
    //testUInt( strtou_or_die( "-3000000000", "oops!e" ), 0 );  // will exit program
    
    
    printTestMsg("\nTesting sgn, monus, mod: ");
    testDouble( sgn(0), 0 );
    testDouble( sgn(-0), 0 );
    testDouble( sgn(0.0), 0 );
    testDouble( sgn(-0.0), 0 );
    testDouble( sgn(0.01), 1 );
    testDouble( sgn(1.0), 1 );
    testDouble( sgn(30), 1 );
    testDouble( sgn(DBL_MIN), 1 );
    testDouble( sgn(DBL_MAX), 1 );

    testDouble( sgn(INFINITY), 1 );
    testDouble( sgn(-INFINITY), -1 );
    testDouble( sgn(-1.0/INFINITY),  0.0 );
    testDouble( sgn(-1.0/INFINITY), -0.0 );
    testDouble( sgn(NAN), NAN );
    
    testInt( monus(7,3), 4 );
    testInt( monus(3,7), 0 );
    testInt( monus(7,7), 0 );
    testInt( monus(0,0), 0 );
    testInt( monus(0,1), 0 );
    testInt( monus(1,0), 1 );
    testInt( monus(1,1), 0 );

    testUInt( monus_u(7u,3u), 4u );
    testUInt( monus_u(3u,7u), 0u );
    testUInt( monus_u(7u,7u), 0u );
    testUInt( monus_u(0u,0u), 0u );
    testUInt( monus_u(0u,1u), 0u );
    testUInt( monus_u(1u,0u), 1u );
    testUInt( monus_u(1u,1u), 0u );


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

    testLong( lmodPos(0,3), 0 );
    testLong( lmodPos(3,3), 0 );
    testLong( lmodPos(6,3), 0 );
    testLong( lmodPos(8,3), 2 );
    testLong( lmodPos(INT_MAX,2), 1 );
    testLong( lmodPos(INT_MAX-1, 2), 0 );
    testLong( lmodPos(LONG_MAX,2), 1 );
    testLong( lmodPos(LONG_MAX-1, 2), 0 );

    testLong( lmodPos(-3,3), 0 );
    testLong( lmodPos(-6,3), 0 );
    testLong( lmodPos(-8,3), 1 );
    testLong( lmodPos(INT_MIN, 2), 0 );
    testLong( lmodPos(INT_MIN+1, 2), 1 );
    testLong( lmodPos(LONG_MIN, 2), 0 );
    testLong( lmodPos(LONG_MIN+1, 2), 1 );

    testLong( lmodPos( 3,-3), 0 );
    testLong( lmodPos( 6,-3), 0 );
    testLong( lmodPos( 8,-3), -1 );
    testLong( lmodPos(INT_MAX, -2), -1 );
    testLong( lmodPos(INT_MAX-1, -2), 0 );
    testLong( lmodPos(LONG_MAX, -2), -1 );
    testLong( lmodPos(LONG_MAX-1, -2), 0 );

    testLong( lmodPos(-3,-3), 0 );
    testLong( lmodPos(-6,-3), 0 );
    testLong( lmodPos(-8,-3), -2 );
    testLong( lmodPos(INT_MIN, -2), 0 );
    testLong( lmodPos(INT_MIN+1, -2), -1 );
    testLong( lmodPos(LONG_MIN, -2), 0 );
    testLong( lmodPos(LONG_MIN+1, -2), -1 );
    
    printTestMsg( "\nTesting isinfinities, max, min: " );
    testBool( isinfinite(INFINITY), true );
    testBool( isinfinite(-INFINITY), true );
    testBool( isinfinite(NAN), false );
    testBool( isinfinite(LONG_MAX), false );
    
    testInt( MAX(2,3), 3 );
    testInt( MAX(3,2), 3 );
    testInt( MAX(3,3), 3 );
    testInt( MAX(-3,-2), -2 );
    testInt( MAX(INT_MAX, 3), INT_MAX );
    testInt( MAX(INT_MIN, 3), 3 );

    testInt( MIN(2,3), 2 );
    testInt( MIN(3,2), 2 );
    testInt( MIN(3,3), 3 );
    testInt( MIN(-3,-2), -3 );
    testInt( MIN(INT_MAX, 3), 3 );
    testInt( MIN(INT_MIN, 3), INT_MIN );
    
    testDouble( MAXF( INFINITY, 7), INFINITY );
    testDouble( MAXF(-INFINITY, 7), 7 );
    testDouble( MAXF( INFINITY,  INFINITY),  INFINITY );
    testDouble( MAXF( INFINITY, -INFINITY),  INFINITY );
    testDouble( MAXF(-INFINITY,  INFINITY),  INFINITY );
    testDouble( MAXF(-INFINITY, -INFINITY), -INFINITY );

    testDouble( MINF( INFINITY, 7), 7 );
    testDouble( MINF(-INFINITY, 7), -INFINITY );
    testDouble( MINF( INFINITY,  INFINITY),  INFINITY );
    testDouble( MINF( INFINITY, -INFINITY), -INFINITY );
    testDouble( MINF(-INFINITY,  INFINITY), -INFINITY );
    testDouble( MINF(-INFINITY, -INFINITY), -INFINITY );

    testDouble( MINF(NAN, 7.0 ), NAN );
    testDouble( MINF(7.0, NAN ), NAN );
    testDouble( MINF(NAN, NAN ), NAN );
    testDouble( MINF(-INFINITY, NAN ), NAN );
    testDouble( MINF( INFINITY, NAN ), NAN );
    testDouble( MINF(NAN, -INFINITY ) , NAN );
    testDouble( MINF(NAN,  INFINITY ) , NAN );

    testDouble( MAXF(NAN, 7.0 ), NAN );
    testDouble( MAXF(7.0, NAN ), NAN );
    testDouble( MAXF(NAN, NAN ), NAN );
    testDouble( MAXF(-INFINITY, NAN ), NAN );
    testDouble( MAXF( INFINITY, NAN ), NAN );
    testDouble( MAXF(NAN, -INFINITY ) , NAN );
    testDouble( MAXF(NAN,  INFINITY ) , NAN );




    printTestMsg("\nTesting degToRad: ");
    testDouble( degToRad(0), 0 );
    testDouble( degToRad(360), M_TAU );
    testDouble( degToRad(180), M_PI );
    testDouble( degToRad(30), M_PI/6 );

    testDouble( radToDeg(0), 0.0 );
    testDouble( radToDeg(M_TAU), 360.0 );
    testDouble( radToDeg(M_PI),  180.0 );
    testDouble( radToDeg(M_PI/6), 30.0 );
   
    printTestMsg("\nTesting approxEquals: ");
    testBool( approxEquals(2.0, 2.0), true );
    testBool( approxEquals(2.0, 3.0), false );
    testBool( approxEquals( 2.0,  2.000000001), true );
    testBool( approxEquals(-2.0, -2.000000001), true );
    testBool( approxEquals(2.0, 2.0e128), false );
    testBool( approxEquals(2.0e128, 2.000000001e128), true );
    testBool( approxEquals(2.0, 2.1), false );
    testBool( approxEquals(2.0, 2.000000001), true );
    testBool( approxEquals(-2.0, +2.0), false );
    testBool( approxEquals(-1e-10,+1e-10), true );
    testBool( approxEquals(-1e+10,+1e+10), false );


    testBool( approxEquals( INFINITY, INFINITY), true );
    testBool( approxEquals( INFINITY,-INFINITY), false );
    testBool( approxEquals(-INFINITY, INFINITY), false );
    testBool( approxEquals(-INFINITY,-INFINITY), true );
    testBool( approxEquals( INFINITY, LONG_MAX), false );

    testBool( approxEquals(1.0/INFINITY, 0.0), true );
    testBool( approxEquals(0.0, 1.0/INFINITY), true );
    testBool( approxEquals(-0.0, -1.0/INFINITY), true );
    testBool( approxEquals(+0.0, -1.0/INFINITY), true );
    testBool( approxEquals(+0.0, -0.0), true );
    
    
    
    testBool( approxEquals(NAN, NAN), false );
    testBool( approxEquals(NAN, 17), false );
    testBool( approxEquals(17, NAN), false );
    testBool( approxEquals(INFINITY, NAN), false );
    testBool( approxEquals(NAN, INFINITY), false );

    printTestMsg("\nTesting arrT_toString: ");
    int* arr5i = malloc(5*sizeof(int));
    arr5i[0] = 7;
    arr5i[1] = 22;
    arr5i[2] = -307;
    arr5i[3] = INT_MAX;
    arr5i[4] = INT_MIN;
    testStr( arrI_toString(arr5i, 5, "<", "%d", " : ", ">"),
             "<7 : 22 : -307 : 2147483647 : -2147483648>" );
    int* arr6i = malloc(6*sizeof(int));
    arr6i[0] = 7;
    arr6i[1] = 22;
    arr6i[2] = -307;
    arr6i[3] = 20;
    arr6i[4] = 777777;
    arr6i[5] = 777777;
    testStr( arrI_toString(arr6i, 6, "<", NULL, " : ", ">"),
             "<7 : 22 : -307 : 20 : 777777 : 777777>" );
    double arr3d[] = { 3.14159, 0.0, -2.718281828 };
    testStr( arrLf_toString(arr3d, 3, NULL, "%+05.1lf", NULL, NULL),
             "[+03.1,+00.0,-02.7]" );
    testStr( arrC_toString("hello", 3, "", "%c", "", ""),
             "hel" );
  

    
    printTestMsg("\nTesting array init/fill");
    const uint SZ1 = 8;
    int* numsOnHeap = newArrayI_uninit(SZ1);
    int numsOnStack[] = { 3, 17, 9, -2, 0, 99, 3, 128 };

    fillArrayI( numsOnHeap, SZ1, 99 );
    fillArrayI( numsOnStack, SZ1, 99 );
    for (uint i=0;  i<SZ1;  ++i) {
        testInt(numsOnHeap[i], 99);
        testInt(numsOnStack[i], 99);
        }
    fillArrayI_rand( numsOnHeap, SZ1, 100,102 );
    fillArrayI_rand( numsOnStack, SZ1, 100,102 );
    bool found100_stack = false, found101_stack = false;
    bool found100_heap  = false, found101_heap  = false;
    for (uint i=0;  i<SZ1;  ++i) {
        found100_stack = found100_stack || (numsOnStack[i]==100);
        found100_heap  = found100_heap  || (numsOnHeap [i]==100);
        found101_stack = found101_stack || (numsOnStack[i]==101);
        found101_heap  = found101_heap  || (numsOnHeap [i]==101);
        testBool(100 <= numsOnHeap[i]  && numsOnHeap[i]  < 102, true);
        testBool(100 <= numsOnStack[i] && numsOnStack[i] < 102, true);
        }
    testBool(found100_stack && found101_stack, true);
    testBool(found100_heap  && found101_heap , true);
    free(numsOnHeap);
    
    
    
    printTestMsg("\nTesting swap: ");
    int i=5;
    int j=7;
    swap_i(&i,&j);
    testInt(i,7);
    testInt(j,5);

    swap_i(&j, &arr6i[3]);
    testInt(j,20);
    testInt(arr6i[3],5);

    
    
    
    
    
    printTestSummary();
    return 0;
    }
