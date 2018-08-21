#include "RelocationSymbolTable.h"
#include <fstream>

using namespace std;

RelocationSymbolTable::RelocationSymbolTable() {
	vector<RelocationSymbol> v1;
	vector<RelocationSymbol> v2;
	vector<RelocationSymbol> v3;
	vector<RelocationSymbol> v4;
	table = {
		{".text", v1},
		{".data", v2},
		{".bss", v3},
		{".rodata", v4}
	};

}

bool RelocationSymbolTable::put(string key, RelocationSymbol* sym) {
	table[key].push_back(*sym);
	//rs.push_back(*sym);
	return 1;
}

vector<RelocationSymbol> RelocationSymbolTable::get(string key) {
	vector<RelocationSymbol> sym = table[key];
	return sym;
}

void RelocationSymbolTable::print(ofstream& outFile) {
	for (map<string, vector<RelocationSymbol>>::iterator iter = table.begin(); iter != table.end(); iter++) {
		vector<RelocationSymbol> v = iter->second;
		if (v.size() > 0) outFile << endl << "#.rel" << iter->first << endl;
		for (int i = 0; i < v.size(); i++) {
			RelocationSymbol rs = v.at(i);
			string addressing;
			if (rs.getType()) addressing = "R_386_PC32";
			else addressing = "R_386_32";

			outFile << rs.getAddress() << "\t\t" << addressing << "\t\t" << rs.getNumber() << endl;
		}
	}

}