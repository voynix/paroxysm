# paroxysm
### a bad idea

# What?
There are a great many programming languages in this world. Some of them are even good! This is not one of them.

To be less coy, **paroxysm** is an [esoteric](http://esolangs.org/wiki/esoteric_programming_language) programming language. It is [interpreted](https://en.wikipedia.org/wiki/Interpreted_language), [imperative](https://en.wikipedia.org/wiki/Imperative_programming), [statically typed](https://en.wikipedia.org/wiki/Type_system#STATIC) (well, there's only one type, so this is fairly trivial), [dynamically scoped](https://en.wikipedia.org/wiki/Scope_(computer_science)#Dynamic_scoping), and hopefully near-useless. It is believed to be [Turing Complete](https://esolangs.org/wiki/Turing-complete).

# Quick Start
    cd paroxysm
    make
    ./paroxysm example.pxy

# Table of Contents
* [Language Definition](#language-definition)
    * [Syntax](#syntax)
    * [Variables, Names, and Literals](#variables-names-and-literals)
    * [Truth Values](#truth-values)
    * [Infix Operators](#infix-operators)
    * [Scopes](#scopes)
    * [Flow Control](#flow-control)
* [Misc Notes](#misc-notes)
* [Formal Specification](#formal-specification)

# Language Description
### Syntax
A **paroxysm** program consists of a set of statements separated by newlines (ie by '\n').

##### Statements
A statement consists of an operator followed by an appropriate set of arguments (expressions, names, or literals).

The following operators may start a statement and take the indicated number and type of arguments:

    expand
    collapse
    init NAME
    term NAME
    set NAME EXPRESSION
    path NAME
    bifurc EXPRESSION NAME NAME
    outn EXPRESSION
    outc EXPRESSION

These operators are described under [Scopes](#scopes), [Flow Control](#flow-control), and [IO](#io).
##### Expressions
An expression is a sequence of infix operators, names, and literals separated by spaces. (See the [formal specification](#formal-specification).)

The following operators may be used in expressions and may be given expressions, variables, or literals as arguments:

###### Binary Operators
* `and`
* `or`
* `+`
* `-`
* `*`
* `/`
* `<<`
* `>>`
* `<`
* `>`
* `&`
* `|`

###### Unary (Prefix) Operator
* `!` 

###### Parentheses (Enclosing Operator)
* `()`

These operators are described under [Infix Operators](#infix-operators).

Note that there are reasonable limits on how many operators and variables or literals may be included in a single expression, so overly large expressions (ie >100 terms) may cause errors.

##### Comments
Comments begin with `;` and continue until the end of the line.

Comments may be included on lines containing code and lines of their own.

    ; This is a comment
    set x 70 ; so is this
    
    ;;; and this is a LISP-style comment!

### Variables, Names, and Literals
All variables and literal values in **paroxysm** are 32-bit signed integers. (ie, **C** *int*s)

##### Variables
Variables are referenced by name. **paroxysm** has no higher order data structures (eg, arrays, structs, objects, etc). *nefas sunt*

Operators for creating and manipulating variables are described under [Scopes](#scopes).

###### Names
A name is a reasonable length sequence of acceptable characters. Acceptable characters are 'a' - 'z', 'A' - 'Z', '0' - '9', and '_'. (in regex terms, `name ~= /[A-Za-z0-9_]+/`)

###### Literals
A literal is a positive or negative integer expressed in base 10. (eg `10`, `-400000`, `33333`)

### Truth Values
`0` evaluates as *false*; all other values evaluate as *true*.

### Infix Operators
**paroxysm** offers the following infix operators for use in expressions. All operators are left-associative.

* `and` logical and
* `or` logical and
* `+` integer addition
* `-` integer subtraction
* `*` integer multiplication
* `/` integer division (rounding follow **C**)
* `<<` bitshift left
* `>>` bitshift right
* `<` less than
* `>` greater than
* `&` bitwise and
* `|` bitwise or
* `!` bitwise negation
* `()` parentheses

`and`, `or`, `<`, and `>` return `1` if true and `0` if false. Parentheses force the enclosed expression to be evaluated first and return no value themselves. All other operators return the value calculated. (eg `set x 2 + 2` sets the value of `x` to `4`, since `2 + 2` evaluates to `4`.)

##### Precedence
Precedence is as follows, from lowest to highest:

* `and`, `or`
* `<`, `>`, `!`
* `<<`, `>>`
* `+`, `-`, `&`, `|`
* `*`, `/`
    
##### NB
Note that operators and their arguments MUST be separated by spaces. eg:

    set y (x+2) ; NO
    set y ( x + 2 ) ; YES!

### Scopes
Since **paroxysm** has no notion of a 'block' of code, it does not use lexical scope.

Scopes are explicitly declared with `expand`; one anonymous scope is created at program startup for each `expand`.

During program execution **paroxysm** maintains a stack of scopes. Note that at program startup this stack is empty.

##### Stack Manipulation
Scopes are popped onto the stack with `expand` and pushed off with `collapse`.

Note that if the scope associated with a given `expand` is already active (ie on the stack) executing that `expand` again will have no effect.

Executing `collapse` when the stack is empty is an error.
##### Variables
The following operators are used to manipulate variables within scopes. Using any of these operators when the stack is empty is an error.

* `init example` creates a variable called `example` in the scope on top of the stack if no variable called `example` exists in any scope on the stack. If `example` already exists in some scope on the stack `init example` does nothing.
* `set example x + 2 * ( 3 + 4 )` sets the value of the variable called `example` to `x + 2 * ( 3 + 4 )`. If there are multiple variables called `example` in different scopes on the stack the `example` in the highest scope is modified. All other instances are unaffected.
* `term example` removes the variable called `example` from its scope on the stack. If there are multiple variables called `example` if different scopes on the stack the `example` in the highest scope is modified. All other instances are unaffected.

If there are multiple variables with the same name in different scopes on the stack, any reference to that variable will evaluate to the value of the instance of the variable in the highest scope on the stack.

Scopes retain their variables when popped off the stack with `collapse`; variables are only removed from stacks by `term`.

### Flow Control
Functions are silly; we don't need them and so **paroxysm** does not include them.

`goto` and `if` are nice, but with our recently slashed budget, we can hardly afford both. Hence, **paroxysm** offers exactly one flow-control operator: `bifurc`.

`bifurc` evaluates the given expression and then transfers program flow to one of the two paths named (Recall from [above](#syntax) that the syntax for `bifurc` is `bifurc EXPRESSION NAME NAME`). If the expression evaluates to true, the first path is taken; if the expression evaluates to false the second path is taken.

`path` is used to create paths for `bifurc` to jump to. `path p>` creates a path called `p`; when jumped to, program execution will continue at the line immediately after `path p`. Path names must be unique across a program but may overlap with variable names (ie, a path named `x` will *not* interfere with a variable named `x`).

`path` statements are evaluated solely at program startup; during program execution `path` statements do nothing. Thus, in the example below, after jumping to `false`, execution will proceed through the `outc`, the declation of path `true`, and then on to `outn`.

The following example demonstrates the use of `bifurc` and `path`.

    bifurc x true false
    
    ; if x is 0, the bifurc will jump here
    path false
    outc x
    
    ; if x is not 0, the bifurc will jump here
    path true
    outn x
     
### IO
##### Output
**paroxysm** includes two output operators, both of which print to standard output.

* `outn EXPRESSION` interprets the value of the given expression as a number and prints it
* `outc EXPRESSION` interprets the value of the given expression `% 128` as an **ASCII** character and prints it

**paroxysm** does *not* automatically print whitespace or newlines after `outn` or `outc`. 
  
##### Input
At present **paroxysm** includes no input operators. An `in` operator is planned for a future release.
# Misc Notes
**paroxysm** has no error-recovery or reporting mechanism at present; any errors will cause the program to terminate with an assertion failure. More user-friendly error-reporting is planned for a future release.

The preferred extension for **paroxysm** files is `.pxy`.
# Formal Specification

    PROGRAM = STATEMENT*
    
    STATEMENT = expand
    STATEMENT = collapse
    STATEMENT = init NAME
    STATEMENT = term NAME
    STATEMENT = set NAME EXPRESSION
    STATEMENT = path NAME
    STATEMENT = bifurc EXPRESSION NAME NAME
    STATEMENT = outn EXPRESSION
    STATEMENT = outc EXPRESSION
    
    EXPRESSION = NAME
    EXPRESSION = LITERAL
    EXPRESSION = EXPRESSION and EXPRESSION
    EXPRESSION = EXPRESSION or EXPRESSION
    EXPRESSION = EXPRESSION + EXPRESSION
    EXPRESSION = EXPRESSION - EXPRESSION
    EXPRESSION = EXPRESSION * EXPRESSION
    EXPRESSION = EXPRESSION / EXPRESSION
    EXPRESSION = EXPRESSION << EXPRESSION
    EXPRESSION = EXPRESSION >> EXPRESSION
    EXPRESSION = EXPRESSION < EXPRESSION
    EXPRESSION = EXPRESSION > EXPRESSION
    EXPRESSION = EXPRESSION & EXPRESSION
    EXPRESSION = EXPRESSION | EXPRESSION
    EXPRESSION = ! EXPRESSION
    EXPRESSION = ( EXPRESSION )
    
    NAME = [A-Za-b0-9_]+
    
    LITERAL = [0-9]+
    LITERAL = -[0-9]+