# The 10-second overview of Makefile:  Format:
#
#   [goal] : [dependency]...
#	[rules]
#
# If the file [goal] is older than any of the [dependency] files, then run [rules].
# But first make sure each [dependency] is itself up-to-date, recursivly.
# For example, if ibarland-utils.c is edited/updated,
# this would 

# command-line args for C:
#
CC       = gcc
CFLAGS   = -O3 -g \
           -Wall -Wextra \
           -Wpointer-arith -Wcast-qual -Wcast-align \
           -Wwrite-strings -Wconversion \
           -Wno-ignored-qualifiers -Wno-float-conversion \
	   -Woverloaded-virtual
#
#          -Wno-incompatible-pointer-types-discards-qualifiers # not on vm660 \
#          -Wno-discarded-qualifiers  # not on tropic

#
# don't include:
# -Wincompatible-pointer-types-discards-qualifiers -- triggers all sorts of errors if passing a string-literal to a function wanting a char*.
# -pedantic: doesn't allow fancier gcc features (like var-adic macros like DPRINTF)
# -Wtraditional: gave me headaches with "printf already defined in a block"
# -Wstrict-prototype, because that disallows #  "double foo();" and requires "double foo(void);" instead.
# -Wenum-clash: it's available for C++ only?
#


CPPFLAGS = -I$(HOME)/Src
LDLIBS   = -L$(HOME)/Src -lm
# NOTE: remove `-lrt` if it's causing problems; some versions of gcc don't like that flag.
CC_ALL_FLAGS = $(CC) $(CFLAGS) $(CPPFLAGS)


# command-line args for C++
#
C++		= g++
C++FLAGS	= $(CFLAGS) -Wenum-clash



test: run-utils-test run-utils-test-memory run-command-line-options-test

run-command-line-options-test: command-line-options-test command-line-options-example
	command-line-options-test
	command-line-options-example
	command-line-options-example --file fromCmdLine
	command-line-options-example --size XXXXM --stuff stuffity-stuff -o other-stuffity-stuff --name fromCmdLine

run-utils-test: ibarland-utils-test
	ibarland-utils-test

ibarland-utils-test: ibarland-utils.o ibarland-utils-test.c
	$(CC_ALL_FLAGS) ibarland-utils-test.c -o ibarland-utils-test ibarland-utils.o $(LDLIBS)

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	$(CC_ALL_FLAGS) -c ibarland-utils.c


run-utils-test-memory: ibarland-utils-test
	@if [ ! `command -v valgrind` ]; then  \
            echo "install \`valgrind\` to test for memory-leaks." ; \
            echo "(If you have a package-manager, like \`homebrew\` for os x, installation might be a one-line task.)" ; \
        else \
            echo "valgrind ibarland-utils-test" ; \
            valgrind ibarland-utils-test 2>&1 | egrep "definitely|indirectly|(ERROR|LEAK) SUMMARY" ; \
            echo "(run valgrind at command-line, to see full error report)" ; \
        fi
	@# Notes about sh syntax:
	@# The  `if [...]; then ... ; else ... ; fi`  is just sh's if-then-else.
	@# The trailing "\" on each line continues the shell-command to the next line.  (It's quoting the newline.)
	@# The `command -v cmd` returns the full path for `cmd` (or, empty-string if `cmd` not found).
	@# The preceding "@" means to suppress make's normal policy of echoing the command it runs.
	@# The  `cmdA 2>&1 | cmdB` means to pipe cmdA's stderr into cmdB's stdin.

clean:
	rm -f  *.o  ibarland-utils-test command-line-options-example  command-line-options-test 
	rm -f *.exe
	rm -rf *.app/  *.dSYM


command-line-options.o: command-line-options.c command-line-options.h ibarland-utils.o
	$(CC_ALL_FLAGS) -c command-line-options.c

command-line-options-test: command-line-options-test.c command-line-options.o ibarland-utils.o
	$(CC_ALL_FLAGS) command-line-options-test.c -o command-line-options-test ibarland-utils.o $(LDLIBS)

command-line-options-example: command-line-options-example.c command-line-options.o ibarland-utils.o
	$(CC_ALL_FLAGS) command-line-options-example.c -o command-line-options-example command-line-options.o ibarland-utils.o $(LDLIBS)
