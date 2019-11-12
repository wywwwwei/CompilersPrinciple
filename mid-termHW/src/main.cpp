#include"syntaxAnalyzer.h"
#include<iostream>

int main(int argc, char*argv[]) {
	std::string filePath;
	std::cin >> filePath;
	SyntaxAnalyzer analyzer(filePath);
	analyzer.analyse();
}