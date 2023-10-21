#include<malloc.h>

#include"ast.h"
#include"util.h"

struct AST* get_ast() {
	return (struct AST*)malloc(sizeof(struct AST));
}

void free_ast(struct AST* ast) {
	switch (ast->tag) {
		case PROC:
			for (int i=0; i<ast->data.PROC.size; i++)
				free_ast(ast->data.PROC.arr[i]);
			free(ast->data.PROC.arr);
			break;
		case NUM_LIT:
		case STR_LIT:
			break;
		case BIN_EXPR:
			free_ast(ast->data.BIN_EXPR.vleft);
			free_ast(ast->data.BIN_EXPR.vright);
			break;
		case UN_EXPR:
			free_ast(ast->data.UN_EXPR.value);
			break;
		case IDENT:
			free(ast->data.IDENT.symbol);
			break;
		case CALL_EXPR:
		case FUNC_DEC:
		default:
			raise_err("Error - Unrecognized Node: unrecognized AST node");
	}

	free(ast);
}
