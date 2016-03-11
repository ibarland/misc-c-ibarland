# misc-c-ibarland
helper library functions for C/C++ programs

ibarland-utils: very general helper functions -- e.g. convert degrees to radians, `itoa` which allocates necessary space, etc.

command-line-options:
(a) user sets up a list of long-name-options/short-name-options/default-value structs;
(b) user calls `allOptions`, passing in argv
(c) user gets back an array of strings: one value per option (either from argv, or the defaults)
