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
	static int getDirectiveSize(string directive);

	static int getSectionNumber(string section);
	static string binaryToHexa(string binary);
	static string decimalToHexa(int number);
	static string decimalToBinary(int number);

};

#endif

