#pragma once
#include<fstream>
#include<string>

#define BUF_SIZE 1024

class Scan
{
public:
	explicit Scan(std::string filePath);
	~Scan() noexcept;
	
	std::string getString();
	void readFromFile();
	char nextChar();
	char curChar();
	int getRow();
	int getCol();
	void nextLexeme();
private:
	std::ifstream fileStream;

	char* lexemeBegin;
	char* forward;

	char readBuffer1[BUF_SIZE];
	char readBuffer2[BUF_SIZE];

	enum Tag {FirstBuf,SecondBuf};
	Tag currentBuf = Tag::FirstBuf;

	int row = 1;
	int col = 1;
};
