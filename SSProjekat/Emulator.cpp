#include "Emulator.h"
#include <sstream>
#include "UtilFunctions.h"
#include "RelocationSymbol.h"
#include "RelocationSymbolTable.h"

using namespace std;

vector<string> Emulator::split(string line) {
	vector<string> ret;
	string word = "";

	for (int i = 0; i < line.size(); i++) {
		if (line.at(i) != ' ' && line.at(i) != ',' && line.at(i)!='\t' )word += line.at(i);
		else if (word != "" && word != "\n" && word!="\t\t") {
			ret.push_back(word);
			word = "";
		}
	}

	if (word != "" && word != "\n" && word!="\t\t") {
		ret.push_back(word);
	}

	return ret;
}

void Emulator::load(int argc, char** argv) {

	for (int i = 1; i < argc - 1; i++) {
		sections.push_back(new Section());
		sections.push_back(new Section());
		sections.push_back(new Section());
		sections.push_back(new Section());
		createSymbolTable(argv[i]);
		sections.clear();
	}

	for (int i = 1; i < argc - 1; i++) {
		sections.push_back(new Section());
		sections.push_back(new Section());
		sections.push_back(new Section());
		sections.push_back(new Section());
		resolveRelocation(argv[i]);
		sections.clear();
	}
}

void Emulator::createSymbolTable(string name) {
	ifstream inFile(name);

	if (!inFile.is_open()) {
		throw new runtime_error("ERROR: There was an error while opening input file");
	}

	string line = "";
	getline(inFile, line);
	line = line.substr(0, line.size());

	if (line == "#Section_table") {
		getline(inFile, line); //read table header

		while (getline(inFile, line)) {
			if (line == "") break;
			vector<string> words = split(line);

			string sectionName = words[0];
			int start = stoi(words[1]);
			int length = stoi(words[2]);

			int n = UtilFunctions::getSectionNumber(sectionName);
			Section * s = new Section(sectionName, start, length);
			sections[n - 1] = s;
		}
	}

	getline(inFile, line);
	line = line.substr(0, line.size());

	if (line == "#Symbol_table") {
		getline(inFile, line);
		getline(inFile, line); //read table headers

		while (getline(inFile, line)) {
			if (line == "") break;
			vector<string> words = split(line);
			if (words[0] == ".text" || words[0] == ".bss" || words[0] == ".rodata" || words[0] == ".data")continue;

			string symName = words[0];
			string section = words[1];
			int offset = stoi(words[2]);
			string locGlo = words[3];
			int num = stoi(words[4]);

			if (locGlo == "global" && section != "UND") {
				Symbol* sym = table.get(symName);
				if (sym != 0)throw new runtime_error("ERROR: Symbol already defined!");
				else {
					int n = UtilFunctions::getSectionNumber(section);
					int start = sections[n - 1]->getStart();
					offset = offset + start;
					sym = new Symbol(symName, section, offset, locGlo, num);
					table.put(symName, sym);
				}
			}
		}
	}
}

