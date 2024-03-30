#! /bin/bash
gcc src/main.c src/builtins.c src/parsing.c src/signals.c src/execute.c -o clobber.out -g3
./clobber.out
