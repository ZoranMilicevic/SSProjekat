#include "SymbolTable.h"
#include <fstream>

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

void SymbolTable::print(ofstream& outFile) {
	outFile << "Label" << "\t\t" << "Section" << "\t\t" << "offset" << "\t\t" << "LocGlo" << "\t\t" << "number" << endl;
	outFile << "--------------------------------------------------------------------------" << endl;
	for (map<string, Symbol*>::iterator it = table.begin(); it != table.end(); it++ ) {
		Symbol * s = it->second;
		outFile << s->getLabel() << "\t\t" << s->getSection() << "\t\t" << s->getOffset() << "\t\t" << s->getLocGlo() << "\t\t" << s->getNumber() << endl;
	}
	
}