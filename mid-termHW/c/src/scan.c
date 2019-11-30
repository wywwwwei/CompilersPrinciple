#define _CRT_SECURE_NO_WARNINGS
#include"scan.h"
#include<stdlib.h>

struct Scan* scan_struct(char* filePath)
{
	struct Scan *scan = malloc(sizeof(struct Scan));

	scan->file = fopen(filePath, "r");
	memset(scan->readBuffer1, 0, sizeof(scan->readBuffer1));
	fread(scan->readBuffer1, 1, BUF_SIZE, scan->file);
	scan->readBuffer1[BUF_SIZE - 1] = EOF;
	scan->lexemeBegin = scan->forward = &scan->readBuffer1[0];
	scan->row = 1;
	scan->col = 1;
	scan->currentBuf = FirstBuf;

	return scan;
}
void scan_destruct(struct Scan *scan)
{
	fclose(scan->file);
	free(scan);
}

char* getString(struct Scan*scan)
{
	int index = 0;
	char* current = scan->lexemeBegin;
	while (current != scan->forward) {
		if (*current == EOF) {
			if (scan->currentBuf == FirstBuf) {
				current = &scan->readBuffer1[0];
			}
			else if (scan->currentBuf == SecondBuf) {
				current = &scan->readBuffer2[0];
			}
		}
		scan->loadLexeme[index++] = *(current++);
	}
	scan->loadLexeme[index] = '\0';
	return scan->loadLexeme;
}

void readFromFile(struct Scan*scan)
{
	if (scan->currentBuf == FirstBuf) {
		memset(scan->readBuffer2, 0, sizeof(scan->readBuffer2));
		fread(scan->readBuffer2, 1, BUF_SIZE, scan->file);
		scan->readBuffer2[BUF_SIZE - 1] = EOF;

		scan->currentBuf = SecondBuf;
		scan->forward = &scan->readBuffer2[0];
	}
	else {
		memset(scan->readBuffer1, 0, sizeof(scan->readBuffer1));
		fread(scan->readBuffer1, 1, BUF_SIZE, scan->file);
		scan->readBuffer1[BUF_SIZE - 1] = EOF;

		scan->currentBuf = FirstBuf;
		scan->forward = &scan->readBuffer1[0];
	}
}

char nextChar(struct Scan*scan)
{
	switch (*scan->forward)
	{
	case EOF:
		readFromFile(scan);
		break;
	case '\n':
		scan->col = 1;
		scan->row++;
		scan->forward++;
		return nextChar(scan);
	default:
		scan->col++;
		break;
	}
	return *scan->forward++;
}

char curChar(struct Scan*scan)
{
	return *scan->forward;
}
int getRow(struct Scan*scan)
{
	return scan->row;
}
int getCol(struct Scan*scan)
{
	return scan->col;
}
void nextLexeme(struct Scan*scan)
{
	scan->lexemeBegin = scan->forward;
}