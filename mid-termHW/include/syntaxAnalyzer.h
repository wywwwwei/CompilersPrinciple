#pragma once
#include "lexicalAnalyzer.h"

class SyntaxAnalyzer {
public:
	explicit SyntaxAnalyzer(std::string filePath);
	void analyse();
	void logError(std::string errMsg);
	bool compareContent(tokennode* tempnode, std::string cmp);
	bool compareType(tokennode* tempnode, tokenType cmp);
	void printTerminal(int indent);
	void printNonterminal(int indent, std::string str);
	//High-level program structure
	void program();
	void methodDecl();
	void type();
	void identifier();
	void formalParams();
	void formalParam();
	//statements
	void block();
	void statement();
	void localVarDecl();
	void assignStmt();
	void returnStmt();
	void ifStmt();
	void whileStmt();
	void writeStmt();
	void readStmt();
	//expression
	void expression();
	void multiplicativeExpr();
	void primaryExpr();
	void boolExpression();
	void actualParams();
private:
	lexicalAnalyzer input;
	int curIndex;
	int curDepth;
};