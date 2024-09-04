<h1>Let's build Compiler</h1>

Building a compiler is not an easy task and definitely not a small one. So let's breakdown these steps as follows:

<ol>
<li>Lexical Analysis</li>
<li>Grammar Analysis</li>
<li>Semantic Analysis</li>
<li>Meaning Translation</li>
</ol>

Let's build a Lexical Scanner now.

<h2>What is a Lexical Scanner</h2>

A lexical scanner (or lexer for short) is a fundamental component in the process of compiling or interpreting a programming language. Its primary function is to break down a sequence of characters in the source code into meaningful units, called tokens. This process is often referred to as lexical analysis.

<i>This is the GPT Definition of a Lexer</i>

<h3>My definition goes like:-</h3> 
Imagine you are backpacking through western Europe and you meet an individual who is not familiar with your native language so you use a translator to identify that foreign language or a high level language in your perspective to further process into your comfort language or a low level language which you can understand.

The job of the Lexical Scanner or a lexer in short is to identify the lexical elements, or <i>tokens</i>, in the input language.

Each token that we scan is going to be stored in this structure

```c
// FILE NAME: defs.h
struct token
{
    int token;
    int intValue;
};
```

where the token field can be one of these values

```c
// FILE NAME: defs.h
enum
{
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT
};
```

When the token is a T_INTLIT (i.e. an Integer Literal), the intValue field will hold the value of the integer that we scanned in.

## Functions in `scan.c`

<b>The `scan.c` file is the holds the function to our lexical scanner. </b><br>

We are going to read in one character at a time from our input file. However, there will be times when we need to "put back" a character if we have read too far ahead in the input stream. We also want to track what line we are currently on.
All this is done in the `next()` function.

```c

static int next(void)
{
    int c;

    if(Putback)
    {
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(Infile);

    if('\n' == c)
        Line++;

    return c;
}


```

The `Putback`, `Line` and `Infile` variables are defined in `data.h`

```c

extern_ int Line;
extern_ int Putback;
extern_ FILE *Infile;

```

Now to put the character back into the Input stream

```c
static void putback(int c)
{
    Putback = c;
}
```

<h3>Ignoring Whitespace</h3>

To read and skip whitespace characters until it gets to a non-whitespace character and also return it.

```c
static int skip(void){
    int c;

    c=next();

    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return (c);

}
```

<h2>Scanning Tokens and intValue</h2>

Now to write our own first lexical scanner

```c
int scan(struct token *t)
{
    int c;

    // Skip whitespace
    c = skip();

    // Determine the token based on
    // the input character
    switch (c)
    {
    case EOF:
        return (0);
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_STAR;
        break;
    case '/':
        t->token = T_SLASH;
        break;
    default:

        // If it's a digit, scan the
        // literal integer value in
        if (isdigit(c))
        {
            t->intValue = scanint(c);
            t->token = T_INTLIT;
            break;
        }

        printf("Unrecognised character %c on line %d\n", c, Line);
        exit(1);
    }

    // We found a token
    return (1);
}

```

This piece of code handles the value of tokens and also takes in value of Digits

<b>Here is the implementation of `scanint` function</b>

Once we hit a decimal digit character, we call the helper function `scanint()` with this first character. It will return the scanned integer value. To do this, it has to read each character in turn, check that it's a legitimate digit, and build up the final number. Here is the code:

```c
static int scanint(int c)
{
    int k, val = 0;

    // Convert each character into an int value
    while ((k = chrpos("0123456789", c)) >= 0)
    {
        val = val * 10 + k;
        c = next();
    }

    // We hit a non-integer character, put it back.
    putback(c);
    return val;
}

```

We start with 0 `val` value. Each time we get a character in the set `0` to `9` we convert this to an `int` value with `chrpos()`. We make `val` 10 times bigger and then adds this new digit to it.

For example, if we have the characters 5, 1, 2, we do:

<ul>
    <li>val= 0 * 10 + 5, i.e. 5</li>
    <li>val= 5 * 10 + 1, i.e. 51</li>
    <li>val= 51 * 10 + 2, i.e. 512</li>
</ul>

Right at the end, we called the function `putback(c)`. We found a character that's not a decimal digit at this point.
Since we cant discard it, we can put it back in the input stream to consume later.

Later we can also use chrpos("0123456789abcdef") to work for hexadecimal digits as well.

Here's the `chrpos()` function:

```c
static int chrpos(char *s, int c)
{
    char *p;

    p = strchr(s, c);
    return (p ? p - s : -1);
}
```

<br><br>

<h2>Now Let's put the Scanner to Work</h2>

The code in `main.c` puts the above scanner to work. The `main()` function opens up a file and then scans it for tokens:

```c
void main(int argc, char *argc[]){
    ...
    init();
    ...

    Infile = fopen(argv[1], "r");
    ...
    scanFile();
    exit(0);
}
```

