#include "defs.h"
#include "decl.h"
#include "data.h"

void var_declaration(void)
{
    match(T_INT, "int");
    ident();
    addglob(Text);
    genglobsym(Text);
    semi();
}