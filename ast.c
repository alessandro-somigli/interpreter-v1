#include<malloc.h>

#include"ast.h"

struct AST* getAST() {
	return (struct AST*)malloc(sizeof(struct AST));
}

void freeAST(struct AST* ast) {
	// free recursively here
}
