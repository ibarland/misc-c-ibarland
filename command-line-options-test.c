#define _GNU_SOURCE    // Needed for rucs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ibarland-utils.h"

// FOR TESTING private functions: We #include the actual C def'ns:
#include "command-line-options.c" 
// (So DON'T link against command-line-options.o.)

void test1(){
    printf("\nTesting extractLongOptionName.\n");
    testStr( extractLongOptionName("--hello"), "hello" );
    testStr( extractLongOptionName("noLeadingDashes"), NULL );
    testStr( extractLongOptionName("-hello"), NULL );
    testStr( extractLongOptionName("-h"), NULL );
    testStr( extractLongOptionName("--"), NULL);
    testStr( extractLongOptionName(""), NULL);
    testStr( extractLongOptionName(NULL), NULL );
    }

void test2(){
    printf("\nTesting extractShortOptionName.\n");
    testChar( extractShortOptionName("-h"), 'h');
    testChar( extractShortOptionName("h"), '\0');
    testChar( extractShortOptionName("hello"), '\0');
    testChar( extractShortOptionName("--hello"), '\0');
    testChar( extractShortOptionName("h-"), '\0');
    testChar( extractShortOptionName("-"), '\0');
    testChar( extractShortOptionName("--"), '\0');
    testChar( extractShortOptionName(""), '\0');
    testChar( extractShortOptionName(NULL), '\0');
    }



void test3() {
    printf("\nTesting findOption.\n");
    char* sample1[] = { "--hello","tag", "-b","99", "--", "--hello", "tag2" } ;
    char* sample2[] = { "--hello","tag", "-b","99", "--hello", "tag2" } ;
    struct option_info options[] = {
        { "hello", 'h', "ibarland", "the name of the package-author" },
        { "bye", 'b', "99", "the size of the frobzat, in meters." },
        };
    testStr( findOption( options[0], SIZEOF_ARRAY(sample1), sample1 ),   "tag" );
    testStr( findOption( options[1], SIZEOF_ARRAY(sample1), sample1 ),   "99" );
    testStr( findOption( options[0], SIZEOF_ARRAY(sample2), sample2 ),   "tag2" );
    }


void test4() {
    printf("\nTesting apparentOptionIsLegal.\n");
    struct option_info options[] = {
        { "name", 'n', "ibarland", "the name of the package-author" },
        { "size", 's', "45", "the size of the frobzat, in meters." },
        };
    int NUM_OPTIONS = SIZEOF_ARRAY(options);
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "--name" ), true );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "--size" ), true );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "-s" ), true );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "-n" ), true );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "blah" ), true );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "--zasd" ), false );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "-z" ), false );
    testBool( apparentOptionIsLegal( NUM_OPTIONS, options, "--" ), true );
    }

int main () {
    test1();
    test2();
    test3();
    test4();
    printf("\n");
    return 0;
    }

