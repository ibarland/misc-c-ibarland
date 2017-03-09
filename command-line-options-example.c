/* A file to demo the use of command-line-options's `allOptions`.
 * If this file is named 'sample.c', compile it with:
 *    gcc -Wall sample.c -o sample   command-line-options.o ibarland-utils.o -lm
 *
 * (If one of the .o files doesn't exist, create it by compiling with -c.  E.g.:)
 *   gcc -Wall -c command-line-options.c
 */

#define _GNU_SOURCE    // Needed for rucs
#include "ibarland-utils.h"
#include "command-line-options.h"


/* The possible command-line options to a program. 
 */
struct option_info options[] =
  {  { "file",  'f',  "foo.txt", "the file to blazlblarg" }
    ,{ "name",  'n',  "ibarland", "the primary blazlbarger" } 
    ,{ "size",  's',  "98", "how many blazls to blarg (in dozens)" }
    ,{ "stuff", '\0', NULL, "what to call your stuff" }
    ,{ "otherStuff", 'o', "blarg", "the help info for other stuff" }
    };
/* After declaring the above, you can invoke the program with (say) 
 *   command-line arguments  `--size 44 -f baz.txt`,
 * and then `allOptions` will return:
 *    { "baz.txt", "ibarland", "44", NULL, "blarg" }
 * Note that these values are in the order that you specify in your array-of-option_info.
 */
  
#define NUM_OPTIONS SIZEOF_ARRAY(options)


int main( int argc, char** argv ) {
    char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.

    printf("The size is %s.\n", settings[2] );
    
    printf("Here are all the options:\n");
    for (unsigned int i=0;  i<NUM_OPTIONS;  ++i) {
        printf("  Option #%d (%s -- %s) is \"%s\".\n", i, options[i].longOption, options[i].helpString, settings[i] );
        }

    return 0;
    }