And `scanfile()` loops while there is a new token and prints out the details of the token:

```c
char *tokstr[] = { "+", "-", "*", "/", "intlit" };
static void scanfile() {
  struct token T;

  while (scan(&T)) {
    printf("Token %s", tokstr[T.token]);
    if (T.token == T_INTLIT)
      printf(", value %d", T.intValue);
    printf("\n");
  }
}
```


<h2>Conclusion and What's Next</h2>

We've started small and we have a simple lexical scanner that recognizes the four main maths operators and also integer literal values. We saw that we needed to skip whitespace and put back characters if we read too far into the input.

Single character tokens are easy to scan, but multi-character tokens are a bit harder. But in the end, the scan() function returns the next token from the input file in a `struct token` variable:

```c
struct token
{
    int token;
    int intValue;
};

```

<br>
<br>
<br>
<br>

Let's build a Parser now.

<h2>What is a Parser?</h2>

A parser is a crucial component responsible for analyzing the structure of the source code. The parser takes the output generated by the lexical analyzer (also known as a lexer) and organizes it into a format that can be further processed by the compiler.

<i>This is the GPT Definition</i>

<h3>My Definition goes like:</h3>

Let's use our previous example the identified data from the lexer we get from the translator is now traversed and parser recognizes the syntax and structural elements of the input and ensure that they conform to the <i>syntax</i> of the language.

Well we can already handled then four basic math operations and the whole numbers.

<h2>BNF: Backus-Naur Form</h2>

Backus Naur Form or BNF is a formal method for describing the syntax of programming language.

Let's express maths expressions wit whole numbers. Here is the BNF description of the grammar:

```bash

expression: number
          | expression '*' expression
          | expression '+' expression
          | expression '-' expression
          | expression '/' expression
          ;

number : T_INTLIT
       ;

```

The above expression is read as:

<ul>
    <li>An expression could be just a number, or</li>
    <li>An expression is two expressions separated by '+' token, or</li>
    <li>An expression is two expressions separated by '-' token, or</li>
    <li>An expression is two expressions separated by '*' token, or</li>
    <li>An expression is two expressions separated by '/' token</li>
    <li>A number is always a T_INTLIT token</li>
</ul>

<h2>Recursive Descent Parsing</h2>

Given that the grammar for our language is recursive, let's try to parse it recursively. What we can do is to read in a token, the look ahead to the next token. Based on what the next token is, we can then decide what path we need to take to parse the input.

In out case, the first token in any expression will be a number and this may be followed by maths operator. After that there may only be a single number, or there may be the start of a whole new expression.

Pseudo-code for a recursive code looks something this:

```
function express(){

    Scan and check the first token is a number. Error if it's not
    Get the next token
    If we have reached the end of the input, return, i.e. base case

    Otherwise, call expression()
}
```

Let's run this function on input 2+3-5 T_EOF where T_EOF is a token that reflects the end of the input. I will number each call to expression().

```
    expression0:
        Scan in the 2, it's a number
        Get next token, +, which isn't T_EOF
        Call expression()

        expression1:
            Scan in the 3, it's a number
            Get next token, -, which isn't T_EOF
            Call expression()

                expression2:
                    Scan in the 5, it's a number
                    Get next token, T_EOF, so return from expression2
            return from expression1
        return from expression0
```

Yes, the function was able to recursively parse the input `2 + 3 - 5 T_EOF`

Of course, we haven't done anything with the input but that's it the parser only parses the input and warn about syntax errors.

In short Parser is just syntax analysis.

Semantic Analysis is someone else's job.

<h2>Abstract Syntax Trees</h2>

An AST is a Data Structure used in compilers and interpreters to represent the structure of the source code. It abstracts away the specific syntax of the code (such as parenthesis, semicolons, etc.) and instead represents the hierarchial structure of the code in terms of its programming constructs.

The structure of each node of AST that we will build is in `defs.h`

```c
//AST node types

enum{
    A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
};

//AST Structure

struct ASTnode{
    int op;     //"Operations to be performed on the tree."
    struct ASTnode* left;
    struct ASTnode* right;
    int intValue;
};
```

Some AST nodes, like those with op values A_ADD and A_SUBTRACT have two child ASTs that are pointed to by left and right. Later on, we will add or subtract the values of the sub-trees.

<h2>Building AST Nodes and Trees</h2>

The code in `tree.c` has the functions to build ASTs. The most general function, mkastnode(), takes values for all four fields in an AST node. It allocates the node, populates the field values and returns the pointer to the node:

