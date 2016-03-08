# The 10-second overview of Makefile:  Format:
#
#   [goal] : [dependency]...
#	[rules]
#
# If the file [goal] is older than any of the [dependency] files, then run [rules].
# But first make sure each [dependency] is itself up-to-date, recursivly.
# For example, if ibarland-utils.c is edited/updated,
# this would 

test: run-utils-test run-utils-test-memory command-line-options-test
	command-line-options-test

run-utils-test: ibarland-utils-test
	ibarland-utils-test

ibarland-utils-test: ibarland-utils.o ibarland-utils-test.c
	gcc -Wall -Wextra ibarland-utils-test.c -o ibarland-utils-test ibarland-utils.o
	./ibarland-utils-test

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	gcc -Wall -Wextra -c ibarland-utils.c


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


command-line-options-example: command-line-options-example.c command-line-options.o ibarland-utils.o
	gcc -Wall -Wextra command-line-options-example.c command-line-options.o ibarland-utils.o -o sample -lm

command-line-options.o: command-line-options.c command-line-options.h ibarland-utils.o
	gcc -Wall -Wextra -c command-line-options.c

command-line-options-test: command-line-options.c command-line-options.h ibarland-utils.o
	gcc -Wall -Wextra command-line-options-test.c -o command-line-options-test ibarland-utils.o -lm

