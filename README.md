# simple-language-interpreter
Interpreter of simple language.

## Description
University project...

it's interpretor of simple language maded up by my university teacher.
It was so long ago, I can't remember it's syntax.
But this program is working...
You can try and help me figure out syntax of this languge :)

## Usage
1. Compile `source.cpp` via g++ for example.
2. Insert desired program into `input.txt`.
3. Launch program.

You will get `output.txt` with result of your program in it.

## Syntax

expressions(exps):

    - "(val <int>)"
    - "(var <id>)"
    - "(add expr1 expr2)" 
    - "(if expr1 expr2 then expr_then else expr_else)" if expr1 > expr2 then expr_then else expr_else
    - "(let <id> = expr in body_expr)"
    - "(function <id>  body_expr)"
    - "(call f_expr arg_expr)"
    - "(reduce expr1 expr2)"