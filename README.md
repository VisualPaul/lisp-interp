lisp-interp
===========

Tiny Lisp interpreter

Introduction
------------

This is a small project on creation of a small Lisp interpreter. The language itself is something Common Lisp-like... but it is Lisp-1.

Building the project
--------------------

To build a project you need a gcc-like C++11 capable compiler, CMake, gmp & gmpxx. For older versions you also required boost::regex, but nowdays native std::regex is good enough.
Use following commands:
```sh
git clone https://github.com/VisualPaul/lisp-interp.git # To download the source code
cmake lisp-interp # to create Makefile
cd lisp-interp
make -j
```

Using the interpreter
---------------------

To use the interpreter you shall run ./main
There are two forms of this command: interactive REPL `./main` and non-interactive interpreter `./main your-code.lisp`
The difference should be clear.

Quick introduction to the language
----------------------------------

For now there is no better documentation, so.

This is a typical Lisp-1, there is a single namespace for everything. Lexical scoping.

We are a case-sensitive language.

The language contains several special forms:
```lisp
(quote x)
```
This special form simply returns x, i.e. if x is a symbol it will return symbol x. As a shorthand for this special form you can use 'x.
This code will be transformed to a normal quote during the parsing. Code that contains several elements after `quote` is incorrect
```lisp
(if condition then-expression else-expression)
```
This code evalutes the `condition`, if it is `nil` it will evalute `else-expression` and report it as result of the while expression.
Otherwise it will report evaluted `then-expression` as the result. `else-expression` may be ommited, in this case it is defaulted to `nil`.
```lisp
(let bind-list expressions)
```
`bind-list` is a list which contains the description of the variables. Each it's element should be either symol (in this case the variable is set to nil)
or list(it is 1 or 2 elements, in the later case second element of the list is initial value of the variable).
After the bindings are set the `expressions` are evaluted consequently. The result of the last expression is taken as the result of the expression at whole.
```lisp
(prog expressions)
```
Simply evalutes `expressions` consequently, result of the last expression is the resulty of the expression as whole
```lisp
(lambda name argument-list body)
```
Creates a function named `name` (which can be ommited, in this case it's value is defaulted to lambda) with arguments named by the symbols, given in `argument-list`.
The function `body` is evaluted consequently. The last expression is the result of the function. Note: this special form creates no binding,
to do so you shall use either `defvar` special form or `defun` macro.
```lisp
(macro name argument-list body)
```
Just like `lambda`, only creates a special kind of function, called "macro", which arguments are not evaluted by default
```lisp
(set variable1 value1 variable2 value2 ....)
```
Sets the value of the last variable named `variable1` to the result of evalution of `value1`, `variable2` to the result of evalution of `value2`, and so on
```lisp
(defvar variable-name-1 expression-1 variable-name-2 expression-2 ...)
```
Creates new variables named variable-name-1, variable-name-2, etc and sets their value to the result of evalution of expression-1, expression-2, etc
```lisp
(while condition body...)
```
Evalutes all expressions within body while condition evalutes to non-nil
