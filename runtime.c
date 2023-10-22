#include"ast.h"
#include"util.h"
#include"runtime.h"

struct RunVal numeric_expr_eval(struct RunVal lval, struct RunVal rval, char op) {
	struct RunVal res;
	double ln, rn;

	if (lval.type == RV_FLOAT || rval.type == RV_FLOAT) {
		res.type = RV_FLOAT;
		ln = lval.type==RV_INTEGER? lval.value.vint : lval.value.vfloat; // cast to float
		rn = rval.type==RV_INTEGER? rval.value.vint : rval.value.vfloat;
	} else {
		res.type = RV_INTEGER;
		ln = lval.value.vint;
		rn = rval.value.vint;
	}

	switch (op) {
		case '+':
			if (res.type == RV_INTEGER)
				res.value.vint = (int)(ln + rn);
			else res.value.vfloat = ln + rn;
			break;
		case '-':
			if (res.type == RV_INTEGER)
				res.value.vint = (int)(ln - rn);
			else res.value.vfloat = ln - rn;
			break;
		case '*':
			if (res.type == RV_INTEGER)
				res.value.vint = (int)(ln * rn);
			else res.value.vfloat = ln * rn;
			break;
		case '/':
			if (res.type == RV_INTEGER) // add checks for / & % by 0
				res.value.vint = (int)(ln / rn);
			else res.value.vfloat = ln / rn;
			break;
		case '%':
			if (res.type == RV_INTEGER)
				res.value.vint = (int)ln % (int)rn; // modulo is only for integers so cast
			else res.value.vfloat = (int)ln % (int)rn; // i'm not using fmod it leads to undefined behaviour
			break;
		default:
			raise_err("Error - Unknown Operator: unknown integer operation");
	}

	return res;
}

struct RunVal bin_expr_eval(struct AST* ast) {
	struct RunVal lval = eval(ast->data.BIN_EXPR.vleft);
	struct RunVal rval = eval(ast->data.BIN_EXPR.vright);
	char op = ast->data.BIN_EXPR.operand;

	struct RunVal val;

	if (lval.type == RV_STRING || rval.type == RV_STRING) { // handle string results

	} else val = numeric_expr_eval(lval, rval, op);

	return val;
}

struct RunVal eval(struct AST* ast) {
	struct RunVal val;

	val.type = RV_INTEGER; // default return value
	val.value.vint = 0;

	switch (ast->tag) {
		case PROC:
			for (int i=0; i<ast->data.PROC.size; i++)
				val = eval(ast->data.PROC.arr[i]); // until return statements take the last value
			break;
		case NUM_LIT:
			if (ast->data.NUM_LIT.type == AST_INTEGER) {
				val.type = RV_INTEGER;
				val.value.vint = ast->data.NUM_LIT.value.vint;
			} else {
				val.type = RV_FLOAT;
				val.value.vfloat = ast->data.NUM_LIT.value.vfloat;
			}
			return val;
		case STR_LIT:
			val.type = RV_STRING;
			val.value.vstring = ast->data.STR_LIT.str;
			break;
		case BIN_EXPR:
			return bin_expr_eval(ast);
			break;
		case UN_EXPR:
		case IDENT:
		case CALL_EXPR:
		case FUNC_DEC:
		default:
			raise_err("Error - Unrecognized Node: unrecognized AST node");
	}

	return val;
}