```c
#include "defs.h"
#include "data.h"
#include "decl.h"

struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *right, int intValue){
    struct ASTnode *n;

    n = (struct ASTnode *)malloc(sizeof(struct ASTnode));

    if(n==NULL){
        fprintf(stderr, "Unable to malloc in mkastnode()\n");
        exit(1);
    }

    n->op = op;
    n->left = left;
    n->right = right;
    n->intValue = intValue;
    return (n);
}


//Make a leaf node
struct ASTnode *mkastleaf(int op, int intValue){
    return (mkastnode(op, NULL, NULL, intValue));
}
//Make a unary node
struct ASTnode *mkastunary(int op, struct ASTnode*left, int intValue){
    return (mkastnode(op, left, NULL, intValue));
}
```

<h2>Purpose of an AST</h2>
We'll store each expression that we recognize so that, later on, we can traverse it recursively to calculate the final value of the expression.

We also need to deal with the precedence of the operators.

lets consider a example:
2*3+4*5. Now, multiplication has higher precedence than addition. Therefore, we want to <i>bind</i> the multiplication operands together and perform these operations before the addition.

```
        +
       / \
      *   *
     / \ / \
    2  3 4  5
```

When traversing the tree we first need to perform 2*3 then 4*5 and then we can pass them up to root of the tree and perform addition.

Now, we could re-use the token values from our scanner as the AST node operation values, but I like to keep the concept of tokens and AST nodes separate. So, to start with, I'm going to have a function to map the token values into AST node operation values. This, along with the rest of the parser, is in expr.c:

```c
#include "defs.h"
#include "data.h"
#include "decl.h"

static struct ASTnode *primary(void){
    struct ASTnode *n;

    switch(Token.token){
        case T_INTLIT:
            n = mkastleaf(A_INTLIT, Token.intValue);
            scan(&Token);
            return (n);
        default:
            fprintf(stderr, "syntax error on line %d\n", Line);
            exit(1);
    }

}

int arithop(int tok){
    switch(tok){
        case T_PLUS:
        return A_ADD;
        case T_MINUS:
        return A_SUBTRACT;
        case T_STAR:
        return A_MULTIPLY;
        case T_SLASH:
        return A_DIVIDE;
        default:
        fprintf(stderr, "Unknown token in arithop() on line %d\n", Line);
        exit(1);
    }
}

struct ASTnode *binexpr(void){
    struct ASTnode *n,*left, *right;
    int nodetype;


    //Get the integer literal on the left
    // Fetch the next token at the same time
    left = primary();

    //If no toke jus return left node
    if(Token.token == T_EOF) return (left);

    nodetype = arithop(Token.token);

    scan(&Token);

    right = binexpr();

    n = mkastnode(nodetype, left, right, 0);

    return n;

}
```

This code is the a AST generator for an arithmetic expression evaluator.

<ul>
<li>
    `primary` Function: Handle the parsing integer literals.
</li>
<li>
    `arithop` Function: Determine the operator type based on the token.
</li>
<li>
    `binexpr` Function: Handle the parsing of binary expressions.
</li>
</ul>

<h2>Interpreting the Tree</h2>

Now that we have out AST tree though it is not correct, let's interpret it.

pseudo-code:

```
interpretTree:
    First, interpret the left-hand sub-tree and get its value
    Then, interpret the right-hand sub-tree and get its value
    Perform the operation in the node at the root of our tree
    on the two sub-tree values, and return this value=
```

```
          +
         / \
        /   \
       /     \
      *       *
     / \     / \
    2   3   4   5
```

```
interpretTree0(tree with +):
  Call interpretTree1(left tree with *):
     Call interpretTree2(tree with 2):
       No maths operation, just return 2
     Call interpretTree3(tree with 3):
       No maths operation, just return 3
     Perform 2 * 3, return 6

  Call interpretTree1(right tree with *):
     Call interpretTree2(tree with 4):
       No maths operation, just return 4
     Call interpretTree3(tree with 5):
       No maths operation, just return 5
     Perform 4 * 5, return 20

  Perform 6 + 20, return 26
```

<h2>To Interpret The tree</h2>

```c

static char *ASTop[] = {"+", "-", "*","/"};

int interpretAST(struct ASTnode *n){
    int leftval , rightval;

    if(n->left) leftval  = interpretAST(n->left);
    if(n->right) leftval  = interpretAST(n->right);

    if(n->op == A_INTLIT) printf("int %d\n", n->intValue);
    else printf("%d %s %d\n", leftval, ASTop[n->op], rightval);

    switch (n->op)
    {
    case A_ADD:
        return leftval + rightval;
        break;
    case A_SUBTRACT:
        return leftval - rightval;
        break;
    case A_MULTIPLY:
        return leftval * rightval;
        break;
    case A_DIVIDE:
        return leftval / rightval;
        break;
    default:
        fprintf(stderr, "Unknown AST operator %d\n", n->op);
        exit(1);
    }
}
```

<h2>Building the Parser</h2>

The following code is added to the `main.c`:

```c
scan(&Token);                 // Get the first token from the input
n = binexpr();                // Parse the expression in the file
printf("%d\n", interpretAST(n));      // Calculate the final result
exit(0);
```


