#include "Compiler.h"
#include "UtilFunctions.h"
#include "Instruction.h"

#include <iostream>
#include <sstream>

using namespace std;

map<string, regex> Compiler::regexMap = {
	{"END", regex(".end")},
	{"ANYTHING", regex("")},
	{"LINEBREAK", regex("\\r")},
	{"LABEL", regex("^([a-zA-Z_][a-zA-Z0-9]*):$") },
	{"SECTION", regex("^\\.(text|data|bss|rodata)$")},
	{"DIRECTIVE", regex("^\\.(char|word|long|skip|align)$")},
	{"GLOBAL", regex("^\\.global$")},
	{"INSTRUCTION", regex("^(eq|ne|gt|al)(add|sub|mul|div|cmp|and|or|not|test|push|pop|call|iret|mov|shl|shr|ret|jmp)$")},
	{"ADDRESSING_SIGNS", regex("[\\&|\\*|\\[|\\$]")},
	{"IMMEDIATE_ADDRESSING", regex("^([0-9]+)$")}
};

Compiler::Compiler() {
	table = new SymbolTable();
	relocationTable = new RelocationSymbolTable();
	currentSection = "";
	locationCounter = 0;
	number = 0;
	startOfCurrentSection = 0;

	generatedCode = {
		{".text", ""},
		{".data", ""},
		{".rodata", ""}
	};
}

Compiler::~Compiler() {
	delete table;
}


void Compiler::compile(ifstream &inFile, ofstream &outFile) {
	try{
		firstRun(inFile);
		table->print();
		secondRun(inFile);
	}
	catch (exception &e) {
		cout << e.what() << endl;
	}
}

void Compiler::firstRun(ifstream &inFile) {
	string line;
	while (getline(inFile, line)) {
		vector<string> words = UtilFunctions::split(line);

		for (vector<string>::size_type i = 0; i < words.size(); i++) {

			if (words[i] == "\n" || words[i] == "\r")break; //if end of line
			else if (words[i] == " ") continue; //if there are more than one spaces
			else if (words[i] == ".end")return; //if end of 

			else if (regex_search(words[i], regexMap["SECTION"])) {
				string labelName = words[i];

				Symbol* sym = table->get(labelName);
				if (sym != 0) throw new runtime_error("ERROR: Section can't be defined more than once!");

				//SAVE THE CURRENT SECTION
				if (currentSection != "") {
					Section* s = new Section(currentSection, startOfCurrentSection, locationCounter);
					sections.push_back(*s);
				}

				//NEW SECTION
				startOfCurrentSection = locationCounter;
				currentSection = labelName;
				Symbol* newSym = new Symbol(labelName, currentSection, locationCounter, "local", number);
				number++;
				table->put(labelName, newSym);

				continue;
			}

			else if (regex_search(words[i], regexMap["LABEL"])) {
				string labelName = words[i].substr(0, words[i].size() - 1);

				Symbol* sym = table->get(labelName);
				if (sym != 0) {
					if (sym->getLocGlo() == "local") throw new runtime_error("ERROR: There can't be two or more symbols with the same name!");
					else {
						sym->setOffset(locationCounter);
						sym->setSection(currentSection);
						sym->setNumber(number);
						sym->setLocGlo("global");
					}
				}
				else {
					Symbol* newSym = new Symbol(labelName, currentSection, locationCounter, "local", number);
					number++;
					table->put(labelName, newSym);
				}
				continue;
			}

			else if (regex_search(words[i], regexMap["DIRECTIVE"])) {
				string name = words[i];
				if (name == ".skip" || name == ".align") {
					i++;
					int k=0;
					try {
						k = stoi(words[i]);
					}
					catch (exception e) {
						throw new runtime_error("ERROR: Invalid argument for directives .skip or .align!");
					}
					if (name == ".skip") locationCounter += k;
					else if (name == ".align") {
						if (k == 0) continue;
						if ((k & (k - 1)) == 0) {
							if (locationCounter / k * k != locationCounter) locationCounter = (locationCounter / k + 1) * k;
						}
						else throw new runtime_error("ERROR: Invalid argument for .skip directive, argument must be a power of 2");
					}
				}
				else if (name == ".char" || name == ".word" || name == ".long") {
					int size = UtilFunctions::getDirectiveSize(name);
					int k =words.size() - i - 1;
					locationCounter += k * size;
				}
				break;
			}

			else if (regex_search(words[i], regexMap["GLOBAL"])) {
				for (int k = i + 1; k < words.size(); k++) {
					string labelName = words[k];
					Symbol* sym = table->get(labelName);

					if (sym != 0) sym->setLocGlo("global");
					else {
						sym = new Symbol(labelName, currentSection, -1, "global", number);
						table->put(labelName, sym);
					}

				}
			}

			else if (regex_search(words[i], regexMap["INSTRUCTION"])) {
				locationCounter += 2;
				for (int k = i + 1; k < words.size(); k++) {
					if ( regex_search(words[k], regexMap["ADDRESSING_SIGNS"]) || regex_search(words[k], regexMap["IMMEDIATE_ADDRESSING"])) {
						locationCounter += 2;
						break;
					}
				}
				break;
			}

			else continue;
		}

	}
	Section* s = new Section(currentSection, startOfCurrentSection, locationCounter);
	sections.push_back(*s);
}

void Compiler::secondRun(ifstream &inFile) {
	string line;
	currentSection = "";
	locationCounter = 0;
	number = 0;
	startOfCurrentSection = 0;

	while (getline(inFile, line)) {
		vector<string> words = UtilFunctions::split(line);
		for (vector<string>::size_type i = 0; i < words.size(); i++) {
			if (regex_search(words[i], regexMap["INSTRUCTION"])) {
				if (currentSection != ".text") {
					throw new runtime_error("ERROR: Instructions must be in .text section");
					return;
				}
				else if (words[i]=="eqiret" || words[i] == "neiret" || words[i] =="gtiret" || words[i] == "aliret") {
					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + "0000000000");
					string gen = generatedCode[currentSection];
					gen = gen + code;
					break;
				}
			
			}

			if (regex_search(words[i], regexMap["SECTION"])) {
				startOfCurrentSection = locationCounter;
				currentSection = words[i];
				continue;
			}

			else if (regex_search(words[i], regexMap["DIRECTIVE"])) {
				int size = UtilFunctions::getDirectiveSize(words[i]);
				string name = words[i];
				if (name == ".skip" || name == ".align") {
					i++;
					int k = 0;
					try {
						k = stoi(words[i]);
					}
					catch (exception e) {
						throw new runtime_error("ERROR: Invalid argument for directives .skip or .align!");
					}
					if (name == ".skip") locationCounter += k;
					else if (name == ".align") {
						if (k == 0) continue;
						if ((k & (k - 1)) == 0) {
							if (locationCounter / k * k != locationCounter) locationCounter = (locationCounter / k + 1) * k;
						}
						else throw new runtime_error("ERROR: Invalid argument for .skip directive, argument must be a power of 2");
					}
				}
				else if (name == ".char" || name == ".word" || name == ".long") {
					
				
				}

			}
			else continue;
		
		}
	}

}


