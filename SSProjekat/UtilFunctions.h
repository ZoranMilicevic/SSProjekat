#ifndef UTILFUNCTIONS_H
#define UTILFUNCTIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <bitset>
#include <vector>
#include <sstream>

using namespace std;

class UtilFunctions{
public:
	static vector<string> split(string line);
	static int getSectionNumber(string section);
	static int getDirectiveSize(string directive);

	static string decimalToBinary(int number);
	static string decimalToHexa(int number);
	static string binaryToHexa(string binary);
}

#endif

