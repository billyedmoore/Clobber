.DEFAULT_GOAL := build

build: 
		@echo "Building"
		gcc src/main.c src/builtins.c src/parsing.c src/signals.c src/execute.c src/commands.c -o clobber.out
run: build
		@echo "Running"
		./clobber.out
debug: 
		@echo "Debugging with gbd"
		gcc src/main.c src/builtins.c src/parsing.c src/signals.c src/execute.c src/commands.c -o clobber.out -g
		gdb ./clobber.out
memtest: build
		@echo "Running with valgrind"
		valgrind ./clobber.out
clean:
		@echo "Cleaning"
		rm clobber.out
