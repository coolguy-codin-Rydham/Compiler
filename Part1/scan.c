#include "defs.h"
#include "data.h"
#include "decl.h"


/*This function returns the 
first occurrence of the character c
int string s, or -1 if c is not found
*/
static int chrpos(char *s, int c)
{
    const char *p;

    p = strchr(s,c);
    return (p ? p-s:-1);
}

// Get the next character from the input field

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

// Put back an unwanted character

static void putback(int c)
{
    Putback = c;  
}

/* Function to skip whitespaces
    i.e. whitespace, newlines. Return the first
    non-whitespace character encountered
*/
static int skip(void){
    int c;

    c=next();

    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    } 
    return (c);
    
}

int scan(struct token *t){
    int c;

    c = skip();

    switch(c){
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

    }
}

int main(){

    char *hello = "hello, world";
    int a = chrpos(hello, 'o');
    printf("%d", a);
    return 0;
}