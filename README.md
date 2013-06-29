# calc

Calc is a proof of concept to implement a simple arithmetic language
interpreter with JIT compilation, using either LLVM or a more low-level JIT
based on GNU Lightning.

A fun weekend project, if you ask me!

# Install

You need LLVM and GNU Lightning installed. On Mac OSX:

    $ brew install llvm lightning

Now:

    $ git clone git://github.com/txus/calc.git calc
    $ cd calc
    $ make

You get the `calc` executable.

## Usage

Using the AST-walking interpreter:

    $ ./calc "3 * (4 + 5)"

Using the LLVM JIT compiler:

    $ ./calc --llvm "3 * (4 + 5)"

Using the lower-level, GNU-lightning based JIT compiler:

    $ ./calc --jit "3 * (4 + 5)"

## Syntax

It's just integers, these operators: `+ - * /`, and parentheses for precedence.

## Who's this

This was made by [Josep M. Bach (Txus)](http://txustice.me) under the MIT
license. I'm [@txustice][twitter] on twitter (where you should probably follow
me!).

[twitter]: https://twitter.com/txustice