<h3>Conclusion</h3>
A parser recognizes the grammar/ syntax of the language and checks that the input to the compiler conforms to this grammar. It it doesn't, the parser should print out an error message.

Right now out parser works but doesn't output the value correctly.

<br><br>

<h2>Operator Precedence</h2>

Our previous code is producing the following output for the test case 2 _ 3 + 4 _ 5

```
       *
      / \
     2   +
        / \
       3   *
          / \
         4   4
```

But the correct one is

```
        +
       / \
      /   \
     /     \
    *        *
   / \      / \
  /   \    /   \
 2     3  4     5
```

To achieve this we must add code to our parser to perform operator precedence. There are (at least) two ways of doing this:

<ul>
    <li>
        Making the operator precedence explicit in the language's grammar
    </li>
    <li>
        Influencing the existing parser with an operator precedence table
    </li>
</ul>

## Pratt Parsing

I am gonna paste a GPT explanation of the Pratt Parsing and then i am gonna provide my implementation.

```
Pratt parsing is a parsing technique used in computer science for processing and interpreting expressions, particularly those involving operator precedence and associativity. It was introduced by Vaughan Pratt in 1973 as a way to efficiently parse expressions in a recursive-descent manner, with a focus on handling different levels of operator precedence without the need for explicit grammar rules for each level.


Key Concepts of Pratt Parsing:


    Nud (Null Denotation): This function is responsible for handling expressions that do not have a left-hand side (null denotation). This typically includes literals (like numbers) and prefix operators (like - for negation).

    Led (Left Denotation): This function is responsible for handling expressions that have a left-hand side, particularly when dealing with infix operators (like + or *). It processes the left-hand side and then continues parsing the right-hand side based on operator precedence.

    Precedence Levels: Pratt parsing uses precedence levels to control the parsing of operators. Operators with higher precedence are parsed first, which allows the parser to correctly interpret expressions like 3 + 4 * 5 as 3 + (4 * 5).

    Binding Power: Each operator or token is associated with a "binding power," which dictates how tightly it binds to the expressions on its left or right. This concept is used to resolve ambiguity in expressions.

How Pratt Parsing Works:


    The parser starts by reading a token.

    Depending on the type of the token (literal, operator, etc.), it will either use a nud function or a led function.

    The nud function is used when the token doesn't have a left-hand side (e.g., a number or a unary operator).

    The led function is used when the token is an infix operator that needs to combine the left-hand side with something on the right.

    The parser recursively processes the expression, respecting operator precedence and associativity rules.


Example:


Consider the expression 2 + 3 * 4. Using Pratt parsing:

    The parser sees 2, which is handled by a nud function (since it's a literal).

    Then it sees +, which is handled by a led function. The parser will then look for the next part of the expression.

    The parser sees 3 * 4. Since * has a higher precedence than +, the multiplication is processed first, resulting in 12.

    The expression is then evaluated as 2 + 12.

Advantages:

    Simplicity: It is a relatively simple way to implement parsers for languages with complex expressions, especially when compared to other parsing techniques.

    Flexibility: It can handle a wide range of grammars and can be easily extended to support new operators or precedence levels.


Pratt parsing is commonly used in interpreters and compilers, particularly for programming languages with complex operator precedence, such as arithmetic expressions in calculators or scripting languages.

```

Complete overview of new expr.c file

Function Overview

    primary():
        This function handles the lowest level of expression parsing, specifically integer literals.
        It checks if the current token (Token.token) is an integer literal (T_INTLIT). If so, it creates an AST (Abstract Syntax Tree) leaf node representing that integer literal and advances the token using scan(&Token).
        If the token is not an integer literal, it throws a syntax error.

    arithop():
        This function converts a token representing an arithmetic operator into a corresponding internal representation (e.g., T_PLUS -> A_ADD).
        If the token isn't a recognized operator, it throws an error.

    op_precedence():
        This function retrieves the precedence of an operator token from the OpPrec array. Operators with higher precedence values are executed before those with lower values.
        If the token's precedence is 0 (indicating an invalid token), the function throws an error.

    binexpr(int ptp):
        This is the core of the expression parsing, handling binary expressions based on operator precedence.
        It starts by parsing the left-hand side (LHS) of the expression using primary().
        It then checks the current token type. If it's EOF, it returns the left-hand side (as the expression is complete).
        Otherwise, it enters a loop to parse binary expressions:
            It scans for the next token and then recursively parses the right-hand side (RHS) of the expression.
            It creates an AST node using the operator token and both the left and right sides.
            The loop continues as long as operators with higher precedence than ptp are found.
        Finally, the function returns the left-hand side of the parsed expression, which would be the root of the AST for that expression.

Dry Run

