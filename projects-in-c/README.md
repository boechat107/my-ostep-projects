# OSTEP projects - C

This folder contains all the implementations of the OSTEP projects in C.

> WIP: I need to move the project "initial-utilities" to this folder. The
> objective is to reuse code as much as possible.

## Processes Shell - `wish`

A very simple Unix shell.

[Exercise page](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell)

### Todo

* [x] built-in commands
  * [x] `exit`
  * [x] `cd`
  * [x] `path`

* [x] fork process
  * [x] set default value for path
  * [x] `ls` works

* [x] execute forked processes using the user paths

* [ ] read script files

* [ ] redirection

* [ ] parallel commands

* [ ] increase the value `EXEC_ARG_STEP_SIZE`
