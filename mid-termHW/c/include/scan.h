#pragma once
#include<string.h>
#include<stdio.h>

#define BUF_SIZE 1024

typedef enum { FirstBuf, SecondBuf }Tag;

struct Scan
{
	FILE* file;

	char* lexemeBegin;
	char* forward;

	char loadLexeme[BUF_SIZE];
	char readBuffer1[BUF_SIZE];
	char readBuffer2[BUF_SIZE];

	Tag currentBuf;

	int row;
	int col;
};

struct Scan* scan_struct(char* filePath);
void scan_destruct(struct Scan *scan);

char* getString(struct Scan*scan);
void readFromFile(struct Scan*scan);
char nextChar(struct Scan*scan);
char curChar(struct Scan*scan);
int getRow(struct Scan*scan);
int getCol(struct Scan*scan);
void nextLexeme(struct Scan*scan);