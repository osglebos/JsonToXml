#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

typedef struct LIST *s_LIST;
typedef struct NODE *s_NODE;

struct LIST
{
	std::string name;
	std::string value;
};

struct NODE
{
	std::string name;
	std::vector<s_NODE> nodes;
	std::vector<s_LIST> lists;
};

string MakeString(const char *inFilename);
vector<string> FormatJSON(string str);
string CreateBeautyString(NODE *T, int dep);
string CreateStandartString(NODE *T);
int ParserJSON(NODE *T, vector<string> arr, int first, int last);
int CountLines(char *inFilename);
