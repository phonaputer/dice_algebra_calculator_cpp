# Dice Algebra Calculator - Written in C++

A dice algebra expression lexer, parser, & executor written in C++. 

The purpose of writing this application is to have fun trying out C++ by writing a simple application.
A dice algebra calculator (including lexing, parsing, and execution of string expressions) was selected as something that is probably complex enough to be interesting yet is simple enough to do in a short amount of time.

It's also something that can be extended later if that seems like it would be fun (e.g. this application could run a GUI window in which expressions can be input & a breakdown of the results can be shown, etc.).

And finally this is a project which can be done in any language, which allows comparing and constrasting (also fun).

The goal here is to write a program with source code which is as clean as possible (not an easy task without an old hand to ask questions to) and learn about tooling around the language. 

## What is "Dice Algebra?"

Dice algebra consists of simple mathematical expressions where operands may be a "dice roll."

A simple dice roll takes the format `xdy` (or `xDy`) where both `x` and `y` must be integers. 
`xdy` means that a `y`-sided die will be rolled `x` times. For example, `3d6` will roll a 6-sided die three times and sum the results.

The leading `x` may be omitted if it is 1. For example, `d4` rolls a 4-sided die one time.


When rolling more than one die it is possible to keep only the lowest `n` rolls or the highest `n` rolls by appending `ln` or `hn`, respectively, to the roll. For example, `2d20h1` will roll two 20-sided dice and keep the highest result.

In addition to rolling dice, it is possible to include integers, addition `+`, subtraction `-`, multiplcation `*`, integer division `/`, and parenthetical expressions `(...)`. For example, `(2d6 + 5) * 10` will roll two 6-sided die, add five to that result, then mutiply that result by ten. 

All integers must be positive (or 0).

## ANTLR Grammar

The above dice algebra format can be expressed as the following ANTLR 4 grammar. This grammar is more-or-less what this application targets when parsing input.

```ANTLR
grammar DiceAlgebra;

// Parser

add : mult (('+' | '-') mult)* ;
mult : atom (('*' | '/') atom)* ;
atom : (roll | '(' add ')') ;
roll : (integer | longroll | shortroll) ;
longroll : integer D integer ((H integer | L integer))? ;
shortroll : D integer ; 
integer : NUMBER ;

// Lexer

WHITESPACE : ' ' -> skip ;
NUMBER : [0-9]+ ;
D : 'd' | 'D' ;
PLUS : '+' ;
MINUS : '-' ;
MULT: '*' ;
DIV : '/' ;
OPENPAREN : '(' ;
CLOSEPAREN : ')' ;
H : 'h' | 'H' ;
L: 'l' | 'L' ;
```

## How to Build and Run Locally

...coming soon...
