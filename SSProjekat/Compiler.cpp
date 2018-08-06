#include "Compiler.h"
#include "UtilFunctions.h"

using namespace std;

map<string, regex> Compiler::regexMap = {
	{"END", regex(".end")},
	{"ANYTHING", regex("")},
	{"LINEBREAK", regex("\\r")},
	{"LABEL", regex("^([a-zA-Z_][a-zA-Z0-9]*):$") },
	{"SECTION", regex("^\.(text|data|bss|rodata)$")},
	{"DIRECTIVE", regex("^\.(char|word|long|skip|align)$")},
	{"INSTRUCTION", regex("^(eq|ne|gt|al)(add|sub|mul|div|cmp|and|or|not|test|push|pop|call|iret|mov|shl|shr)$")},
};

Compiler::Compiler() {
	table = new SymbolTable();
	currentSection = "";
	locationCounter = 0;
	number = 0;
	startOfCurrentSection = 0;
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
				if (sym != 0) throw new runtime_error("ERROR: Section can't be defined more tah once!");

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
				string labelName = words[i];

				Symbol* sym = table->get(labelName);
				if (sym != 0) {
					if (sym->getLocGlo() == "local") throw new runtime_error("ERROR: There can't be two or more symbols with the same name!");
					else sym->setOffset(locationCounter);
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
					int k;
					try {
						int k = stoi(words[i]);
					}
					catch (exception e) {
						throw new runtime_error("ERROR: Invalid argument for directives .skip or .align!");
					}
					if (name == ".skip") locationCounter += k;
					else if (name == ".align") {
						if (k == 0) continue;
						if (k & (k - 1) == 0) {
							if (locationCounter / k * k != locationCounter) locationCounter = (locationCounter / k + 1) * k;
						}
						else throw new runtime_error("ERROR: Invalid argument for .skip directive, argument must be a power of 2");
					}
				}
				else if (name == ".char" || name == ".word" || name == ".long") {
					int size = UtilFunctions::getDirectiveSize(name);
					int r;
					while (words[i] != "\n" || words[i] != "\r") {
						if (words[i] != " ")r++;
						i++;
					}
					i--;
					locationCounter += r * size;
				}
				continue;
			}

			else if (regex_search(words[i], regexMap["INSTRUCTION"])) {
					
				
			}

			else continue;
		}


	}
	Section* s = new Section(currentSection, startOfCurrentSection, locationCounter);
	sections.push_back(*s);
}

void Compiler::secondRun() {


}

void Compiler::writeObjectFile(ofstream &outFile) {
	

}

