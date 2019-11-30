#define _CRT_SECURE_NO_WARNINGS
#include"lexicalAnalyzer.h"
#include<stdlib.h>
#include<stdbool.h>

struct lexicalAnalyzer* lexicalAnalyzer_struct(char* filePath)
{
	struct lexicalAnalyzer *la = malloc(sizeof(struct lexicalAnalyzer));
	la->input = scan_struct(filePath);
	return la;
}
void lexicalAnalyzer_destruct(struct lexicalAnalyzer* la)
{
	scan_destruct(la->input);
	free(la);
}

void logError(struct lexicalAnalyzer* la, const char* errMsg)
{
	printf("Row %d Column %d :%s\n", getRow(la->input), getCol(la->input), errMsg);
	lexicalAnalyzer_destruct(la);
	exit(1);
}

tokennode curToken(struct lexicalAnalyzer* la, tokenType _token)
{
	tokennode node;
	node.type = _token;
	strcpy(node.content, getString(la->input));
	node.row = getRow(la->input);
	node.col = getCol(la->input);
	nextLexeme(la->input);
	return node;
}

tokennode lexical_analyse(struct lexicalAnalyzer* la)
{
	char current = '\0';
	while (current = nextChar(la->input)) {
		if (current == '\0')break;

		if ((current >= 'a'&&current <= 'z') || (current >= 'A'&&current <= 'Z') || current == '_') {
			while (current = curChar(la->input)) {
				if ((current >= 'a'&&current <= 'z') || (current >= '0'&&current <= '9') || (current >= 'A'&&current <= 'Z') || current == '_')
				{
					nextChar(la->input);
				}
				else {
					return curToken(la,getToken(getString(la->input)));
				}
			}
		}

		char lastChar;
		bool real = false;
		switch (current)
		{
		case' ':
		case'\t':
			break;
		case'=':
			if (nextChar(la->input) == '=') {
				return curToken(la, Operator);
			}
			logError(la, "Unrecognizable characters.");
		case':':
			if (nextChar(la->input) == '=') {
				return curToken(la,Operator);
			}
			logError(la, "Unrecognizable characters.");
		case '!':
			if (nextChar(la->input) == '=') {
				return curToken(la, Operator);
			}
			logError(la, "Unrecognizable characters.");
		case ';':
		case',':
		case '(':
		case ')':
			return curToken(la,Delimeter);
		case '+':
		case '-':
		case '*':
			return curToken(la,Operator);
		case '/':
			if (nextChar(la->input) != '*') {
				return curToken(la,Operator);
			}
			if (nextChar(la->input) != '*') {
				logError(la, "Unrecognizable characters.");
			}
			while (lastChar = nextChar(la->input)) {
				if (lastChar == '*') {
					lastChar = nextChar(la->input);
					if (lastChar == '*') {
						lastChar = nextChar(la->input);
						if (lastChar == '/') {
							break;
						}
					}
				}
			}
			break;
		case '<':
		case '>':
			if (curChar(la->input) == '=') {
				nextChar(la->input);
				return curToken(la,Operator);
			}
			else {
				return curToken(la,Operator);
			}
		case  '0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			while (lastChar = curChar(la->input)) {
				if (lastChar == '.') {
					if (real) {
						logError(la, "Unrecognizable characters.");
					}
					real = true;
					nextChar(la->input);
					continue;
				}
				else if (lastChar >= '0' && lastChar <= '9') {
					nextChar(la->input);
					continue;
				}
				else {
					if (real) {
						return curToken(la,Number);
					}
					else {
						return curToken(la,Number);
					}
					break;
				}
			}
			break;
		case'"':
			while (lastChar = nextChar(la->input)) {
				if (lastChar == '"') {
					return curToken(la,String);
					break;
				}
			}
			break;
		default:
			logError(la, "Unrecognizable characters.");
		}
		nextLexeme(la->input);
	}
	tokennode temp;
	temp.type = -1;
	return temp;
}