Let's assume we are parsing the expression 2 + 3 \* 4. The tokens would be:

    T_INTLIT(2)
    T_PLUS
    T_INTLIT(3)
    T_STAR
    T_INTLIT(4)
    T_EOF

Initial State:

    Token.token = T_INTLIT(2)
    Line is used for error reporting, assuming Line = 1 for simplicity.

Step-by-Step Execution:

    First Call to binexpr(0):
        Call to primary():
            Token.token is T_INTLIT(2).
            primary() creates an AST leaf node for 2 (A_INTLIT).
            Token is advanced to T_PLUS.
            Returns the AST node for 2.
        tokentype is T_PLUS.
        Enters the while loop since the precedence of T_PLUS (10) is greater than ptp = 0.

    Second Call to binexpr(10):
        scan() advances the token to T_INTLIT(3).
        Call to primary():
            Token.token is T_INTLIT(3).
            primary() creates an AST leaf node for 3.
            Token is advanced to T_STAR.
            Returns the AST node for 3.
        tokentype is T_STAR.
        Enters the while loop since the precedence of T_STAR (20) is greater than ptp = 10.

    Third Call to binexpr(20):
        scan() advances the token to T_INTLIT(4).
        Call to primary():
            Token.token is T_INTLIT(4).
            primary() creates an AST leaf node for 4.
            Token is advanced to T_EOF.
            Returns the AST node for 4.
        tokentype is T_EOF.
        Exits the while loop since EOF has no precedence.
        Returns the AST node for 4.

    Returning from the Recursive Calls:
        The third call to binexpr(20) returns an AST node representing 4.
        The second call to binexpr(10) creates an AST node for A_MULTIPLY with 3 and 4 as children, and returns it.
        The first call to binexpr(0) creates an AST node for A_ADD with 2 as the left child and the A_MULTIPLY node (representing 3 * 4) as the right child.
        This final AST node, representing 2 + (3 * 4), is returned as the root of the AST for the entire expression.

Resulting AST

The final AST will represent the expression as follows:

```

    A_ADD
   /     \
  2    A_MULTIPLY
       /         \
      3           4
```

This tree structure respects operator precedence, where multiplication is evaluated before addition, ensuring the correct evaluation of the expression 2 + 3 \* 4 as 2 + 12, resulting in 14.

Let's Assume another example for expression say `2*3+4*7`. The token would be:

    T_INTLIT(2)
    T_STAR
    T_INTLIT(3)
    T_PLUS
    T_INTLIT(4)
    T_STAR
    T_INTLIT(7)
    T_EOF

Initial State:

    Token.token = T_INTLIT(2)
    Line is used for error reporting, assuming Line = 1 for simplicity.

Step-by-Step Execution

    First Call to `binexpr(0)`:
      Call to primary()
          Token.token is T_INTLIT(2)
          primary() creates an AST leaf node for 2 (A_INTLIT).
          Token is advanced to T_PLUS.
          Returns the AST node for 2.
      tokentype is T_STAR.
      Enters the while loop since the precedence of T_STAR (20) is greater than ptp = 0.


      `AST` :

          2



    Second Call to `binexpr(20)`:
      Call to primary()
        Token.token is T_INTLIT(3)
        primary() creates an AST leaf node for 3 (A_INTLIT).
        Token is advanced to T_PLUS.
        Returns the AST node for 3.
      tokentype is T_PLUS
      Exits the while loop since the precedence of T_PLUS (10) is not greater than ptp = 20.
      Returns the AST node for 3.
    Return from the Second Call:
      Creates an AST node for `A_MULTIPLY` with 2 as the left child and 3 as the right child.

      A_MULTIPLY
       /     \
      2       3

    Back to First Call `binexpr(10)`:
      `tokentype` is T_PLUS
      Enters the while loop since the precedence of T_PLUS (10) is greater than `ptp=0`

    Third Call to `binexpr(10)`:
      scan() advances the token to `T_INTLIT(4)`
      Call to primary():
        Token.token is `T_INTLIT(4)`
        primary() creates an AST leaf node for 4 (A_INTLIT).
        Token is advanced to `T_STAR`.
        Returns the AST node for 4.
      tokentype is `T_STAR`
      Enters the while loop since the precedence of `T_STAR` (20) is greater than `ptp=10`

      `AST`  :

            4

      Fourth Call to `binexpr(20)`:
      scan() advances the token to `T_INTLIT(7)`
      Call to primary():
        Token.token is `T_INTLIT(7)`
        primary() creates an AST leaf node for 7 (A_INTLIT).
        Token is advanced to `T_EOF`.
        Returns the AST node for 7.
      tokentype is `T_EOF`
      Exits the while loop since `T_EOF` has not precedence.
      Returns the AST node for 7.

      `AST`:

            7

      Returning from the fourth call:

        Creates an AST node for `A_MULTIPLY` with 4 as the left child and 7 as the right child.


        `AST`:
              A_MULTIPLY
             /          \
            4            7

        Returning from the Third Call;
          Creates an AST node for `A_ADD` with `A_MULTIPLY(2, 3)` as left child and `A_MULTIPLY` as the right child.

        `Final AST`:

                        A_ADD
                       /      \
                  A_MULTIPLY    A_MULTIPLY
                   /       \     /       \
                  2         3   4         7


