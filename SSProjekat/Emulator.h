#ifndef EMULATOR_H
#define EMULATOR_H

#include <vector>
#include <fstream>
#include "SymbolTable.h"
#include "Section.h"
#include "Memory.h"
#include "Cpu.h"
#include "Ivt.h"

using namespace std;


class Emulator {
private:
	int START=-1;
	SymbolTable table;
	vector<Section*> sections;
	Memory mem;

	vector<string> split(string line);
	void createSymbolTable(string name);
	void resolveRelocation(string name);
	void writeToMemory(string*, vector<Section*>);
	int hexToDecimal(string num);
	

public:
	Emulator() {};
	~Emulator() {};

	void load(int, char**);
	void run();


};

#endif 