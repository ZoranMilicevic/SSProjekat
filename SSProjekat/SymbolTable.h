#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <map>
#include <iostream>

#include "Symbol.h"

using namespace std;

class SymbolTable {
private:
	map<string, Symbol*> table;

public:
	SymbolTable() {}
	~SymbolTable() {}

	bool put(string key, Symbol* sym);
	Symbol* get(string key);

	void print();
};


#endif
