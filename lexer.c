#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<ctype.h>
#include <string.h>

#include"lexer.h"
#include"util.h"

/* extracts an identifier token from the file stream starting with a given character.
 * returns the extracted Token struct representing the identifier token.
 */
struct Token get_ident_token(FILE *file, char ch) {
    struct Token token;
    token.type = T_VAR;

    char word_str[64] = {ch};
    int i_word = 1;
    while ((ch = fgetc(file)) != EOF && (isalnum(ch) || ch == '_'))
        word_str[i_word++] = ch;
    ungetc(ch, file);

    // check all keywords
    if (strcmp(word_str, "let") == 0) {
        token.type = T_LET;
    }

    return token;
}

/* extracts a string token from the file stream enclosed in double quotes.
 * returns the extracted Token struct representing the string token.
 */
struct Token get_str_token(FILE *file) {
	struct Token token;
	token.type = T_STR;

	char *str = (char*)malloc(ISIZE_STR); // char length is 1byte
	int ch, i_str = 0, l_str = ISIZE_STR;

	while ((ch = fgetc(file)) != EOF && ch != '"') {
		if (++i_str == l_str) {
			l_str <<= 1; // effectively l_str * 2
			str = (char*)realloc(str, l_str);
		}
		str[i_str] = ch;
	}

	str[i_str] = '\0';
	token.value.vstr = str;

	return token;
}

/* extracts a numeric token the file stream and determines its type (integer or float).
 * returns the extracted Token struct representing the number token.
 */
struct Token get_num_token(FILE *file, char ch) {
    struct Token token;
    token.type = T_INTEGER;

    char num_str[12] = {ch};
    int i_num = 1;
    while ((ch = fgetc(file)) != EOF &&  (isdigit(ch) || ch == '.')) {
	    if (ch == '.') token.type = T_FLOAT;
        num_str[i_num++] = ch;
	} ungetc(ch, file);

    if (token.type == T_INTEGER) {
        token.value.vint = atoi(num_str);
    } else {
        token.value.vfloat = atof(num_str);
    }

    return token;
}

// take in: file_ptr and token_arr
// do: fill token_arr
// return: size of token_arr
struct Token* tokenize(FILE *file) {
	struct Token* tokens = (struct Token*)malloc(ISIZE_TOKEN_ARR * sizeof(struct Token));
    struct Token token;
    int i_tokens = 0, s_tokens = ISIZE_TOKEN_ARR;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        switch (ch) {
            case ' ':
    		case '\t'...'\r': // horizontal tab (\t), newline (\n), vertical tab (\v), formfeed (\f), carriage line (\r)
    			break;
			case '0'...'9':
                token = get_num_token(file, ch);
                tokens[i_tokens++] = token;
				break;
            case 'A'...'Z':
            case 'a'...'z':
            case '_':
                token = get_ident_token(file, ch);
                tokens[i_tokens++] = token;
                break;
            case '"':
            	token = get_str_token(file);
            	tokens[i_tokens++] = token;
            	break;
			case '+':
				token.type = T_ADD;
				tokens[i_tokens++] = token;
				break;
			case '-':
				token.type = T_SUB;
				tokens[i_tokens++] = token;
				break;
			case '*':
				token.type = T_MUL;
				tokens[i_tokens++] = token;
				break;
			case '/':
				token.type = T_DIV;
				tokens[i_tokens++] = token;
				break;
            case '%':
                token.type = T_MOD;
                tokens[i_tokens++] = token;
                break;
			case '(':
				token.type = T_LPAREN;
				tokens[i_tokens++] = token;
				break;
			case ')':
				token.type = T_RPAREN;
				tokens[i_tokens++] = token;
				break;
            case '[':
                token.type = T_LSQUARE;
                tokens[i_tokens++] = token;
                break;
            case ']':
                token.type = T_RSQUARE;
                tokens[i_tokens++] = token;
                break;
            case '{':
                token.type = T_LBRACE;
                tokens[i_tokens++] = token;
                break;
            case '}':
                token.type = T_RBRACE;
                tokens[i_tokens++] = token;
                break;
			case ';':
				token.type = T_BREAK;
				tokens[i_tokens++] = token;
				break;
			default:
	            raise_err("Error - Unrecognized Character: unrecognized character found in source: %c", ch);
        }

        if (i_tokens == s_tokens) {
        	s_tokens <<= 1; // s_tokens * 2
        	tokens = (struct Token*)realloc(tokens, s_tokens * sizeof(struct Token));
        }
    }

    struct Token eof_token = { T_EOF, -1 };
    tokens[i_tokens] = eof_token;

    return tokens;
}
