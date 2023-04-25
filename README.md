# ExpressionEx
A simple calculator which supports brackets and various operators.Mainly used Shunting-Yard algorithm. You can also add more operators easily.

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
#### 3.Floating Numbers and Negative Numbers
#### 4.Exception Handling like C++
Imitated the exception handling form of C++ using `setjmp` and `longjmp`:
```c
    TRY(LABEL)
    {
        THROW(exc);
        ...
    }
    CATCH()
    {
        e=CATCH_var(LABEL);
        ...
    }
```
Exception definitions:
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

## How to Use
Enter an expression which only consists of numbers and operators mentioned above, press ENTER.  
There is an instance below.
``` c
3!+4*2%10/(1-5)^2
Value=6.5
```  

**P.S. The comments inside the code files are in Chinese, and the file encoding is GB2312. This is just a beginner's assignment. :)**
