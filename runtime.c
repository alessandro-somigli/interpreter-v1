#include"ast.h"

void eval(struct AST* ast) {
	switch (ast->tag) {
		case PROC:
		case NUM_LIT:
		case STR_LIT:
		case BIN_EXPR:
		case UN_EXPR:
		case IDENT:
		case CALL_EXPR:
		case FUNC_DEC:
		default:
			raise_err("Error - Unrecognized Node: unrecognized AST node");
	}
}
