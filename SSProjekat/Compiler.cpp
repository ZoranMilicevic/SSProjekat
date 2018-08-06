#include "Compiler.h"
#include "UtilFunctions.h"

using namespace std;

map<string, regex> Compiler::regexMap = {
	{"END", regex(".end")},
	{"ANYTHING", regex("")},
	{"LINEBREAK", regex("\\r")},
	{"LABEL", regex("^([a-zA-Z_][a-zA-Z0-9]*):$") },
	{"SECTION", regex("^\.(text|data|bss|rodata)")},
};

Compiler::Compiler() {
	table = new SymbolTable();
}

Compiler::~Compiler() {
	delete table;
}


void Compiler::compile(ifstream &inFile, ofstream &outFile) {
	try{
		firstRun(inFile);
		secondRun();
		writeObjectFile(outFile);
	}
	catch (exception &e) {
		cerr << e.what() << endl;
	}
}

void Compiler::firstRun(ifstream &inFile) {
	string line;
	smatch sm;
	while (getline(inFile, line)) {
		vector<string> words = UtilFunctions::split(line);

		for (vector<string>::size_type i = 0; i != words.size(); i++) {

			if (words[i] == "\n" || words[i] == "\r")break; //if end of line
			else if (words[i] == " ") continue; //if there are more than one spaces
			else if (words[i] == ".end")return; //if end of 

			else if (regex_search(words[i], regexMap["SECTION"])) {
				string labelName = words[i];

				Symbol* sym = table->get(labelName);
				if (sym != 0) throw new runtime_error("ERROR: ")
			}

			else if (regex_search(words[i], regexMap["LABEL"])) {
				string labelName = words[i];

				Symbol* sym = table->get(labelName);
				if (sym != 0) {
					if (sym->getLocGlo() == "local") throw new exception("EXCEPTION: There can't be two or more symbols with the same name!");
					else sym->setOffset(locationCounter);
				}
				else {
					Symbol* newSym = new Symbol(labelName, currentSection, locationCounter, "local", number);
					number++;
				}
				continue;
			}

			



		}
		

	}

}

void Compiler::secondRun() {


}

void Compiler::writeObjectFile(ofstream &outFile) {
	

}

