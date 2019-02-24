// Sam Glendenning
// Abbas Lawal
// AC21009 Assignment 2

#include <iostream>
#include <array>
#include <fstream>
#include <cstring>

using namespace std;

char getColour(char, char, char, int*);
void setColour(char*, char*, int*);
void printToFile(char*, char*, int*, int, int);
void printToScreen(char*, char*, int*, int, int);
void switchArrays(char*, char*);
int* decimalToBinary(int);
int getDecimal();
int* binaryToArray(string);

int main()
{
	int genCount;
	int selection;	

	while (selection != 3)
	{
		cout << endl << "Select an option." << endl;
		cout << "1. Create an automaton." << endl;
		cout << "2. Open an automaton from file." << endl;
		cout << "3. Quit." << endl;

		while (!(cin >> selection) || selection < 1 || selection > 3)
		{
			cout << "Enter either 1, 2 or 3." << endl;
			cin.clear();
			cin.ignore();
			cin >> selection;
		}

		if (selection == 1)
		{
			cout << "Cellular Automata are created based on a specific ruleset. A cell is given a value of '.' or 'X' based on three cells above it." << endl;
			cout << "Each cell can have one of two values. With 2^3 combinations of these 3 cells, as well as 8 total rules, we have 2^8 possible rulesets." << endl;
			cout << "These can be selected with a binary value, each with its own rules that determine the pattern the automaton generates." << endl;
			cout << "Enter B to enter a binary value or D to enter a decimal value." << endl;

			char bindec;
			cin >> bindec;

			while (bindec != 'B' && bindec != 'b' && bindec != 'd' && bindec != 'D')
			{
				cout << "Error. Enter B for binary or D for decimal." << endl;
				cin >> bindec;
			}

			int* ruleSet;

			if (bindec == 'D' || bindec == 'd')
			{
				int decimal = getDecimal();
				ruleSet = decimalToBinary(decimal);
			}
			else
			{
				cout << "Enter an 8-bit binary number." << endl;
				string binary;
				cin >> binary;

				while (binary.length() != 8)
				{
					cout << "Binary number must be 8 bits." << endl;
					cin >> binary;
				}

				for (unsigned int i=0; i<binary.length(); i++)
				{
					if (binary[i] != '0' && binary[i] != '1')
					{
						cout << "Invalid binary number." << endl;	
						return (0);
					}
				}

				ruleSet = binaryToArray(binary);
			}

			cout << "Enter the length of each row." << endl;
			int rowCount;
			cin >> rowCount;

			while (rowCount < 3)
			{
				cout << "Your number must be greater than or equal to 3." << endl;	
				cin >> rowCount;
			}

			char parent [rowCount];
			char child [rowCount];

			for(int i=0; i<rowCount; i++)
			{
				parent[i] = '.';
			}

			cout << "Enter the number of generations in the automaton. This is the number of rows the automaton will have." << endl;
			cin >> genCount;

			while (genCount < 1)
			{
				cout << "Your number must be greater than or equal to 1." << endl;	
				cin >> genCount;
			}

			int column;
			cout << "The first generation of the automaton is of length " << rowCount << endl;
			cout << "Enter the column number for the 'X' to go in." << endl;
			cin >> column;

			while (column < 0 || column > rowCount)
			{
				cout << "Column number must be in range 0 to " << rowCount << endl;
				cin >> column;
			}

			parent[column-1] = 'X';

			char yesno;
			cout << "Save output to a text file (Output.txt)? [Y/N]" << endl;
			cin >> yesno;

			if (yesno == 'Y' || yesno == 'y')
				printToFile(parent, child, ruleSet, genCount, rowCount);
			else
				printToScreen(parent, child, ruleSet, genCount, rowCount);
		}
		else if (selection == 2)
		{
			string name;
			cout << "Enter the name of the file to open." << endl;
			cin.ignore();
			getline(cin, name);

			ifstream input(name);
			if (input.is_open())
			{
				string line;
				while (getline(input, line))
				{
					cout << line << '\n';
				}

				input.close();
			}
			else
			{
				cout << "Unable to open this file." << endl;
			}
		}
	}

	return (0);
}

//////////////////////////////////////////////////////////////////////////

