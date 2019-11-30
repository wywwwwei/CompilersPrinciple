#pragma once
#include "lexicalAnalyzer.h"
#include<stdbool.h>
#define NODE_STORE 3

struct SyntaxAnalyzer {
	struct lexicalAnalyzer* input;
	tokennode store[NODE_STORE];
	int curIndex;
	int curDepth;
	bool notCreate;
};

struct SyntaxAnalyzer* SyntaxAnalyzer_struct(char* filePath);
void SyntaxAnalyzer_destruct(struct SyntaxAnalyzer* sa);
void analyse(struct SyntaxAnalyzer* sa);
void logSyntaxError(struct SyntaxAnalyzer* sa, const char* errMsg);
bool compareContent(tokennode tempnode, const char* cmp);
bool compareType(tokennode tempnode, tokenType cmp);
void printTerminal(struct SyntaxAnalyzer* sa);
void printNonterminal(struct SyntaxAnalyzer* sa, const char* str);
tokennode getTokennode(struct SyntaxAnalyzer* sa, int offset);
//High-level program structure
void program(struct SyntaxAnalyzer* sa);
void methodDecl(struct SyntaxAnalyzer* sa);
void type(struct SyntaxAnalyzer* sa);
void identifier(struct SyntaxAnalyzer* sa);
void formalParams(struct SyntaxAnalyzer* sa);
void formalParam(struct SyntaxAnalyzer* sa);
//statements
void block(struct SyntaxAnalyzer* sa);
void statement(struct SyntaxAnalyzer* sa);
void localVarDecl(struct SyntaxAnalyzer* sa);
void assignStmt(struct SyntaxAnalyzer* sa);
void returnStmt(struct SyntaxAnalyzer* sa);
void ifStmt(struct SyntaxAnalyzer* sa);
void whileStmt(struct SyntaxAnalyzer* sa);
void writeStmt(struct SyntaxAnalyzer* sa);
void readStmt(struct SyntaxAnalyzer* sa);
//expression
void expression(struct SyntaxAnalyzer* sa);
void multiplicativeExpr(struct SyntaxAnalyzer* sa);
void primaryExpr(struct SyntaxAnalyzer* sa);
void boolExpression(struct SyntaxAnalyzer* sa);
void actualParams(struct SyntaxAnalyzer* sa);