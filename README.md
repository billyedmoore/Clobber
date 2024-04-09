# Clobber

![Example command line usage.](images/terminal_screenshot.png)

Simple UNIX shell. So named as clobber is a synonym of bash, how funny I know.

## Features

+ Run any program, support for the same programs as bash using *execvp* and forking.
+ Run programs in the background by appending "&", produces an output when the background job is exited.
+ Built-in programs "cd", "help" and "exit".
+ When a program is running in the foreground CTRL-C is redirected to close that program, otherwise CTRL-C 
causes Clobber to exit.
+ Non-interactive mode, i.e. running commands from files using "clobber.out < "

### Limitations

+ No redirection (e.g command1 >> filename).
+ No piping from one process to another (e.g command1 | command2).

## Usage 

Build with gcc.
```
$ make build
```
Run in interactive mode.
```
$ ./clobber.out
```
Run a script, where *SCRIPT_PATH* is the path of the script.
```
$ ./clobber.out < SCRIPT_PATH
```
