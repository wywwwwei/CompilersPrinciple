#pragma once
#include<string>

#define KEYWORD_SIZE 12
#define DELIM_SIZE 4
#define OPT_SIZE 11


typedef enum  {
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

typedef struct node{
	tokenType type;
	std::string content;
	int row;
	int col;

	node(tokenType _type, std::string str,int _row,int _column) {
		type = _type;
		content = str;
		row = _row;
		col = _column;
	}
}tokennode;

class Token {
public:
	std::string keyword[KEYWORD_SIZE] = { "IF","ELSE","WHILE","WRITE", "READ", "RETURN", "BEGIN", "END", "MAIN", "INT", "REAL","STRING"};
	std::string operation[OPT_SIZE] = { "+","-","*","/","==",":=","!=","<",">","<=",">=" };
	std::string delimeter[DELIM_SIZE] = { ";",",","(",")" };
	tokenType getToken(std::string lexeme);
	std::string getTokenStr(tokenType token);
private:
};