void Emulator::resolveRelocation(string name) {
	ifstream inFile(name);
	inFile.clear();
	inFile.seekg(0, ios::beg);

	if (!inFile.is_open()) {
		throw new runtime_error("ERROR: There was an error while opening input file");
	}

	string line = "";
	SymbolTable* localTable = new SymbolTable();
	RelocationSymbolTable* relocationTable = new RelocationSymbolTable();
	string genCode[4];
	getline(inFile, line);
	line = line.substr(0, line.size());

	//CREATE SECTION TABLE
	if (line == "#Section_table") {
		getline(inFile, line); //read table header

		while (getline(inFile, line)) {
			if (line == "") break;
			vector<string> words = split(line);

			string sectionName = words[0];
			int start = stoi(words[1]);
			int length = stoi(words[2]);

			int n = UtilFunctions::getSectionNumber(sectionName);
			Section * s = new Section(sectionName, start, length);
			sections[n - 1] = s;
		}
	}

	getline(inFile, line);
	line = line.substr(0, line.size());

	//CREATE LOCAL SYMBOL TABLE
	if (line == "#Symbol_table") {
		getline(inFile, line);
		getline(inFile, line); //read table headers

		while (getline(inFile, line)) {
			if (line == "") break;
			vector<string> words = split(line);
			

			string symName = words[0];
			string section = words[1];
			int offset = stoi(words[2]);
			string locGlo = words[3];
			int num = stoi(words[4]);

			if (section != "UND") {
				int n = UtilFunctions::getSectionNumber(section);
				if (symName == ".text" || symName == ".data" || symName == ".rodata" || symName == ".bss") offset = 0;
				offset = offset + sections[n - 1]->getStart();
				if (symName == "START") {
					START = offset;
				}
			}
			
			Symbol *sym = new Symbol(symName, section, offset, locGlo, num);
			localTable->put(symName, sym);
		}
	}	

	//RESOLVE RELOCATIONS
	while (getline(inFile, line)) {
		line = line.substr(0, line.size());
		if (line == "#.rel.data" || line == "#.rel.text" || line == "#.rel.rodata" || line == "#.rel.bss") {
			int l;
			string sec;
			if (line == "#.rel.text") { l = 0; sec = ".text"; }
			else if (line == "#.rel.data") { l = 1; sec = ".data"; }
			else if (line == "#.rel.rodata") { l = 2; sec = ".rodata"; }
			else if (line == "#.rel.bss") { l = 3; sec = ".bss"; }
			int start = sections[l]->getStart();

			while (getline(inFile, line)) {
				if (line == "")break;
				vector<string> words = split(line);

				string address = words[0];
				int adr = UtilFunctions::hexToDecimal(address);
				string type = words[1];
				int num = stoi(words[2]);

				RelocationSymbol *rs = new RelocationSymbol(to_string(adr), type=="R_386_PC32", num);
				relocationTable->put(sec, rs);
			}
		}
		else if (line == "#.data" || line == "#.text" || line == "#.rodata" || line == "#.bss") {
			string sec = line.substr(1, line.size());
			string code;
			getline(inFile, code);
			vector<RelocationSymbol> rels = relocationTable->get(sec);
			for (int i = 0; i < rels.size(); i++) {
				RelocationSymbol r = rels[i];
				int addr = stoi(r.getAddress());
				bool type = r.getType();
				int num = r.getNumber();

				if (type) {	//PCREL
					char pod[4] = { code[2*addr + 2], code[2*addr + 3], code[2*addr], code[2*addr + 1] };
					string dat(pod);
					dat = dat.substr(0, 4);
					int d = UtilFunctions::hexToDecimal(dat); //GET THE DATA IN INT
					Symbol* sym = localTable->getByNum(num);
					if (sym->getSection() != "UND") {	//LOCAL
						d += sym->getOffset();
					}
					else {		//GLOBAL
						sym = table.get(sym->getLabel());
						if (sym == 0)throw runtime_error("ERROR: There is an unresolved symbol");
						d += sym->getOffset();
					}
					int n = UtilFunctions::getSectionNumber(sec);
					d = d - addr - sections[n - 1]->getStart();
					string g = UtilFunctions::generateCode(d, 2);
					code[2*addr] = g[0];
					code[2*addr + 1] = g[1];
					code[2*addr + 2] = g[2];
					code[2*addr + 3] = g[3];
				}
				else { //ABS
					char pod[4] = { code[2*addr + 2], code[2*addr + 3], code[2*addr], code[2*addr + 1] };
					string dat(pod);
					dat = dat.substr(0, 4);
					int d = UtilFunctions::hexToDecimal(dat);
					Symbol* sym = localTable->getByNum(num);
					if (sym->getSection() != "UND") {	//LOCAL
						d += sym->getOffset();
					}
					else {		//GLOBAL
						sym = table.get(sym->getLabel());
						if (sym == 0)throw runtime_error("ERROR: There is an unresolved symbol");
						d += sym->getOffset();
					}
					string g = UtilFunctions::generateCode(d, 2);
					code[2*addr] = g[0];
					code[2*addr + 1] = g[1];
					code[2*addr + 2] = g[2];
					code[2*addr + 3] = g[3];
				}
			}
			genCode[UtilFunctions::getSectionNumber(sec) - 1] = code;
		}	
	}
	writeToMemory(genCode, sections);
}

void Emulator::writeToMemory(string* genCode, vector<Section*> sections) {
	for (int i = 0; i < sections.size()-1; i++) {
		Section * sec = sections[i];
		int start = sec->getStart();
		int length = sec->getLength();
		string name = sec->getName();
		if (name == "")continue;
		int n = UtilFunctions::getSectionNumber(name);
		string code = genCode[n - 1];
		int k = 0;
		for (int j = 0; j < length; j++) {
			char p[] = { code[k], code[k + 1] };
			string s(p);
			s = s.substr(0, 2);
			mem.writeRamByte(start, s);
			start++; 
			k += 2;
		}
	}
}


void Emulator::run() {
	bool end = false;
	Cpu* c = new Cpu(&mem);
	Ivt* ivt = new Ivt(&mem);

	for (int i = 0; i < 6; i++)c->regs[i] = 0;
	c->regs[Cpu::PSW] = 0;
	c->regs[Cpu::SP] = 1001; 
	c->regs[Cpu::PC] = START;

	if (START == -1)throw new runtime_error("ERROR: START symbol not defined");

	while (!end) {
		bool b = c->decodeAndExec();
		if (c->regs[Cpu::PC] > 209)break;
	}


	//WRITE TO FILE
	ofstream out("C:\\Testovi\\emulOutput.txt");
	mem.print(out);
	out << endl;
	for (int i = 0; i < 9; i++)out << "r" << i << " = " << c->regs[i] << endl;
}