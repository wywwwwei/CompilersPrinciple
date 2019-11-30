#include"syntaxAnalyzer.h"
#include<iostream>
#include<iomanip>
//Constructor
SyntaxAnalyzer::SyntaxAnalyzer(std::string filePath) :input(filePath) {

}

void SyntaxAnalyzer::analyse() {
	input.analyse();
	program();
}

void SyntaxAnalyzer::logError(std::string errMsg) {
	std::cerr <<"Row "<<std::to_string(input.getTokenAt(curIndex-1)->row)<<" Col "<<std::to_string(input.getTokenAt(curIndex - 1)->col)<<": "<< errMsg << std::endl;
	exit(1);
}

bool SyntaxAnalyzer::compareContent(tokennode* tempnode, std::string cmp) {
	return (tempnode->content == cmp);
}
bool SyntaxAnalyzer::compareType(tokennode* tempnode, tokenType cmp) {
	return (tempnode->type == cmp);
}


void SyntaxAnalyzer::printTerminal(int indent)
{
	for (int i = 0; i < indent; i++) {
		std::cout << "        ";
	}
	std::cout <<  input.getTokenAt(curIndex - 1)->content << std::endl;
}
void SyntaxAnalyzer::printNonterminal(int indent, std::string str)
{
	for (int i = 0; i < indent; i++) {
		std::cout << "        ";
	}
	std::cout  << str << std::endl;
}

//High-level program structure
void SyntaxAnalyzer::program()
{
	printNonterminal(curDepth, "Program");
	curDepth++;
	while (input.getTokenAt(curIndex) != nullptr) {
		methodDecl();
	}
	curDepth--;
}

void SyntaxAnalyzer::methodDecl()
{
	printNonterminal(curDepth, "MethodDecl");
	curDepth += 1;
	type();
	if (compareType(input.getTokenAt(curIndex) , MAIN)) {
		curIndex++;
		printTerminal(curDepth);
	}
	identifier();

	if (compareContent( input.getTokenAt(curIndex++), "(")) {
		printTerminal(curDepth);
		formalParams();
	}
	else {
		logError("Missing '('");
	}
	if (!compareContent(input.getTokenAt(curIndex++), ")")) {
		logError("Missing ')'");
	}
	printTerminal(curDepth);
	block();
	curDepth -= 1;
}

void SyntaxAnalyzer::type()
{
	printNonterminal(curDepth, "Type");
	curDepth++;
	switch (input.getTokenAt(curIndex++)->type) {
	case INT:
	case REAL:
	case STRING:
		printTerminal(curDepth);
		break;
	default:
		logError("No matchable type");
	}
	curDepth--;
}

void SyntaxAnalyzer::identifier() {
	printNonterminal(curDepth, "Identifier");
	curDepth++;
	curIndex++;
	printTerminal(curDepth);
	curDepth--;
}

void SyntaxAnalyzer::formalParams() {
	printNonterminal(curDepth, "FormalParams");
	curDepth++;
	switch (input.getTokenAt(curIndex)->type) {
	case INT:
	case REAL:
	case STRING:
		formalParam();
		while (compareContent(input.getTokenAt(curIndex), ",")) {
			curIndex++;
			formalParam();
		}
		break;
	default:
		break;
	}
	curDepth--;
}

void SyntaxAnalyzer::formalParam() {
	printNonterminal(curDepth, "FormalParam");
	curDepth++;
	type();
	identifier();
	curDepth--;
}

//statements
void SyntaxAnalyzer::block(){
	printNonterminal(curDepth, "Block");
	if (!compareType(input.getTokenAt(curIndex++), BEGIN)) {
		logError("Missing BEGIN of the block");
	}
	printTerminal(curDepth);
	curDepth++;
	while (!compareType(input.getTokenAt(curIndex), END)) {
		statement();
	}
	curDepth--;
	curIndex++;
	printTerminal(curDepth);
}

