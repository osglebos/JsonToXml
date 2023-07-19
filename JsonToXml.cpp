#include "JsonToXml.h"

string MakeString(const char *inFilename)
{
	string res("");
	FILE *inFile = fopen(inFilename, "rt");
	if (inFile == NULL)
	{
		fprintf(stderr, "Could not open %s file\n", inFilename);
		return string();
	}

	char c;
	while ((c = getc(inFile)) != EOF)
	{
		switch (c)
		{
		case '\n':
		case ' ':
			res += " ";
			while (((c = getc(inFile)) != EOF) && ((c == ' ') || (c == '\n')));

		default:
			res += c;
			break;
		}
	}
	fclose(inFile);
	return res;
}

bool is_str_empty(string str)
{
	for (int i = 0; i < (int)str.length(); i++)
		if (str[i] != ' ') return false;

	return true;
}

bool IsRightLetter(char c)
{
	bool res = false;
	switch (c)
	{
	case '}':
	case '{':
	case '[':
	case ']':
		res = true;
		break;
	default:
		res = false;
		break;
	}
	return res;
}

vector<string> FormatJSON(string str)
{
	vector<string> res;
	int start = 0;

	for (int i = 0; i < (int)str.length(); i++)
	{
		if (IsRightLetter(str[i]) == true)
		{
			string tmp = str.substr(start, i - start + 1);
			res.push_back(tmp);
			start = i + 2;
		}
		else if ((str[i] == '"') && (str[i+1] == ','))
		{
			string tmp = str.substr(start, i - start+2);
			if ((i - start != 1) && (is_str_empty(tmp) == false)) res.push_back(tmp);
			start = i + 3;
		}
	}

	for (int j = 0; j < (int)res.size(); j++)
	{
		string str = res[j];
		for (int i = 0; i < (int)str.size(); i++)
		{
			if (str[i] == ' ') str = str.substr(1, str.size() - 1);
			else break;
		}
		res[j] = str;
	}

	return res;
}

int CountLines(char *inFilename)
{
	int counter = 0;
	ifstream inFile(inFilename);
	string s;
	while (getline(inFile, s)) counter++;
	inFile.close();
	return counter;
}

vector<int> FindQuotes(string tmp)
{
	vector<int> res;
	res.clear();

	for (int i = 0; i < (int)tmp.size(); i++)
		if (tmp[i] == '"') res.push_back(i);

	return res;
}

int ParserJSON(NODE *T, vector<string> arr, int first, int last)
{
	bool exit = false;
	int count = 0;
	int j = first;
	while ((j < last) && (exit == false))
	{
		string str = arr[j];
		char c = str.back();
		switch (c)
		{
		case '[':
		{
			NODE *tmp = new NODE;
			vector<int> coords = FindQuotes(str);
			tmp->name = str.substr(coords[0] + 1, coords[1] - coords[0] - 1);
			tmp->lists.clear();
			tmp->nodes.clear();
			(T->nodes).push_back(tmp);
			count += ParserJSON(tmp, arr, j + 1, last);
			count++;
		}
		break;
		case '{':
		{
			if (str.size() >= 2)
			{
				NODE *tmp = new NODE;
				vector<int> coords = FindQuotes(str);
				tmp->name = str.substr(coords[0] + 1, coords[1] - coords[0] - 1);
				tmp->lists.clear();
				tmp->nodes.clear();
				(T->nodes).push_back(tmp);
				count += ParserJSON(tmp, arr, j + 1, last);
			}
			else count += ParserJSON(T, arr, j + 1, last);
			count++;
		}
		break;
		case '}':
			exit = true;
		case ']':
			if (str.size() < 3)
			{
				count++;
				break;
			}
		case ',':
		{
			LIST *tmp = new LIST;
			vector<int> coords = FindQuotes(str);
			tmp->name = str.substr(coords[0] + 1, coords[1] - coords[0] - 1);
			tmp->value = str.substr(coords[2] + 1, coords[3] - coords[2] - 1);
			T->lists.push_back(tmp);
			count++;
		}
		break;
		}
		j = first + count;
	}
	return count;
}

string AddTab(int count)
{
	string res = "";
	for (int i = 0; i < count; i++) res += "  ";
	return res;
}

string CreateBeautyString(NODE *T, int dep)
{
	string res = "";
	
	res += AddTab(dep) + "<" + T->name + ">\n";
	for (int i = 0; i < (int)T->lists.size(); i++)
		res += AddTab(dep+1) + "<" + T->lists[i]->name + ">" + T->lists[i]->value + "</" + T->lists[i]->name + ">\n";

	for (int i = 0; i < (int)T->nodes.size(); i++)
		res += CreateBeautyString(T->nodes[i], dep + 1);

	res += AddTab(dep) + "</" + T->name + ">\n";

	return res;
}

string CreateStandartString(NODE *T)
{
	string res = "";

	res += "<" + T->name + "> ";
	for (int i = 0; i < (int)T->lists.size(); i++)
		res += "<" + T->lists[i]->name + ">" + T->lists[i]->value + "</" + T->lists[i]->name + "> ";

	for (int i = 0; i < (int)T->nodes.size(); i++)
		res += CreateStandartString(T->nodes[i]);

	res += "</" + T->name + "> ";

	return res;
}