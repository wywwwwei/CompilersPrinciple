#include"token.h"

tokenType Token::getToken(std::string lexeme) {
	for (int i = 0; i < KEYWORD_SIZE; i++) {
		if (lexeme == keyword[i]) {
			return tokenType(i);
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

std::string Token::getTokenStr(tokenType token) {
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