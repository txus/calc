# calc

Calc is a proof of concept to implement a simple arithmetic language
interpreter with JIT compilation, using LLVM.

**Note**: the JIT mode doesn't work yet. It falls back to an AST-walking
interpreter.

A fun weekend project, if you ask me!

# Install

You need LLVm installed. On Mac OSX:

    $ brew install llvm

Now:

    $ git clone git://github.com/txus/calc.git calc
    $ cd calc
    $ make

You get the `calc` executable.

## Usage

    $ ./calc [--jit] "3 * (4 + 5)"

When run without `--jit`, it falls back to an AST-walking interpreter.

## Syntax

It's just integers, these operators: `+ - * /`, and parentheses for precedence.

## Who's this

This was made by [Josep M. Bach (Txus)](http://txustice.me) under the MIT
license. I'm [@txustice][twitter] on twitter (where you should probably follow
me!).

[twitter]: https://twitter.com/txustice