## Conclusion and What's Next

It's probably time step back a bit and see where we've got to. We now have:

<ul>
<li>A scanner that recognizes and returns the tokens in our language</li>
<li>A parser that recognizes our grammar, reports syntax errors and builds an Abstract Syntax Tree</li>
<li>A precedence table for the parser that implements the semantics of our language</li>
<li>An interpreter that traverses the Abstract Syntax Tree depth-first and calculates the result of the expression in the input</li>
</ul>

<br><br>

<h2>Actual Compiler</h2>
It's about time we start making the actual thing. So let's replace the interpreter into our program that generated x86-64 assembly code.

These are the operations that are used to give assembly commands using the C. Following is the function for freeall Registers, allocate Registers, Free Register, code generateor for preamble code generator for postamble and for loading, arithmetic operations and printing.

```c

//File name cg.c
#include "defs.h"
#include "data.h"
#include "decl.h"

static int freereg[4];
static char *reglist[4]= { "%r8", "%r9", "%r10", "%r11" };

void freeall_registers(void)
{
  freereg[0]= freereg[1]= freereg[2]= freereg[3]= 1;
}

static int alloc_register(void)
{
  for (int i=0; i<4; i++) {
    if (freereg[i]) {
      freereg[i]= 0;
      return(i);
    }
  }
  fprintf(stderr, "Out of registers!\n");
  exit(1);
}
static void free_register(int reg)
{
  if (freereg[reg] != 0) {
    fprintf(stderr, "Error trying to free register %d\n", reg);
    exit(1);
  }
  freereg[reg]= 1;
}

void cgpreamble()
{
  freeall_registers();
  fputs(
	"\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
  Outfile);
}

void cgpostamble()
{
  fputs(
	"\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n",
  Outfile);
}

int cgload(int value) {

  int r= alloc_register();

  fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
  return(r);
}
int cgadd(int r1, int r2) {
  fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return(r2);
}

int cgsub(int r1, int r2) {
  fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  free_register(r2);
  return(r1);
}
int cgmul(int r1, int r2) {
  fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  free_register(r1);
  return(r2);
}
int cgdiv(int r1, int r2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  free_register(r2);
  return(r1);
}

void cgprintint(int r) {
  fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(Outfile, "\tcall\tprintint\n");
  free_register(r);
}

```

This is the code to initialize the program and to define what function to call for the current interation of the whole procedure.

```c
#include "defs.h"
#include "data.h"
#include "decl.h"

static int genAST(struct ASTnode *n) {
  int leftreg, rightreg;

  if (n->left)
    leftreg = genAST(n->left);
  if (n->right)
    rightreg = genAST(n->right);

  switch (n->op) {
    case A_ADD:
      return (cgadd(leftreg,rightreg));
    case A_SUBTRACT:
      return (cgsub(leftreg,rightreg));
    case A_MULTIPLY:
      return (cgmul(leftreg,rightreg));
    case A_DIVIDE:
      return (cgdiv(leftreg,rightreg));
    case A_INTLIT:
      return (cgload(n->intvalue));
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}

void generatecode(struct ASTnode *n) {
  int reg;

  cgpreamble();
  reg= genAST(n);
  cgprintint(reg);
  cgpostamble();
}
```

<h3>Conclusion</h3>

Changing from the interpreter to a generic code generator was trivial, but then we had to write some code to generate real assembly output. To do this, we had to think about how to allocate registers: for now, we have a naive solution.

# Let's work on getting Statements

It's time to add some "proper" statements to the grammar of our language. I want to be able to write lines of code like this:

```
print 2+3*5;
print 18-6/3+4*2;
```

Each statement starts with the keyword print and is terminated with a semicolon. So these are going to become new tokens in our language.

## BNF Description

```
statements: statement
     | statement statements
     ;

statement: 'print' expression ';'
     ;
```

An input file consists of several statements. They are either one statement, or a statement followed by more statements. Each statement starts with the keyword print, then one expression, then a semicolon.

<h3>Changes to Lexical Scanner</h3>

```c
static int scanident(int c, char *buf, int lim) {
  int i = 0;

  while (isalpha(c) || isdigit(c) || '_' == c) {
    if (lim - 1 == i) {
      printf("identifier too long on line %d\n", Line);
      exit(1);
    } else if (i < lim - 1) {
      buf[i++] = c;
    }
    c = next();
  }
  putback(c);
  buf[i] = '\0';
  return (i);
}
```

