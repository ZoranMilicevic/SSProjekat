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

	void compile(ifstream &inFIle, ofstream &outFile, int startAddress);

	static map<string, regex> regexMap;

private:
	void firstRun(ifstream &inFile);
	void secondRun(ifstream &inFile);

	void process_first_operand(string* operation, string* op, string* src, bool* flag, string* value);
	void process_second_operand(string* operation, string* op, string* src, bool* flag, string* value);
	string findAddressing(string op);

	string currentSection;
	int number;
	int locationCounter;
	int startOfCurSec;
	
	SymbolTable * table;
	vector<Section> sections;

	RelocationSymbolTable* relocationTable;
	map<string, string> generatedCode;


};


#endif
