#pragma once
#include"token.h"
#include"scan.h"


struct lexicalAnalyzer {
	struct Scan* input;
};

struct lexicalAnalyzer* lexicalAnalyzer_struct(char* filePath);
void lexicalAnalyzer_destruct(struct lexicalAnalyzer* la);
tokennode lexical_analyse(struct lexicalAnalyzer* la);
void logError(struct lexicalAnalyzer* la, const char* errMsg);
