#pragma once
#include<string.h>

#define KEYWORD_SIZE 12
#define DELIM_SIZE 4
#define OPT_SIZE 11
#define EACH_LEN 256

typedef enum {
	//keywords
	IF,
	ELSE,
	WHILE,
	WRITE,
	READ,
	RETURN,
	BEGIN,
	END,
	MAIN,
	INT,
	REAL,
	STRING,

	IDENTIFIER,
	//Operator such as + - * / == := !=
	Operator,
	//Delimeter such as ; , ( )
	Delimeter,
	Number,
	String
}tokenType;

typedef struct node {
	tokenType type;
	char content[EACH_LEN];
	int row;
	int col;
}tokennode;

tokenType getToken(char* lexeme);
char* getTokenStr(tokenType token);


extern const char* keyword[KEYWORD_SIZE];
extern const char* operation[OPT_SIZE];
extern const char* delimeter[DELIM_SIZE];