void SyntaxAnalyzer::statement() {
	printNonterminal(curDepth, "Statement");
	curDepth++;
	switch (input.getTokenAt(curIndex)->type)
	{
	case INT:
	case REAL:
	case STRING:
		localVarDecl();
		break;
	case IDENTIFIER:
		assignStmt();
		break;
	case RETURN:
		returnStmt();
		break;
	case IF:
		ifStmt();
		break;
	case WHILE:
		whileStmt();
		break;
	case READ:
		readStmt();
		break;
	case WRITE:
		writeStmt();
		break;
	case BEGIN:
		block();
		break;
	default:
		curIndex++;
		logError("Unrecognizable statement, starting with the wrong");
		break;
	}
	curDepth--;
}

void SyntaxAnalyzer::localVarDecl(){
	printNonterminal(curDepth, "LocalVarDecl");
	curDepth++;
	type();
	identifier();
	if (!compareContent(input.getTokenAt(curIndex++), ";")) {
		logError("Missing ';' before this statement");
	}
	curDepth--;
}

void SyntaxAnalyzer::assignStmt()
{
	printNonterminal(curDepth, "assignStmt");
	curDepth++;
	identifier();
	if (!compareContent(input.getTokenAt(curIndex++), ":="))
	{
		logError("Missing \":=\"");
	}
	printTerminal(curDepth);
	expression();
	if (!compareContent(input.getTokenAt(curIndex++), ";")) {
		logError("Missing ';' before this statement");
	}
	curDepth--;
}

void SyntaxAnalyzer::returnStmt() {
	printNonterminal(curDepth, "ReturnStmt");
	curDepth++;
	if (!compareType(input.getTokenAt(curIndex++), RETURN))
	{
		logError("Missing \"RETURN\"");
	}
	printTerminal(curDepth);
	expression();
	if (!compareContent(input.getTokenAt(curIndex++), ";")) {
		logError("Missing ';' before this statement");
	}
	curDepth--;
}

void SyntaxAnalyzer::ifStmt() {
	printNonterminal(curDepth, "IfStmt");
	curDepth++;
	if (!compareType(input.getTokenAt(curIndex++), IF))
	{
		logError("Missing \"IF\"");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), "(")) {
		logError("Missing '('");
	}
	printTerminal(curDepth);
	boolExpression();
	if (!compareContent(input.getTokenAt(curIndex++), ")")) {
		logError("Missing ')'");
	}
	printTerminal(curDepth);
	statement();
	if (compareType(input.getTokenAt(curIndex), ELSE))
	{
		curIndex++;
		printTerminal(curDepth);
		statement();
	}
	curDepth--;
}

void SyntaxAnalyzer::whileStmt() {
	printNonterminal(curDepth, "WhileStmt");
	curDepth++;
	if (!compareType(input.getTokenAt(curIndex++), WHILE))
	{
		logError("Missing \"WHILE\"");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), "(")) {
		logError("Missing '('");
	}
	printTerminal(curDepth);
	boolExpression();
	if (!compareContent(input.getTokenAt(curIndex++), ")")) {
		logError("Missing ')'");
	}
	printTerminal(curDepth);
	statement();
	curDepth--;
}

void SyntaxAnalyzer::writeStmt() {
	printNonterminal(curDepth, "WriteStmt");
	curDepth++;
	if (!compareType(input.getTokenAt(curIndex++), WRITE))
	{
		logError("Missing \"WRITE\"");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), "(")) {
		logError("Missing '('");
	}
	printTerminal(curDepth);
	expression();
	if (!compareContent(input.getTokenAt(curIndex++), ",")) {
		logError("Missing ','");
	}
	if (!compareType(input.getTokenAt(curIndex++), String)) {
		logError("Missing string");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), ")")) {
		logError("Missing ')'");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), ";")) {
		logError("Missing ';' before this statement");
	}
	curDepth--;
}

