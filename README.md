# Find
A simple implementation of GNU/Linux find command written in C.

## Installation
Just run make in the repository directory in order to compile program.
There are no special dependencies, as it is using only C standard library.
It will do it in two times, first it'll create a object file, then a runnable file called program.

## Running
In order to run program just go into repository folder then run ./program [-L|-P] <folder> <expression> [arg]

## Usage
Usage : find [-L|-P] [path] [expression]
  expression :
    -name <name>
    -type <type> (f,d,l,b,c,p,s)
    -executable
    -empty
    -anewer <file>
    
##Licensing
You are free to use this as you want, except for terrorism and if you are a cat.
