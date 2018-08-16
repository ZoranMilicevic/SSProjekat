#ifndef _RELOCATIONSYMBOLTABLE_H
#define _RELOCATIONSYMBOLTABLE_H

#include "RelocationSymbol.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class RelocationSymbolTable {
private:
	map<string, vector<RelocationSymbol>> table;

public:
	RelocationSymbolTable() {}
	~RelocationSymbolTable() {}

	bool put(string key, RelocationSymbol* sym);
	vector<RelocationSymbol> get(string key);

	void print();



};

#endif