#include"token.h"

 const char* keyword[KEYWORD_SIZE] = { "IF","ELSE","WHILE","WRITE", "READ", "RETURN", "BEGIN", "END", "MAIN", "INT", "REAL","STRING" };
 const char* operation[OPT_SIZE] = { "+","-","*","/","==",":=","!=","<",">","<=",">=" };
 const char* delimeter[DELIM_SIZE] = { ";",",","(",")" };

tokenType getToken(char* lexeme) {
	for (int i = 0; i < KEYWORD_SIZE; i++) {
		if (strcmp(lexeme, keyword[i]) == 0) {
			return i;
		}
	}
	for (int i = 0; i < OPT_SIZE; i++) {
		if (lexeme == operation[i]) {
			return Operator;
		}
	}
	for (int i = 0; i < DELIM_SIZE; i++) {
		if (lexeme == delimeter[i]) {
			return Delimeter;
		}
	}
	return IDENTIFIER;
}

char* getTokenStr(tokenType token) {
	if (token == IDENTIFIER) {
		return "IDENTIFIER";
	}
	if (token == Operator) {
		return "Operator";
	}
	if (token == Delimeter) {
		return "Delimeter";
	}
	if (token == Number) {
		return "Number";
	}
	if (token == String) {
		return "String";
	}
	return "Keyword";
}