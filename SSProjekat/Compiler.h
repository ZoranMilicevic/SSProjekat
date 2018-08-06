#ifndef COMPILER_H
#define COMPILER_H
#include <unordered_map> 
#include <string>
#include<regex>

#include "SymbolTable.h"

using namespace std;

class Compiler {
public:
	Compiler();
	~Compiler();

	void compile(ifstream &inFIle, ofstream &outFile);

	static map<string, regex> regexMap;

private:
	void firstRun(ifstream &inFile);
	void secondRun();
	void writeObjectFile(ofstream &outFile);

	string currentSection;
	int startOfCurrentSection;
	int number;
	int locationCounter;
	
	SymbolTable * table;



};


#endif