In `scan.c`, I've added this code which I've borrowed from the SubC compiler. It reads in alphanumeric characters into a buffer until it hits a non-alphanumeric character.

We also need a function to recognise keywords in the language. One way would be to have a list of keywords, and to walk the list and `strcmp()` each one against the buffer from `scanident()`. The code from SubC has an optimisation: match against the first letter before doing the `strcmp()`. This speeds up the comparison against dozens of keywords. Right now we don't need this optimisation but I've put it in for later:

```c
static int keyword(char *s) {
  switch (*s) {
    case 'p':
      if (!strcmp(s, "print"))
        return (T_PRINT);
      break;
  }
  return (0);
}
```

Now, at the bottom of the switch statement in scan(), we add this code to recognise semicolons and keywords:

```c
    case ';':
      t->token = T_SEMI;
      break;
    default:

      // If it's a digit, scan the
      // literal integer value in
      if (isdigit(c)) {
        t->intvalue = scanint(c);
        t->token = T_INTLIT;
        break;
      } else if (isalpha(c) || '_' == c) {
        // Read in a keyword or identifier
        scanident(c, Text, TEXTLEN);

        // If it's a recognised keyword, return that token
        if (tokentype = keyword(Text)) {
          t->token = tokentype;
          break;
        }
        // Not a recognised keyword, so an error for now
        printf("Unrecognised symbol %s on line %d\n", Text, Line);
        exit(1);
      }
      // The character isn't part of any recognised token, error
      printf("Unrecognised character %c on line %d\n", c, Line);
      exit(1);
```

<h3>Conclusion</h3>

We've added our first "real" statement grammar to our language. I've defined it in BNF notation, but it was easier to implement it with a loop and not recursively.

<br><br><br>

<h1>Variables: Well let's what they are haha!!</h1>
<br>

## What Do we want from Variables?

We want to be able to:

<ul>
  <li>Declare Variables</li>
  <li>Use varaibles to get stored values</li>
  <li>Assign Varables</li>
</ul>

Here is `input02` which will be our test program;

```bash
$ cat input02
int fred;
int jim;
fred = 5;
jim = 12;
print fred + jim;
```

### The Symbol Table

Every Compiler is going to need a symbol Table.

<i>
A Symbol Table is a critical data structure used in a compiler to store information about the symbols (identifiers) used in the source program. 
</i>

<b>This is GPT blabbering some definitions</b>

<b>Here is my definition</b>

<i>
Think of a symbol say a hyphen '-' now how do you know that this is a hyphen, cuz the name is never given along with the symbol. Because the name of symbol is already stored in your memory along with its name mapped accordingly.
</i>

```c
//defs.h
struct symtable
{
  char *name;
}
```

```c
//data.h
#define NSYMBOLS 1024;                  // Number of symbol table enteries
extern_ struct symtable Gsym[NSYMBOLS]; // Global symbol table
static int Globs = 0;                   // Position of next free global symbol slot
```

## Scanning and New Tokens

If you look at the example input file, we need a few tokens:

<ul>
  <li>'int' known as T_INT</li>
  <li>'=' is known as T_EQUALS</li>
  <li>identifier names, known as T_IDENT</li>
</ul>

The scanning of '=' is easy to add to scan();

```c
  case '='
    t->token = T_EQUALS; break;
```

We can add the 'int' keyword:

```c
 case 'i':
    if (!strcmp(s, "int"))
      return (T_INT);
    break;
```

## The New Grammar

We're about ready to look at the changes to the grammar of our input language. As before, I'll define it with BNF notation:

```
 statements: statement
      |      statement statements
      ;

 statement: 'print' expression ';'
      |     'int'   identifier ';'
      |     identifier '=' expression ';'
      ;

 identifier: T_IDENT
      ;
```

An identifier is returned as a T_IDENT token, and we already have the code to parse print statements. But, as we now have three types of statements, it makes sense to write a function to deal with each one. Our top-level statements() function in stmt.c now looks like:

```c
void statements(void) {

  while (1) {
    switch (Token.token) {
    case T_PRINT:
      print_statement();
      break;
    case T_INT:
      var_declaration();
      break;
    case T_IDENT:
      assignment_statement();
      break;
    case T_EOF:
      return;
    default:
      fatald("Syntax error, token", Token.token);
    }
  }
}
```

## Varaible Declaration

Let's look at variable declarations. This is in a new file, decl.c, as we are going to have lots of other types of declarations in the future.

```c
// Parse the declaration of a variable
void var_declaration(void) {

  // Ensure we have an 'int' token followed by an identifier
  // and a semicolon. Text now has the identifier's name.
  // Add it as a known identifier
  match(T_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}
```

The ident() and semi() functions are wrappers around match():

```c
void semi(void)  { match(T_SEMI, ";"); }
void ident(void) { match(T_IDENT, "identifier"); }
```

## Assignment Statements

