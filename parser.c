#include<malloc.h>

#include"ast.h"
#include"lexer.h"

#define ISIZE_MAIN_PROC 1024



struct AST parse_expr(struct Token* tokens) {

}

struct AST parse_stmt(struct Token* tokens, int* i_tokens) {
	// nothing to do yet I guess, go to parse expr
	return parse_expr(tokens);
}

struct AST makeAST(FILE *file) {
    // get tokens
    struct Token* tokens = tokenize(file);
    int i_tokens = 0;

    // create parent procedure
    struct AST root;
    root.tag = PROC;
    root.data.PROC.arr = (struct AST*)malloc(ISIZE_MAIN_PROC * sizeof(struct AST));
    root.data.PROC.size = ISIZE_MAIN_PROC;

    /*
    while (tokens[i_tokens].type != T_EOF) {
    	root.data.PROC.arr[i_tokens] = parse_stmt();

        i_tokens++;
    }*/

    // after AST is generated
    free(tokens);

    // remember: string literals inside AST are allocated dynamically!!!

    return root;
}


