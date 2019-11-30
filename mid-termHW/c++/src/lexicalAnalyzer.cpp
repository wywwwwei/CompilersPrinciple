#include"lexicalAnalyzer.h"
#include<iostream>
#include<iomanip>

lexicalAnalyzer::lexicalAnalyzer(std::string filePath):input(filePath){
}

tokennode* lexicalAnalyzer::getTokenAt(int index) {
	if (index >= tokensStore.size()) {
		return nullptr;
	}
	return tokensStore[index];
}
lexicalAnalyzer::~lexicalAnalyzer() {
	for (int i = 0; i < tokensStore.size(); i++) {
		delete(tokensStore[i]);
	}
}
void lexicalAnalyzer::analyse() {
	char current = '\0';
	while (current = input.nextChar() ) {
		if (current == '\0')break;

		if((current >= 'a'&&current <= 'z') || (current >= 'A'&&current <= 'Z')||current=='_') {
			while (current = input.curChar()) {
				if ((current >= 'a'&&current <= 'z') || (current >= '0'&&current <= '9') || (current >= 'A'&&current <= 'Z') || current == '_')
				{
					input.nextChar();
				}
				else {
					printToken(token.getToken(input.getString()));
					input.nextLexeme();
					break;
				}
			}
			continue;
		}

		char lastChar;
		bool real = false;
		switch (current)
		{
		case' ':
		case'\t':
			break;
		case'=':
			if (input.nextChar() == '=') {
				printToken(Operator);
				break;
			}
			logError("Row" + std::to_string(input.getRow()) + "  Column" + std::to_string(input.getCol()) + ": Unrecognizable characters.");
		case':':
			if (input.nextChar() == '=') {
				printToken(Operator);
				break;
			}
			logError("Row" + std::to_string(input.getRow()) + "  Column" + std::to_string(input.getCol()) + ": Unrecognizable characters.");
		case '!':
			if (input.nextChar() == '=') {
				printToken(Operator);
				break;
			}
			logError("Row" + std::to_string(input.getRow()) + "  Column" + std::to_string(input.getCol()) + ": Unrecognizable characters.");
		case ';':
		case',':
		case '(':
		case ')':
			printToken(Delimeter);
			break;
		case '+':
		case '-':
		case '*':
			printToken(Operator);
			break;
		case '/':
			if (input.nextChar() != '*') {
				printToken(Operator);
				break;
			}
			if (input.nextChar() != '*') {
				logError("Row" + std::to_string(input.getRow()) + "  Column" + std::to_string(input.getCol()) + ": Unrecognizable characters.");
			}
			while (lastChar = input.nextChar()) {
				if (lastChar == '*') {
					lastChar = input.nextChar();
					if (lastChar == '*') {
						lastChar = input.nextChar();
						if (lastChar == '/') {
							break;
						}
					}
				}
			}
			break;
		case '<':
		case '>':
			if (input.curChar() == '=') {
				input.nextChar();
				printToken(Operator);
			}
			else {
				printToken(Operator);
			}
			break;
		case  '0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
			while (lastChar = input.curChar()) {
				if (lastChar == '.') {
					if (real) {
						logError("Row " + std::to_string(input.getRow()) + "  Column" + std::to_string(input.getCol()) + ": Unrecognizable characters.");
					}
					real = true;
					input.nextChar();
					continue;
				}
				else if (lastChar>='0' && lastChar<='9') {
					input.nextChar();
					continue;
				}
				else {
					if (real) {
						printToken(Number);
					}
					else {
						printToken(Number);
					}
					break;
				}
			}
			break;
		case'"':
			while (lastChar = input.nextChar()) {
				if (lastChar == '"') {
					printToken(String);
					break;
				}
			}
			break;
		default:
			logError("Row " + std::to_string(input.getRow()) + "  Column " + std::to_string(input.getCol()) + ": Unrecognizable characters.");
		}
		input.nextLexeme();
	}
}

void lexicalAnalyzer::logError(std::string errMsg) {
	std::cerr << errMsg << std::endl;
	exit(1);
}

void lexicalAnalyzer::printToken(tokenType _token) {
	tokennode*nodes = new tokennode(_token, input.getString(),input.getRow(),input.getCol()-input.getString().length());
	tokensStore.push_back(nodes);
	std::cout << std::left << "<" <<std::setw(10)<< token.getTokenStr(_token) << ",  " <<std::setw(11) <<input.getString()<<">" << std::endl;
}

int lexicalAnalyzer::getTokenSize() {
	return tokensStore.size();
}