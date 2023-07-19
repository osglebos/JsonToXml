#include "JsonToXml.h"

int CheckSum1(string fromwhere)
{
	FILE *F = fopen(fromwhere.c_str(), "rt");
	if (F == NULL)
	{
		fprintf(stderr, "Could not open %s file\n", fromwhere.c_str());
		return -1;
	}
	int sum = 0;
	while (getc(F) != EOF)
		sum++;

	fclose(F);
	return sum;
}

int main(void)
{
	bool exit = false;
	char key;
	char *fromwhere = new char[15];
	char *towhere = new char[15];
	NODE *root = new NODE;

	printf("\nesc - Exit\n");
	printf("1 - Create XML\n");
	printf("2 - Display formated XML\n");
	printf("3 - Upload formated XML to file\n");
	printf("4 - Display string XML\n");
	printf("5 - Upload string XML to file\n");
	printf("6 - Check sum\n");

	while (!exit)
	{

		key = _getch();
		switch (key)
		{
		case 27:
			printf("Exit.\n");
			exit = true;
			break;

		case '1':
		{
			string fromwhere, towhere;
			vector<string> arr;

			cout << "Input name of JSON file to convert(format_full.json):\n " << endl;
			cin >> fromwhere;
			towhere = MakeString(fromwhere.c_str());
			arr = FormatJSON(towhere);
			
			root->name = "root";
			root->lists.clear();
			root->nodes.clear();
			ParserJSON(root, arr, 0, arr.size());
			root = root->nodes[0];

			cout << "Convert sucsses\n";
		}
		break;

		case '2':
			cout << CreateBeautyString(root, 0);
		break;

		case '3':
		{
			string towhere;
			cout << "Input name of XML file to save format xml(format_free.xml|format_full.xml):\n " << endl;
			cin >> towhere;

			FILE *F = fopen(towhere.c_str(), "wt");
			if (F == NULL)
			{
				fprintf(stderr, "Could not open %s file\n", towhere.c_str());
				break;
			}
			fprintf(F, "%s", CreateBeautyString(root, 0).c_str());
			fclose(F);
			cout << "Upload sucsess\n";
		}
		break;
		
		case '4':
			cout << CreateStandartString(root) << endl;
			break;

		case '5':
		{
			string towhere;
			cout << "Input name of XML file to save mxl string(string_free.xml|string_full.xml):\n " << endl;
			cin >> towhere;

			FILE *F = fopen(towhere.c_str(), "wt");
			if (F == NULL)
			{
				fprintf(stderr, "Could not open %s file\n", towhere.c_str());
				break;
			}
			fprintf(F, "%s", CreateStandartString(root).c_str());
			fclose(F);
			cout << "Upload sucsess\n";
		}
		break;

		case '6':
		{
			cout << "Input name of files(string_free):\n " << endl;
			cin >> towhere;
			printf("Number of elements in xml file: %i.\n", CheckSum1(string(towhere) + ".xml"));
			printf("Number of elements in json file: %i.\n", CheckSum1(string(towhere) + ".json"));
			printf("Number of lines in xml file: %i.\n", CountLines((char *)(string(towhere) + ".xml").c_str()));
			printf("Number of lines in json file: %i.\n", CountLines((char *)(string(towhere) + ".json").c_str()));
		}
		break;

		default:
			printf("Wrong symbol\n");
			break;
		}
		printf("----------------------------------------------\n\n");
	}
	return 0;
}