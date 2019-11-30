#pragma once
#include"token.h"
#include"scan.h"
#include<vector>

class lexicalAnalyzer {
public:
	explicit lexicalAnalyzer(std::string filePath);
	~lexicalAnalyzer();
	void analyse();
	void logError(std::string errMsg);
	void printToken(tokenType _token);
	tokennode* getTokenAt(int index);
	int getTokenSize();
private:
	Token token;
	Scan input;
	std::vector<tokennode*> tokensStore;
};