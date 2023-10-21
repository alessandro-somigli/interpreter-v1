struct AST {
    enum {          // AST node statement type
        PROC,       // procedures
        NUM_LIT,    // numeric literal
		STR_LIT,	// string literal
        BIN_EXPR,   // binary expressions
        UN_EXPR,    // unary expressions
        CALL_EXPR,  // expression call
        IDENT,      // identifier
        FUNC_DEC,   // function declaration
    } tag;
    union {
        // procedures are arrays of instructions
        struct {
        	struct AST** arr; // array of pointers
        	int max_size;
        	int size;
        } PROC;
        // numeric literals are just numbers
        struct { // NUM_LIT
        	enum {
        		INTEGER,
				FLOAT
        	} type;
            union {
                int vint;
                double vfloat;
            } value;
        } NUM_LIT;
        // string literals are arrays of chars
        struct { char* str; } STR_LIT;
        // binary expressions are expressions with 1 operator and two operands (2 + 2)
        struct { // BIN_EXPR
            struct AST* vleft;
            struct AST* vright;
            char operand;
        } BIN_EXPR;
        // unary expressions are expressions with 1 operator and 1 operand (!true)
        struct { // UN_EXPR
            struct AST* value;
            char operand;
        } UN_EXPR;
        // identifiers are variable names or keywords
        struct { // IDENT
            char* symbol;
        } IDENT;

    } data;
};

struct AST* get_ast();

void free_ast(struct AST* ast);
