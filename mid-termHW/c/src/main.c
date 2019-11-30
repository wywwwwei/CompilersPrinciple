#define _CRT_SECURE_NO_WARNINGS
#include"syntaxAnalyzer.h"

int main(void)
{
	char filePath[256];
	scanf("%s", &filePath);
	struct SyntaxAnalyzer *sa = SyntaxAnalyzer_struct(filePath);
	analyse(sa);
	SyntaxAnalyzer_destruct(sa);
}