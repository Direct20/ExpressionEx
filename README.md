# ExpressionEx
A simple calculator which supports brackets and various operator.

## Features
#### 1.Multiple Operators
```c
+(addition)
-(subtraction)
*(multiplication)
/(division)
%(modulo)
^(exponentiation)
!(fractorial)
```
#### 2.Brackets
#### 3.Negative Sign and Extra Positive Sign
#### 4.Exception Handling
```c
	{1000,"Failed to create the stack."},
	{1001,"The bracket does not match."},
	{1002,"Invalid expression."},
	{1003,"Unexpected operator in converted postfix expression."},
	{1004,"Invalid converted postfix expression info."},
	{1005,"Invalid converted postfix expression."},
	{1006,"The expression is too long.(Longer than 384 characters)"},
	{1007,"Failed to allocate memory."},
	{1008,"Cannot divide by 0."},
	{1009,"Factorial operator cannot operate a negative number."},
	{1010,"Factorial operator cannot operate a fraction."}
```
## How to Build and Compile
#### Method 1
Just add all files to an project in Visual Studio or other IDEs, then compile.
#### Method 2
Use gcc or other compilers compile all files and link the object files.

**PS.The comments inside the code file are in Chinese, and the file encoding is GB2312. This is just a beginner's assignment. :)**
