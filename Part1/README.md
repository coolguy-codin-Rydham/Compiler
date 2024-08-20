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

To read and silently skip whitespace characters until it gets to a non-whitespace character and also return it.

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
