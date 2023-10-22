struct RunVal eval(struct AST*);

struct RunVal {
	enum {
		RV_INTEGER,
		RV_FLOAT,
		RV_STRING
	} type;
	union {
		int vint;
		double vfloat;
		char* vstring;
	} value;
};
