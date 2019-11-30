#include"scan.h"
#include<sstream>

Scan::Scan(std::string filePath) {
	fileStream.open(filePath,std::ifstream::in|std::ifstream::_Nocreate);

	readBuffer1[BUF_SIZE - 1] = EOF;
	fileStream.read(readBuffer1, BUF_SIZE - 1);

	lexemeBegin = forward = &readBuffer1[0];
}

Scan::~Scan() noexcept{
	if (fileStream) {
		fileStream.close();
	}
}

std::string Scan::getString() {
	std::stringstream ss;

	char* current = lexemeBegin;
	while (current != forward) {

		if (*current == EOF) {
			if (currentBuf == Tag::FirstBuf) {
				current = &readBuffer1[0];
			}
			else if (currentBuf == Tag::SecondBuf) {
				current = &readBuffer2[0];
			}
		}
		ss << *current++;
	}

	return ss.str();
}

void Scan::readFromFile() {
	if (currentBuf == Tag::FirstBuf) {
		readBuffer2[BUF_SIZE - 1] = EOF;
		fileStream.read(readBuffer2, BUF_SIZE - 1);

		currentBuf = Tag::SecondBuf;
		forward = &readBuffer2[0];
	}
	else {
		readBuffer1[BUF_SIZE - 1] = EOF;
		fileStream.read(readBuffer1, BUF_SIZE - 1);

		currentBuf = Tag::FirstBuf;
		forward = &readBuffer1[0];
	}
}

char Scan::nextChar() {
	switch (*forward)
	{
	case EOF:
		readFromFile();
		break;
	case '\n':
		col = 1;
		row++;
		forward++;
		return nextChar();
	default:
		col++;
		break;
	}
	return *forward++;
}

char Scan::curChar() {
	return *forward;
}

int Scan::getCol(){
	return col;
}

int Scan::getRow() {
	return row;
}

void Scan::nextLexeme() {
	lexemeBegin = forward;
}