```c
void assignment_statement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  ident();

  if ((id = findglob(Text)) == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, id);

  match(T_EQUALS, "=");

  left = binexpr(0);

  tree = mkastnode(A_ASSIGN, left, right, 0);

  genAST(tree, -1);
  genfreeregs();

  semi();
}
```

We have a couple of new AST node types. A_ASSIGN takes the expression in the left-hand child and assigns it to the right-hand child. And the right-hand child will be an A_LVIDENT node.

Why did I call this node A_LVIDENT? Because it represents an lvalue identifier. So what's an lvalue?

An lvalue is a value that is tied to a specific location. Here, it's the address in memory which holds a variable's value.

we assign the result of the right-hand side (i.e. the rvalue) to the variable in the left-hand side (i.e. the lvalue). The rvalue isn't tied to a specific location.

## Changes to the AST Structure

```c
struct ASTnode
{
  int op;
  struct ASTnode *left;
  struct ASTnode *right;
  union
  {
    int intvalue;
    int id;
  } v;
};

```

## Generating the Assignment Code

```c
int genAST(struct ASTnode *n, int reg) {
  int leftreg, rightreg;

  if (n->left)
    leftreg = genAST(n->left, -1);
  if (n->right)
    rightreg = genAST(n->right, leftreg);

  switch (n->op) {
  // already exising code
    case A_INTLIT:
    return (cgloadint(n->v.intvalue));
  case A_IDENT:
    return (cgloadglob(Gsym[n->v.id].name));
  case A_LVIDENT:
    return (cgstorglob(reg, Gsym[n->v.id].name));
  case A_ASSIGN:
    return (rightreg);
  default:
    fatald("Unknown AST operator", n->op);
  }

```

Note that we evaluate the left-hand AST child first, and we get back a register number that holds the left-hand sub-tree's value. We now pass this register number to the right-hand sub-tree. We need to do this for A_LVIDENT nodes, so that the cgstorglob() function in cg.c knows which register holds the rvalue result of the assignment expression.

So, consider this AST tree:

```
           A_ASSIGN
          /        \
     A_INTLIT   A_LVIDENT
        (3)        (5)
```

## Generating x86-64 Code

You would have noticed that I changed the name of the old cgload() function to cgloadint(). This is more specific. We now have a function to load the value out of a global variable (in cg.c):

```c
int cgloadglob(char *identifier) {
  // Get a new register
  int r = alloc_register();

  // Print out the code to initialise it
  fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reglist[r]);
  return (r);
}
```

Similarly, we need a function to save a register into a variable:

```c
// Store a register's value into a variable
int cgstorglob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return (r);
}
```

We also need a function to create a new global integer variable:

```c
// Generate a global symbol
void cgglobsym(char *sym) {
  fprintf(Outfile, "\t.comm\t%s,8,8\n", sym);
}
```

Of course, we can't let the parser access this code directly. Instead, there is a function in the generic code generator in gen.c that acts as the interface:

```c
void genglobsym(char *s) { cgglobsym(s); }
```

## Variables in Expressions

So now we can assign to variables. But how do we get a variable's value into an expression. Well, we already have a primary() function to get an integer literal. Let's modify it to also load a variable's value:

```c

static struct ASTnode *primary(void)
{
  struct ASTnode *n;
  int id;

  switch (Token.token)
  {
  case T_INTLIT:
    n = mkastleaf(A_INTLIT, Token.intvalue);
    break;
  case T_IDENT:
    id = findglob(Text);
    if (id == -1)
      fatals("Unknown Variable", Text);

    n = mkastleaf(A_IDENT, id);
    break;
  default:
    fatald("Syntax error, token", Token.token);
  }

  scan(&Token);
  return (n);
}
```

Note the syntax checking in the T_IDENT case to ensure the variable has been declared before we try to use it.

Also note that the AST leaf node that retrieves a variable's value is an A_IDENT node. The leaf that saves into a variable is an A_LVIDENT node. This is the difference between rvalues and lvalues.

## Other Changes

```c
// Print out fatal messages
void fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, Line); exit(1);
}

void fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line); exit(1);
}

void fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, Line); exit(1);
}

void fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, Line); exit(1);
}
```

## Trying it out

```bash
$ make testall
make -s hulacomp
cc -o test1.out main.c -L. hulacomp.a
./test1.out input01
cc -o out out.s
./out
36
10
25
./test1.out input02
cc -o out out.s
./out
17
./test1.out input03
cc -o out out.s
./out
1
2
3
4
5
```


## Conclusion

So that was a lot of work. We had to write the beginnings of symbol table management. We had to deal with two new statement types. We had to add some new tokens and some new AST node types. Finally, we had to add some code to generate the correct x86-64 assembly output.

Try writing a few example input files and see if the compiler works as it should, especially if it detects syntax errors and semantic errors (variable use without a declaration).