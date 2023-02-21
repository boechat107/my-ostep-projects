# OSTEP projects - C

This folder contains all the implementations of the OSTEP projects in C.

> WIP: I need to move the project "initial-utilities" to this folder. The
> objective is to reuse code as much as possible.

## Processes Shell - `wish`

A very simple Unix shell.

[Exercise page](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell)

### Usage

``` shell
make
# Interactive mode
./build/wish
# Process a script
./build/wish examples/wish_script.txt
# Debugging with valgrind
make valgrind args='./build/wish examples/wish_script.txt'
```

### Todo

* [x] built-in commands
  * [x] `exit`
  * [x] `cd`
  * [x] `path`

* [x] fork process
  * [x] set default value for path
  * [x] `ls` works

* [x] execute forked processes using the user paths

* [x] read script files

* [ ] redirection

* [ ] parallel commands

* [ ] increase the value `EXEC_ARG_STEP_SIZE`
