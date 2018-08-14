#include "RelocationSymbolTable.h"

using namespace std;

bool RelocationSymbolTable::put(string key, RelocationSymbol* sym) {
	RelocationSymbol* rs = table[key];
	if (rs != 0) {
		return 0;
	}
	table[key] = sym;
	return 1;
}

RelocationSymbol* RelocationSymbolTable::get(string key) {
	RelocationSymbol* sym = table[key];

	if (sym == 0)return 0;
	return sym;
}

void RelocationSymbolTable::print() {

}