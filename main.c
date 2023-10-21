#include <stdio.h>
#include <stdlib.h>

#include"parser.h"
#include"ast.h"
#include"util.h"

void print_depth(int depth) {
	for (int i=0; i<depth; i++) printf("  ");
}

void print_ast(struct AST* root, int depth) {
	print_depth(depth);

	switch(root->tag) {
		case FUNC_DEC:
		case CALL_EXPR:
			break;
		case PROC:
			printf("proc:\n");
			int i;
			for (i=0; i<root->data.PROC.size; i++) {
				print_ast(root->data.PROC.arr[i], (depth+1));
				printf("\n"); // end of instruction
			}
			break;
		case STR_LIT:
			printf("str_lit: %s\n", root->data.STR_LIT.str);
			break;
		case NUM_LIT:
			if (root->data.NUM_LIT.type == INTEGER)
				printf("num_lit: %d\n", root->data.NUM_LIT.value.vint);
			else
				printf("num_lit: %f\n", root->data.NUM_LIT.value.vfloat);
			break;
		case BIN_EXPR:
			printf("bin_expr:\n");
			print_ast(root->data.BIN_EXPR.vleft, (depth+1));

			print_depth(depth+1);
			printf("%c \n", root->data.BIN_EXPR.operand);

			print_ast(root->data.BIN_EXPR.vright, (depth+1));
			break;
		case UN_EXPR:
			printf("un_expr: %c\n", root->data.UN_EXPR.operand);
			print_ast(root->data.UN_EXPR.value, (depth+1));
			break;
		case IDENT:
			printf("ident: %s\n", root->data.IDENT.symbol);
			break;
	}
}

int main(int argc, char *argv[]) {
	// handle args errors
	if (argc < 2)
		raise_err("No Args: required file path.");
	else if (argc > 2)
		raise_err("Too Many Args: the only argument should be the file path.");

	// open file and handle file not found
    FILE* file = fopen(argv[1], "r");

    if (file == NULL)
    	raise_err("File Not Found: the path does not lead to any file");

    struct AST ast = make_ast(file);

    print_ast(&ast, 0);

    printf("finished.");

	return EXIT_SUCCESS;
}