void SyntaxAnalyzer::readStmt() {
	printNonterminal(curDepth, "ReadStmt");
	curDepth++;
	if (!compareType(input.getTokenAt(curIndex++), READ))
	{
		logError("Missing \"READ\"");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), "(")) {
		logError("Missing '('");
	}
	printTerminal(curDepth);
	identifier();
	if (!compareContent(input.getTokenAt(curIndex++), ",")) {
		logError("Missing ','");
	}
	if (!compareType(input.getTokenAt(curIndex++), String)) {
		logError("Missing string");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), ")")) {
		logError("Missing ')'");
	}
	printTerminal(curDepth);
	if (!compareContent(input.getTokenAt(curIndex++), ";")) {
		logError("Missing ';' before this statement");
	}
	curDepth--;
}

//expression
void SyntaxAnalyzer::expression() {
	printNonterminal(curDepth, "Expression");
	curDepth++;
	multiplicativeExpr();
	while(compareContent(input.getTokenAt(curIndex), "+")|| compareContent(input.getTokenAt(curIndex), "-")) {
		curIndex++;
		printTerminal(curDepth);
		multiplicativeExpr();
	}
	curDepth--;
}

void SyntaxAnalyzer::multiplicativeExpr() {
	printNonterminal(curDepth, "MultiplicativeExpr");
	curDepth++;
	primaryExpr();
	while (compareContent(input.getTokenAt(curIndex), "*") || compareContent(input.getTokenAt(curIndex), "/")) {
		curIndex++;
		printTerminal(curDepth);
		primaryExpr();
	}
	curDepth--;
}

void SyntaxAnalyzer::primaryExpr() {
	printNonterminal(curDepth, "PrimaryExpr");
	curDepth++;
	tokennode * temp;
	switch (input.getTokenAt(curIndex)->type)
	{
	case Number:
		curIndex++;
		printTerminal(curDepth);
		break;
	case IDENTIFIER:
		identifier();
		temp = input.getTokenAt(curIndex);
		if (temp != nullptr) {
			if (compareContent(temp, "(")) {
				curIndex++;
				printTerminal(curDepth);
				actualParams();
				if (!compareContent(input.getTokenAt(curIndex++), ")")) {
					logError("Missing ')'");
				}
				printTerminal(curDepth);
			}
		}
		break;
	case Delimeter:
		if (compareContent(input.getTokenAt(curIndex++), "(")) {
			printTerminal(curDepth);
			expression();
			if (!compareContent(input.getTokenAt(curIndex++), ")")) {
				logError("Missing ')'");
			}
			printTerminal(curDepth);
		}
		else {
			logError("Missing '('");
		}
		break;
	default:
		break;
	}
	curDepth--;
}

void SyntaxAnalyzer::boolExpression() {
	printNonterminal(curDepth, "BoolExpression");
	curDepth++;
	expression();
	if (!compareContent(input.getTokenAt(curIndex), "==")
		&& !compareContent(input.getTokenAt(curIndex), "<")
		&& !compareContent(input.getTokenAt(curIndex), ">")
		&& !compareContent(input.getTokenAt(curIndex), "<=")
		&& !compareContent(input.getTokenAt(curIndex), ">=")) 
	{
		curIndex++;
		logError("Missing comparison operator");
	}
	curIndex++;
	printTerminal(curDepth);
	expression();
	curDepth--;
}

void SyntaxAnalyzer::actualParams() {
	printNonterminal(curDepth, "ActualParams");
	curDepth++;
	switch (input.getTokenAt(curIndex)->type)
	{
	case Number:
		break;
	case IDENTIFIER:
		break;
	case Delimeter:
		if (compareContent(input.getTokenAt(curIndex), "(")) {

		}
		else {
			logError("Missing '('");
		}
		break;
	default:
		return;
	}
	expression();
	while (compareContent(input.getTokenAt(curIndex), ",")) {
		curIndex++;
		expression();
	}
	curDepth--;
}