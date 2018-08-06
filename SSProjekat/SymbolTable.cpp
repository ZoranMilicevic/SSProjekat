#include "SymbolTable.h"

bool SymbolTable::put(string key, Symbol* sym) {
	Symbol* s = table[key];
	if (s != 0) {
		return 0;
	}
	table[key] = sym;
	return 1;
}

Symbol* SymbolTable::get(string key) {
	Symbol* sym = table[key];

	if (sym == 0)return 0;
	return sym;
}