#include "RelocationSymbolTable.h"

using namespace std;

bool RelocationSymbolTable::put(string key, RelocationSymbol* sym) {
	vector<RelocationSymbol> rs = table[key];
	rs.push_back(*sym);
	return 1;
}

vector<RelocationSymbol> RelocationSymbolTable::get(string key) {
	vector<RelocationSymbol> sym = table[key];
	return sym;
}

void RelocationSymbolTable::print() {

}