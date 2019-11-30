#include "syntaxAnalyzer.h"
#include<stdlib.h>

struct SyntaxAnalyzer* SyntaxAnalyzer_struct(char* filePath)
{
	struct SyntaxAnalyzer * sa = malloc(sizeof(struct SyntaxAnalyzer));
	sa->input = lexicalAnalyzer_struct(filePath);
	sa->curIndex = -1;
	sa->curDepth = 0;
	sa->notCreate = false;
	return sa;
}

void SyntaxAnalyzer_destruct(struct SyntaxAnalyzer* sa)
{
	lexicalAnalyzer_destruct(sa->input);
	free(sa);
}

void analyse(struct SyntaxAnalyzer* sa)
{
	program(sa);
}

void logSyntaxError(struct SyntaxAnalyzer* sa, const char* errMsg)
{
	tokennode node = getTokennode(sa, 0);
	printf("Row %d Column %d :%s", node.row, node.col, errMsg);
	SyntaxAnalyzer_destruct(sa);
	exit(1);
}
bool compareContent(tokennode tempnode, const char* cmp)
{
	return (strcmp(tempnode.content, cmp) == 0);
}
bool compareType(tokennode tempnode, tokenType cmp)
{
	return tempnode.type == cmp;
}
void printTerminal(struct SyntaxAnalyzer* sa)
{
	for (int i = 0; i < sa->curDepth; i++) {
		printf("        ");
	}
	printf("%s\n", getTokennode(sa, 0).content);
}
void printNonterminal(struct SyntaxAnalyzer* sa, const char* str)
{
	for (int i = 0; i < sa->curDepth; i++) {
		printf("        ");
	}
	printf("%s\n", str);
}
tokennode getTokennode(struct SyntaxAnalyzer* sa, int offset)
{
	if (offset == 0) {
		return sa->store[sa->curIndex];
	}
	else if (offset > 0) {
		if (sa->notCreate) {
			sa->notCreate = false;
			return sa->store[sa->curIndex];
		}
		for (int i = 0; i < offset; i++) {
			sa->curIndex = (sa->curIndex + 1) % NODE_STORE;
			sa->store[sa->curIndex] = lexical_analyse(sa->input);
		}
		return sa->store[sa->curIndex];
	}
	else{
		int index = sa->curIndex - offset;
		index = index >= 0 ? index : (index%NODE_STORE) + NODE_STORE;
		return sa->store[index];
	}
}
//High-level program structure
void program(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "Program");
	sa->curDepth++;
	while (getTokennode(sa, 1).type != -1) {
		sa->notCreate = true;
		methodDecl(sa);
	}
	sa->curDepth--;
}
void methodDecl(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "MethodDecl");
	sa->curDepth += 1;
	type(sa);
	if (compareType(getTokennode(sa,1), MAIN)) {
		printTerminal(sa);
	}
	else {
		sa->notCreate = true;
	}
	identifier(sa);

	if (compareContent(getTokennode(sa, 1), "(")) {
		printTerminal(sa);
		formalParams(sa);
	}
	else {
		logSyntaxError(sa,"Missing '('");
	}
	if (!compareContent(getTokennode(sa, 1), ")")) {
		logSyntaxError(sa,"Missing ')'");
	}
	printTerminal(sa);
	block(sa);
	sa->curDepth -= 1;
}
void type(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "Type");
	sa->curDepth++;
	switch (getTokennode(sa, 1).type) {
	case INT:
	case REAL:
	case STRING:
		printTerminal(sa);
		break;
	default:
		logSyntaxError(sa,"No matchable type");
	}
	sa->curDepth--;
}
void identifier(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "Identifier");
	sa->curDepth++;
	getTokennode(sa, 1);
	printTerminal(sa);
	sa->curDepth--;
}
void formalParams(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "FormalParams");
	sa->curDepth++;
	switch (getTokennode(sa,1).type) {
	case INT:
	case REAL:
	case STRING:
		sa->notCreate = true;
		formalParam(sa);
		while (compareContent(getTokennode(sa,1), ",")) {
			formalParam(sa);
		}
		sa->notCreate = true;
		break;
	default:
		sa->notCreate = true;
		break;
	}
	sa->curDepth--;
}
void formalParam(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "FormalParam");
	sa->curDepth++;
	type(sa);
	identifier(sa);
	sa->curDepth--;
}
//statements
void block(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "Block");
	if (!compareType(getTokennode(sa,1), BEGIN)) {
		logSyntaxError(sa,"Missing BEGIN of the block");
	}
	printTerminal(sa);
	sa->curDepth++;
	while (!compareType(getTokennode(sa,1), END)) {
		sa->notCreate = true;
		statement(sa);
	}
	sa->curDepth--;
	printTerminal(sa);
}
void statement(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "Statement");
	sa->curDepth++;
	switch (getTokennode(sa, 1).type)
	{
	case INT:
	case REAL:
	case STRING:
		sa->notCreate = true;
		localVarDecl(sa);
		break;
	case IDENTIFIER:
		sa->notCreate = true;
		assignStmt(sa);
		break;
	case RETURN:
		sa->notCreate = true;
		returnStmt(sa);
		break;
	case IF:
		sa->notCreate = true;
		ifStmt(sa);
		break;
	case WHILE:
		sa->notCreate = true;
		whileStmt(sa);
		break;
	case READ:
		sa->notCreate = true;
		readStmt(sa);
		break;
	case WRITE:
		sa->notCreate = true;
		writeStmt(sa);
		break;
	case BEGIN:
		sa->notCreate = true;
		block(sa);
		break;
	default:
		logSyntaxError(sa,"Unrecognizable statement, starting with the wrong");
		break;
	}
	sa->curDepth--;
}
void localVarDecl(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "LocalVarDecl");
	sa->curDepth++;
	type(sa);
	identifier(sa);
	if (!compareContent(getTokennode(sa,1), ";")) {
		logSyntaxError(sa,"Missing ';' before this statement");
	}
	sa->curDepth--;
}
void assignStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "assignStmt");
	sa->curDepth++;
	identifier(sa);
	if (!compareContent(getTokennode(sa,1), ":="))
	{
		logSyntaxError(sa,"Missing \":=\"");
	}
	printTerminal(sa);
	expression(sa);
	if (!compareContent(getTokennode(sa,1), ";")) {
		logSyntaxError(sa,"Missing ';' before this statement");
	}
	sa->curDepth--;
}
void returnStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "ReturnStmt");
	sa->curDepth++;
	if (!compareType(getTokennode(sa,1), RETURN))
	{
		logSyntaxError(sa,"Missing \"RETURN\"");
	}
	printTerminal(sa);
	expression(sa);
	if (!compareContent(getTokennode(sa,1), ";")) {
		logSyntaxError(sa,"Missing ';' before this statement");
	}
	sa->curDepth--;
}
void ifStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "IfStmt");
	sa->curDepth++;
	if (!compareType(getTokennode(sa,1), IF))
	{
		logSyntaxError(sa,"Missing \"IF\"");
	}
	printTerminal(sa);
	if (!compareContent(getTokennode(sa,1), "(")) {
		logSyntaxError(sa,"Missing '('");
	}
	printTerminal(sa);
	boolExpression(sa);
	if (!compareContent(getTokennode(sa,1), ")")) {
		logSyntaxError(sa,"Missing ')'");
	}
	printTerminal(sa);
	statement(sa);
	if (compareType(getTokennode(sa,1), ELSE))
	{
		printTerminal(sa);
		statement(sa);
	}
	sa->notCreate = true;
	sa->curDepth--;
}
void whileStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "WhileStmt");
	sa->curDepth++;
	if (!compareType(getTokennode(sa,1), WHILE))
	{
		logSyntaxError(sa,"Missing \"WHILE\"");
	}
	printTerminal(sa);
	if (!compareContent(getTokennode(sa,1), "(")) {
		logSyntaxError(sa,"Missing '('");
	}
	printTerminal(sa );
	boolExpression(sa);
	if (!compareContent(getTokennode(sa,1), ")")) {
		logSyntaxError(sa,"Missing ')'");
	}
	printTerminal(sa);
	statement(sa);
	sa->curDepth--;
}
void writeStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "WriteStmt");
	sa->curDepth++;
	if (!compareType(getTokennode(sa,1), WRITE))
	{
		logSyntaxError(sa,"Missing \"WRITE\"");
	}
	printTerminal(sa);
	if (!compareContent(getTokennode(sa,1), "(")) {
		logSyntaxError(sa,"Missing '('");
	}
	printTerminal(sa);
	expression(sa);
	if (!compareContent(getTokennode(sa,1), ",")) {
		logSyntaxError(sa,"Missing ','");
	}
	if (!compareType(getTokennode(sa,1), String)) {
		logSyntaxError(sa,"Missing string");
	}
	printTerminal(sa);
	if (!compareContent(getTokennode(sa,1), ")")) {
		logSyntaxError(sa,"Missing ')'");
	}
	printTerminal(sa);
	if (!compareContent(getTokennode(sa,1), ";")) {
		logSyntaxError(sa,"Missing ';' before this statement");
	}
	sa->curDepth--;
}
void readStmt(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "ReadStmt");
	sa->curDepth++;
	if (!compareType(getTokennode(sa,1), READ))
	{
		logSyntaxError(sa,"Missing \"READ\"");
	}
	printTerminal(sa );
	if (!compareContent(getTokennode(sa,1), "(")) {
		logSyntaxError(sa,"Missing '('");
	}
	printTerminal(sa );
	identifier(sa);
	if (!compareContent(getTokennode(sa,1), ",")) {
		logSyntaxError(sa,"Missing ','");
	}
	if (!compareType(getTokennode(sa,1), String)) {
		logSyntaxError(sa,"Missing string");
	}
	printTerminal(sa );
	if (!compareContent(getTokennode(sa,1), ")")) {
		logSyntaxError(sa,"Missing ')'");
	}
	printTerminal(sa );
	if (!compareContent(getTokennode(sa,1), ";")) {
		logSyntaxError(sa,"Missing ';' before this statement");
	}
	sa->curDepth--;
}
//expression
void expression(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa , "Expression");
	sa->curDepth++;
	multiplicativeExpr(sa);
	while (1) {
		tokennode temp = getTokennode(sa, 1);
		if (compareContent(temp, "+") || compareContent(temp, "-"))
		{
			printTerminal(sa);
			multiplicativeExpr(sa);
		}
		else {
			break;
		}
	}
	sa->notCreate = true;
	sa->curDepth--;
}
void multiplicativeExpr(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa,  "MultiplicativeExpr");
	sa->curDepth++;
	primaryExpr(sa);
	while (1) {
		tokennode temp = getTokennode(sa, 1);
		if (compareContent(temp, "*") || compareContent(temp, "/"))
		{
			printTerminal(sa);
			primaryExpr(sa);
		}
		else {
			break;
		}
	}
	sa->notCreate = true;
	sa->curDepth--;
}
void primaryExpr(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa , "PrimaryExpr");
	sa->curDepth++;
	tokennode  temp;
	switch (getTokennode(sa,1).type)
	{
	case Number:
		printTerminal(sa);
		break;
	case IDENTIFIER:
		sa->notCreate = true;
		identifier(sa);
		temp = getTokennode(sa, 1);
		if (temp.type != -1) {
			if (compareContent(temp, "(")) {
				printTerminal(sa  );
				actualParams(sa);
				if (!compareContent(getTokennode(sa,1), ")")) {
					logSyntaxError(sa,"Missing ')'");
				}
				printTerminal(sa );
			}
			else {
				sa->notCreate = true;
			}
		}
		break;
	case Delimeter:
		if (compareContent(getTokennode(sa,1), "(")) {
			printTerminal(sa );
			expression(sa);
			if (!compareContent(getTokennode(sa, 1), ")")) {
				logSyntaxError(sa,"Missing ')'");
			}
			printTerminal(sa );
		}
		else {
			logSyntaxError(sa,"Missing '('");
		}
		break;
	default:
		break;
	}
	sa->curDepth--;
}
void boolExpression(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "BoolExpression");
	sa->curDepth++;
	expression(sa);
	if (!compareContent(getTokennode(sa,1), "==")
		&& !compareContent(getTokennode(sa, 0), "<")
		&& !compareContent(getTokennode(sa, 0), ">")
		&& !compareContent(getTokennode(sa, 0), "<=")
		&& !compareContent(getTokennode(sa, 0), ">="))
	{
		
		logSyntaxError(sa,"Missing comparison operator");
	}
	printTerminal(sa );
	expression(sa);
	sa->curDepth--;
}
void actualParams(struct SyntaxAnalyzer* sa)
{
	printNonterminal(sa, "ActualParams");
	sa->curDepth++;
	switch (getTokennode(sa,1).type)
	{
	case Number:
		break;
	case IDENTIFIER:
		break;
	case Delimeter:
		if (compareContent(getTokennode(sa,0), "(")) {

		}
		else {
			logSyntaxError(sa,"Missing '('");
		}
		break;
	default:
		return;
	}
	sa->notCreate = true;
	expression(sa);
	while (compareContent(getTokennode(sa,1), ",")) {
		expression(sa);
	}
	sa->notCreate = true;
	sa->curDepth--;
}