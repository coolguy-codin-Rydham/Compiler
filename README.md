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

<h2>Test Cases</h2>

The 4 files input01, input02, input03, input04 can be used to see what the tokens the scanner finds out after parsing each file, and what input files the scanner. Edge cases also exist.

```bash
$ gcc -o scanner -g main.c scan.c

$ cat input01
2 + 4 * 5 - 8 / 3

$ cat input04
23 +
18 -
45.6 * 2
/   19

$ ./scanner input01
Token intlit, value 2
Token +
Token intlit, value 4
Token *
Token intlit, value 5
Token -
Token intlit, value 8
Token /
Token intlit, value 3

$ ./scanner input04
Token intlit, value 23
Token +
Token intlit, value 18
Token -
Token intlit, value 45
Unrecognized character . on line 3

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

Output from running the parser

```bash
$ gcc -o parser -g expr.c Interp.c main.c scan.c tree.c 

$ cat input01
2 + 4 * 5 - 8 / 3

$ ./parser input01
int 2
int 4
int 5
int 8
int 3
8 / 3
5 - 2
4 * 3
2 + 12
14

$ cat input02
12 34 + -56 * / - - 8 + * 2

$ ./parser input02
unknown token in arithop() on line 1

$ cat input03
13 -6+  4*
5
       +
08 / 3

./parser input03
int 13
int 6
int 4
int 5
int 8
int 3
8 / 3
5 + 2
4 * 7
6 + 28
13 - 34
-21

$ cat input04
23 +
18 -
45.6 * 2

$ ./parser input04
Unrecognised character . on line 3

$ cat input05
23 * 456abcdefg

$ ./parser input05
Unrecognised character a on line 1

```

<h2>Conclusion</h2>
A parser recognizes the grammar/ syntax of the language and checks that the input to the compiler conforms to this grammar. It it doesn't, the parser should print out an error message.

Right now out parser works but doesn't output the value correctly.