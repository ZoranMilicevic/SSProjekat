#ifndef _RELOCATIONSYMBOLTABLE_H
#define _RELOCATIONSYMBOLTABLE_H

#include "RelocationSymbol.h"
#include <map>
#include <string>

using namespace std;

class RelocationSymbolTable {
private:
	map<string, RelocationSymbol*> table;

public:
	RelocationSymbolTable() {}
	~RelocationSymbolTable() {}

	bool put(string key, RelocationSymbol* sym);
	RelocationSymbol* get(string key);

	void print();



};

#endif