char getColour(char first, char second, char third, int *ruleSet)
{
	int value;

	if (first == 'X' && second == 'X' && third == 'X')
		value = 1;
	else if (first == 'X' && second == 'X' && third == '.')
		value = 2;
	else if (first == 'X' && second == '.' && third == 'X')
		value = 3;
	else if (first == 'X' && second == '.' && third == '.')
		value = 4;
	else if (first == '.' && second == 'X' && third == 'X')
		value = 5;
	else if (first == '.' && second == 'X' && third == '.')
		value = 6;
	else if (first == '.' && second == '.' && third == 'X')
		value = 7;
	else
		value = 8;

	int colour;

	if (value == 1)
		colour = ruleSet[0];
	else if (value == 2)
		colour = ruleSet[1];
	else if (value == 3)
		colour = ruleSet[2];
	else if (value == 4)
		colour = ruleSet[3];
	else if (value == 5)
		colour = ruleSet[4];
	else if (value == 6)
		colour = ruleSet[5];
	else if (value == 7)
		colour = ruleSet[6];
	else
		colour = ruleSet[7];

	if (colour == 0)
		return '.';
	else
		return 'X';
}

void setColour(char *parent, char *child, int *ruleSet)
{
	for(unsigned int i=0; i<99; i++)
	{
		if (i == 0)
		{
			child[i] = getColour(parent[98], parent[i], parent[i+1], ruleSet);
		}
		else if (i == 98)
		{
			child[i] = getColour(parent[i-1], parent[i], parent[0], ruleSet);
		}
		else
		{
			child[i] = getColour(parent[i-1], parent[i], parent[i+1], ruleSet);
		}
	}
}

void printToFile(char* parent, char *child, int* ruleSet, int genCount, int rowCount)
{
	int counter = 1;

	ofstream output;
	output.open("Output.txt");
	for(int i=0; i<rowCount; i++)
	{
		output << parent[i];
	}
	
	output << "\n";
		
	while (counter < genCount)
	{
		setColour(parent, child, ruleSet);
		for(int i=0; i<rowCount; i++)
		{
			output << child[i];
		}
		output << "\n";
		switchArrays(parent, child);
		counter++;
	}

	output.close();
	cout << "Success." << endl;
	return;
}

void printToScreen(char *parent, char *child, int* ruleSet, int genCount, int rowCount)
{
	int counter = 1;
	
	for(int i=0; i<rowCount; i++)
	{
		cout << parent[i];
	}

	cout << endl;

	while (counter < genCount)
	{
		setColour(parent, child, ruleSet);
		for(int i=0; i<rowCount; i++)
		{
			cout << child[i];
		}
		cout << endl;
		switchArrays(parent, child);
		counter++;
	}
	
	return;
}

void switchArrays(char* parent, char* child)
{
	for(unsigned int i=0; i<99; i++)
	{
		parent[i] = child[i];
	}
}

int* decimalToBinary(int ruleNum){

	static int binary[8];

	for (int i=0; i<8; i++)
		binary[i] = 0;

	while (ruleNum >= 1)
	{
		if (ruleNum / 128 >= 1)
		{
			binary[0] = 1;
			ruleNum = ruleNum - 128;
		}
		else if (ruleNum / 64 >= 1)
		{
			binary[1] = 1;
			ruleNum = ruleNum - 64;
		}
		else if (ruleNum / 32 >= 1)
		{
			binary[2] = 1;
			ruleNum = ruleNum - 32;
		}
		else if (ruleNum / 16 >= 1)
		{
			binary[3] = 1;
			ruleNum = ruleNum - 16;
		}
		else if (ruleNum / 8 >= 1)
		{
			binary[4] = 1;
			ruleNum = ruleNum - 8;
		}
		else if (ruleNum / 4 >= 1)
		{
			binary[5] = 1;
			ruleNum = ruleNum - 4;
		}
		else if (ruleNum / 2 >= 1)
		{
			binary[6] = 1;
			ruleNum = ruleNum - 2;
		}
		else if (ruleNum / 1 >= 1)
		{
			binary [7] = 1;
			ruleNum = ruleNum - 1;
		}
	}

	return binary;
}

int getDecimal()
{
	int decimal;
	cout << "Enter a decimal value between 0 and 255 to be converted into binary." << endl;
	cin >> decimal;

	while (decimal < 0 || decimal > 255)
	{
		cout << "Your number must be between 0 and 255." << endl;
		cin >> decimal;
	}

	return decimal;
}

int* binaryToArray(string input)
{
	static int binary[8];
	
	for (unsigned int i=0; i<input.length(); i++)
	{
		if (input[i] == '0')
		{
			binary[i] = 0;	
		}
		else
		{
			binary[i] = 1;	
		}
	}
	
	return binary;
}
