#ifndef COMPILER_H
#define COMPILER_H
#include <unordered_map> 
#include <string>
#include<regex>
#include <vector>

#include "SymbolTable.h"
#include "RelocationSymbolTable.h"
#include "Section.h"

using namespace std;

class Compiler {
public:
	Compiler();
	~Compiler();

	void compile(ifstream &inFIle, ofstream &outFile);

	static map<string, regex> regexMap;

private:
	void firstRun(ifstream &inFile);
	void secondRun(ifstream &inFile);

	string currentSection;
	int startOfCurrentSection;
	int number;
	int locationCounter;
	
	SymbolTable * table;
	vector<Section> sections;

	RelocationSymbolTable* relocationTable;
	map<string, string> generatedCode;


};


#endif
