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