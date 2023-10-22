#include<malloc.h>

#include"ast.h"
#include"lexer.h"
#include"util.h"

#define ISIZE_MAIN_PROC 1024

struct AST* parse_stmt(struct Token* tokens, int* i_tokens);
struct AST* parse_expr(struct Token* tokens, int* i_tokens);
struct AST* parse_additive_expr(struct Token* tokens, int* i_tokens);
struct AST* parse_multiplicative_expr(struct Token* tokens, int* i_tokens);
struct AST* parse_primary_expr(struct Token* tokens, int* i_tokens);

struct AST* parse_primary_expr(struct Token* tokens, int* i_tokens) {
	struct Token token = tokens[*i_tokens];
	struct AST* expr = get_ast();

	// parse primary expression
	switch (token.type) {
		case T_IDENT: // handle variables
			expr->tag = IDENT;
			expr->data.IDENT.symbol = token.value.vstr;
			(*i_tokens)++;
			break;
		case T_INTEGER: // handle num literals
			expr->tag = NUM_LIT;
			expr->data.NUM_LIT.type = AST_INTEGER;
			expr->data.NUM_LIT.value.vint = token.value.vint;
			(*i_tokens)++;
			break;
		case T_FLOAT: // handle num literals
			expr->tag = NUM_LIT;
			expr->data.NUM_LIT.type = AST_FLOAT;
			expr->data.NUM_LIT.value.vfloat = token.value.vfloat;
			(*i_tokens)++;
			break;
		case T_LPAREN:
			(*i_tokens)++; // eat opening parens
			expr = parse_expr(tokens, i_tokens);
			if (tokens[*i_tokens].type != T_RPAREN)
			  raise_err("Error - Missing Closing Paren: closing parenthesis was not found");
			else (*i_tokens)++;
			break;

		default:
			raise_err("Error - Unexpected Token: unrecognized token found while parsing: %d", token.type);
	}

	return expr;
}

struct AST* parse_multiplicative_expr(struct Token* tokens, int* i_tokens) {
	struct AST* left = get_ast();
	left = parse_primary_expr(tokens, i_tokens);

	while (tokens[*i_tokens].type == T_MUL || tokens[*i_tokens].type == T_DIV || tokens[*i_tokens].type == T_MOD) {
		struct AST* bin_op = get_ast();
		enum TokenType t = tokens[(*i_tokens)++].type;
		bin_op->tag = BIN_EXPR; 		// careful: eat token here
		bin_op->data.BIN_EXPR.operand = t==T_MUL? '*':t==T_DIV? '/':'%';
		bin_op->data.BIN_EXPR.vleft = left;
		bin_op->data.BIN_EXPR.vright = parse_primary_expr(tokens, i_tokens);

		left = bin_op;
	}

	return left;
}

struct AST* parse_additive_expr(struct Token* tokens, int* i_tokens) {
	struct AST* left = get_ast();
	left = parse_multiplicative_expr(tokens, i_tokens);

	while (tokens[*i_tokens].type == T_ADD || tokens[*i_tokens].type == T_SUB) {
		struct AST* bin_op = get_ast();
		bin_op->tag = BIN_EXPR;
		bin_op->data.BIN_EXPR.operand = tokens[(*i_tokens)++].type==T_ADD? '+':'-';
		bin_op->data.BIN_EXPR.vleft = left;
		bin_op->data.BIN_EXPR.vright = parse_multiplicative_expr(tokens, i_tokens);

		left = bin_op;
	}

	return left;
}

struct AST* parse_expr(struct Token* tokens, int* i_tokens) {
	return parse_additive_expr(tokens, i_tokens);
}

struct AST* parse_stmt(struct Token* tokens, int* i_tokens) {
	// nothing to do yet, skip to parse_expr

	return parse_expr(tokens, i_tokens);
}

struct AST* make_ast(FILE* file) {
    struct Token* tokens = tokenize(file);

    // create the root procedure
    struct AST* root = get_ast();
    root->tag = PROC;
    root->data.PROC.arr = (struct AST**)malloc(ISIZE_MAIN_PROC * sizeof(struct AST*));

    int i_tokens = 0, i_proc = 0, s_proc = ISIZE_MAIN_PROC;

    while (tokens[i_tokens].type != T_EOF) {
    	root->data.PROC.arr[i_proc++] = parse_stmt(tokens, &i_tokens);

    	if (s_proc == i_proc) { // dynamic resizing of main PROC
    		s_proc <<= 1;
    		root->data.PROC.arr = (struct AST**)realloc(root->data.PROC.arr, s_proc * sizeof(struct AST*));
    	}

    }

    root->data.PROC.size = i_proc;

    // free tokens after generating AST
    free(tokens);

    return root;
}

/* order of precedence:
 * primary expr
 * unary expr
 * multiplicative expr | done
 * additive expr | done
 * comparative expr
 * logical expr
 * function call
 * member expr
 * assignment expr | done
 * */


