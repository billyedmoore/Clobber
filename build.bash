#! /bin/bash
gcc src/main.c src/builtins.c src/parsing.c -o clobber.out
./clobber.out
