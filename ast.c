#include<malloc.h>

#include"ast.h"

struct AST* get_ast() {
	return (struct AST*)malloc(sizeof(struct AST));
}

void free_ast(struct AST* ast) {
	// free recursively here
}
