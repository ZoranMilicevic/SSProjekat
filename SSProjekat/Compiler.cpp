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


	{ "NUMBER", regex("^([0-9]+)$")},
	{ "ARITMETICAL", regex("^(eq|ne|gt|al)(add|sub|mul|div|and|or|not|shl|shr|mov)$")},
	{ "LOGICAL", regex("^(eq|ne|gt|al)(cmp|test)$") },
	{ "PUSHCALL", regex("^(eq|ne|gt|al)(call|push)$") },

	{ "OPERAND_DEC", regex("^([0-9]+)$") },
	{ "OPERAND_HEX", regex("^(0x[0-9abcdef]+)$") },
	{ "OPERAND_REG", regex("^r([0-7])$") },
	{ "OPERAND_REGSPEC", regex("^(pc|sp)$") },
	{ "OPERAND_PSW", regex("^(psw)$")},
	{ "OPERAND_SYMBOL", regex("^\\&([a-zA-Z_][a-zA-Z0-9]*)$") },
	{ "OPERAND_MEMDIR", regex("^([a-zA-Z_][a-zA-Z0-9]*)$") },
	{ "OPERAND_IMMADDR", regex("^\\*([0-9]+)$") },
	{ "OPERAND_IMMADDRHEX", regex("^\\*(0x[0-9abcdef]+)$") },
	{ "OPERAND_PCREL", regex("^\\$([a-zA-Z_][a-zA-Z0-9]*)$") },
	{ "OPERAND_REGINDPOM", regex("^r([0-7])\\[") },
};

Compiler::Compiler() {
	table = new SymbolTable();
	relocationTable = new RelocationSymbolTable();
	currentSection = "";
	locationCounter = 0;
	startOfCurSec = 0;
	number = 5;

	generatedCode = {
		{".text", ""},
		{".data", ""},
		{".rodata", ""}
	};
}

Compiler::~Compiler() {
	delete table;
}


void Compiler::compile(ifstream &inFile, ofstream &outFile, int startAddress) {
	try{
		startOfCurSec = startAddress;
		firstRun(inFile);

		inFile.clear();
		inFile.seekg(0, ios::beg);
		secondRun(inFile);

		writeToFile(outFile);

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
			else if (words[i] == ".end") {
				//SAVE THE LAST SECTION
				Section* s = new Section(currentSection, startOfCurSec, locationCounter);
				sections.push_back(*s);
				return;
			}

			else if (regex_search(words[i], regexMap["SECTION"])) {
				string labelName = words[i];

				Symbol* sym = table->get(labelName);
				if (sym != 0) throw new runtime_error("ERROR: Section can't be defined more than once!");

				//SAVE THE CURRENT SECTION
				if (currentSection != "") {
					Section* s = new Section(currentSection, startOfCurSec, locationCounter);
					sections.push_back(*s);
				}

				//NEW SECTION
				currentSection = labelName;
				Symbol* newSym = new Symbol(labelName, currentSection, locationCounter, "local", UtilFunctions::getSectionNumber(labelName));
				table->put(labelName, newSym);
				startOfCurSec += locationCounter;
				locationCounter = 0;
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
						sym = new Symbol(labelName, "UND", locationCounter, "global", number);
						number++;
						table->put(labelName, sym);
					}

				}
			}

			else if (regex_search(words[i], regexMap["INSTRUCTION"])) {
				locationCounter += 2;
				for (int k = i + 1; k < words.size(); k++) {
					string adr = findAddressing(words[k]);
					if (adr != "regDir" && adr !="regDirSpec" && adr!="psw" && adr!="not found") {
						locationCounter += 2;
						break;
					}
				}
				break;
			}

			else continue;
		}

	}
	//SAVE THE LAST SECTION
	Section* s = new Section(currentSection, 0, locationCounter);
	sections.push_back(*s);
}

void Compiler::secondRun(ifstream &inFile) {
	cout << "Second run begins" << endl << endl;
	string line;
	currentSection = "";
	locationCounter = 0;
	number = 5;

	while (getline(inFile, line)) {
		cout << endl << "Next line is: " << line << endl;
		vector<string> words = UtilFunctions::split(line);
		for (vector<string>::size_type i = 0; i < words.size(); i++) {
			if (regex_search(words[i], regexMap["INSTRUCTION"])) {

				if (currentSection != ".text") {
					throw new runtime_error("ERROR: Instructions must be in .text section");
					return;
				}

				else if (regex_search(words[i], regexMap["ARITMETICAL"])) { //add, sub, mul, div, and, or, not, shl, shr, mov
					if (words.size() < 3) {
						throw new runtime_error("ERROR: Wrong number of arguments for aritmetical operation");
					}
	
					string operation = "ARITMETICAL";
					string op1 = words[i + 1];
					string op2 = words[i + 2];
					string src = "";
					string dst = "";
					bool flag1 = false;
					bool flag2 = false;
					string value = "";

					process_first_operand(&operation, &op1, &src, &flag1, &value);
					process_second_operand(&operation, &op2, &dst, &flag2, &value);

					if (flag1 == true && flag2 == true)throw new runtime_error("ERROR: Only one operand can request aditional bytes to store data");

					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + src + dst);
					generatedCode[currentSection] = generatedCode[currentSection] + code + value;

					locationCounter += 2;
					if (flag1 == true || flag2 == true)locationCounter += 2;

					break;
				}

				else if (regex_search(words[i], regexMap["LOGICAL"])) {	//cmp, tst
					if (words.size() < 3) {
						throw new runtime_error("ERROR: Wrong number of arguments for aritmetical operation");
					}

					string operation = "LOGICAL";
					string op1 = words[i + 1];
					string op2 = words[i + 2];
					string src = "";
					string dst = "";
					bool flag1 = false;
					bool flag2 = false;
					string value = "";

					process_first_operand(&operation, &op1, &src, &flag1, &value);
					process_second_operand(&operation, &op2, &dst, &flag2, &value);

					if (flag1 == true && flag2 == true)throw new runtime_error("ERROR: Only one operand can request aditional bytes to store data");

					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + src + dst);
					generatedCode[currentSection] = generatedCode[currentSection] + code + value;

					locationCounter += 2;
					if (flag1 == true || flag2 == true)locationCounter += 2;

					break;
				}

				else if (regex_search(words[i], regexMap["PUSHCALL"])) {
					if (words.size() < 2) {
						throw new runtime_error("ERROR: Wrong number of arguments for aritmetical operation");
					}

					string operation = "PUSHCALL";
					string op1 = words[i + 1];
					string src = "";
					bool flag1 = false;
					string value = "";

					process_first_operand(&operation, &op1, &src, &flag1, &value);
					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + "00000" + src);
					generatedCode[currentSection] = generatedCode[currentSection] + code + value;

					locationCounter += 2;
					if (flag1 == true)locationCounter += 2;

					break;
				}

				else if (words[i] == "eqpop" || words[i] == "nepop" || words[i] == "gtpop" || words[i] == "alpop") {
					if (words.size() < 2) {
						throw new runtime_error("ERROR: Wrong number of arguments for aritmetical operation");
					}

					string operation = "POP";
					string op1 = words[i + 1];
					string dst = "";
					bool flag1 = false;
					string value = "";

					process_first_operand(&operation, &op1, &dst, &flag1, &value);
					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + dst + "00000");
					generatedCode[currentSection] = generatedCode[currentSection] + code + value;

					locationCounter += 2;
					if (flag1 == true)locationCounter += 2;

					break;
				
				}
				
				else if (words[i]=="eqiret" || words[i] == "neiret" || words[i] =="gtiret" || words[i] == "aliret") {
					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + "0000000000");
					generatedCode[currentSection]= generatedCode[currentSection] + code;
					locationCounter += 2;
					break;
				}	

				else if (words[i] == "eqret" || words[i] == "neret" || words[i] == "gtret" || words[i] == "alret") {
					//same as pop pc
					string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i]]->getOpcode() + "01111" + "00000"); //regdir i pc
					generatedCode[currentSection] = generatedCode[currentSection] + code;

					locationCounter += 2;
					break;
				}

				else if (words[i] == "eqjmp" || words[i] == "nejmp" || words[i] == "gtjmp" || words[i] == "aljmp") {
					if (words.size() < 2) {
						throw new runtime_error("ERROR: Wrong number of arguments for aritmetical operation");
					}

					string operation = "JMP";
					string op1 = words[i + 1];
					string dst = "";
					bool flag1 = false;
					string value = "";

					string addressing = findAddressing(op1);

					process_first_operand(&operation, &op1, &dst, &flag1, &value);
					
					if (addressing == "pcrel") {
						string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i] + "add"]->getOpcode() + "01111" + "10000"); //ADD r7, offset(x)
						generatedCode[currentSection] = generatedCode[currentSection] + code + value;
						cout << code << endl;
					}
					else {
						if (addressing == "regIndPom") {
							string opp = words[i + 1];
							int regNum = opp.at(1);
							if (regNum == 7) {
								string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i] + "add"]->getOpcode() + "01111" + "10000"); //ADD r7, offset(x)
								generatedCode[currentSection] = generatedCode[currentSection] + code + value;
							}
							else {
								string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i] + "mov"]->getOpcode() + "01111" + dst); //MOV r7, ...
								generatedCode[currentSection] = generatedCode[currentSection] + code + value;
							}
						}
						else {
							string code = UtilFunctions::binaryToHexa(Instruction::instructions[words[i] + "mov"]->getOpcode() + "01111" + dst); //MOV r7, ...
							generatedCode[currentSection] = generatedCode[currentSection] + code + value;
						}
					}
					
					locationCounter += 2;
					if (flag1 == true) locationCounter += 2;
					break;
				}
			
			}

			if (regex_search(words[i], regexMap["SECTION"])) {
				locationCounter = 0;
				currentSection = words[i];
				cout << "New section found " << words[i] << endl;
				continue;
			}

			else if (regex_search(words[i], regexMap["DIRECTIVE"])) {
				string name = words[i];
				if (name == ".skip" || name == ".align") {
					cout << "Skip or align" << endl;
					i++;
					int k = 0;
					try {
						k = stoi(words[i]);
					}
					catch (exception e) {
						throw new runtime_error("ERROR: Invalid argument for directives .skip or .align!");
					}
					if (name == ".skip") {
						locationCounter += k;
						string code = "";
						for (int i = 0; i < 2 * k; i++) {
							code += "0";
						}
						generatedCode[currentSection] += code;
					}
					else if (name == ".align") {
						if (k == 0) continue;
						int oldLc = locationCounter;
						if ((k & (k - 1)) == 0) {
							if (locationCounter / k * k != locationCounter) locationCounter = (locationCounter / k + 1) * k;
						}
						if (locationCounter - oldLc > 0) {
							int j = locationCounter - oldLc;
							string code = "";
							for (int i = 0; i < 2 * j; i++) {
								code = code + "0";
							}
							generatedCode[currentSection] += code;
						}
						else throw new runtime_error("ERROR: Invalid argument for .align directive, argument must be a power of 2");
					}
				}

				else if (name == ".char" || name == ".word" || name == ".long") {
					int size = UtilFunctions::getDirectiveSize(name);
					for (int k = i + 1; k < words.size(); k++) {
						//IF IT IS A NUMBER, IN DECIMAL
						if (regex_search(words[k], regexMap["NUMBER"])) {	
							int val = 0;
							try {
								val = stoi(words[k]);
							}
							catch (exception e) {
								throw new runtime_error("ERROR: Unexpected conversion error!");
							}
							string code = UtilFunctions::generateCode(val, size);
							generatedCode[currentSection]+=code;
							locationCounter += size;

							cout << "Directive with number in dec" << endl;
						}
						//NUMBER IN HEX
						else if (regex_search(words[k], regexMap["OPERAND_HEX"])) {
							string pom = words[k];
							pom = pom.substr(2, pom.size());
							while (pom.size() < 4) {
								pom = "0" + pom;
							}
							char arr[] = { pom[2], pom[3], pom[0], pom[1] };
							string code(arr);
							code = code.substr(0, 4);
							generatedCode[currentSection] += code;
							locationCounter += size;

							cout << "Directive with number in hex" << endl;
						}
						//IF IT IS A SYMBOL
						else {										
							Symbol * sym = table->get(words[i + 1]);
							if (sym == 0) {
								throw new runtime_error("ERROR: Symbol not defined");
							}
							else {
								if (sym->getLocGlo() == "global") {
									string code = UtilFunctions::generateCode(0, size);
									generatedCode[currentSection] += code;

									string address = UtilFunctions::decimalToHexa(locationCounter);
									RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
									relocationTable->put(currentSection, rels);

									cout << "Directive with global symbol " << sym->getLabel() <<endl;
								}
								else if (sym->getLocGlo() == "local") {
									int offset = sym->getOffset();
									string code = UtilFunctions::generateCode(offset, size);
									generatedCode[currentSection] +=code;

									string address = UtilFunctions::decimalToHexa(locationCounter);
									RelocationSymbol *rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(currentSection));
									relocationTable->put(currentSection, rels);
									locationCounter += size;

									cout << "Directive with local symbol " << sym->getLabel() << endl;
								}
							}
						}
					}
				
				} //for else .char .word .long



			}
			else continue;
		}
	}

}

void Compiler::process_first_operand(string* operation, string* op1, string* src, bool* flag1, string* value) {
	string addressing = findAddressing(*op1);

	if (addressing == "immediateDec" || addressing == "immediateHex") {
		if (*operation == "ARITMETICAL") throw new runtime_error("ERROR: First operand can't be a immediate value in artihmetical operations!");
		if(*operation == "POP")throw new runtime_error("ERROR: First operand can't be a immediate value in POP instruction!");
		*src = "00000";
		*flag1 = true;
		
		if (addressing == "immediateDec") { 
			int v = stoi(*op1);
			*value = UtilFunctions::generateCode(v, 2); 

			cout << "First operand is immidiateDec value is " << v << endl;
		}
		else {
			string opp = *op1;
			opp = opp.substr(3, opp.size());
			while (opp.size() < 4) {
				opp = "0" + opp;
			}
			char arr[] = { opp[2], opp[3], opp[0], opp[1] };
			string code(arr);
			code = code.substr(0, 4);
			*value = code; //little endian processor

			cout << "First operand is immidiateHex value is " << opp << endl;
		}
	}

	else if (addressing == "psw") {
		*src = "00111";
		
		cout << "First operand is psw" << endl;
	}

	else if(addressing == "regDir" || addressing == "regDirSpec"){
		if (addressing == "regDir") {
			string opp = *op1;
			int regNum = opp.at(1) - '0'; //register number
			string reg = UtilFunctions::decimalToBinary(regNum);
			*src = "01" + reg;
			 
			cout << "First operand is regDir with register " << regNum << endl;
		}
		else {
			string opp = *op1;
			if (opp == "sp") *src = "01110"; //r6
			else if (opp == "pc") *src = "01111"; //r7

			cout << "First operand is regSpec with register " << opp << endl;
		}
	}

	else if (addressing == "immAddr" || addressing == "immAddrHex") {
		*src = "10000";
		*flag1 = true;

		if (addressing == "immAddr") {
			string opp = *op1;
			string num = opp.substr(1, opp.size());
			int v = stoi(num);
			*value = UtilFunctions::generateCode(v, 2);

			cout << "First operand is immAddr with value " << v << endl;
		}
		else {
			string opp = *op1;
			opp = opp.substr(3, opp.size());
			while (opp.size() < 4) {
				opp = "0" + opp;
			}
			char arr[] = { opp[2], opp[3], opp[0], opp[1] };
			string code(arr);
			code = code.substr(0, 4);
			*value = code;

			cout << "First operand is immAddrHex with value " << opp << endl;
		}
	}

	else if (addressing == "memDir") {
		*src = "10000";
		*flag1 = true;
		string symName = *op1;
		Symbol * sym = table->get(symName);
		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset(), 2);
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection()));
				relocationTable->put(currentSection, rels);

				cout << "First operand is memDir on local symbol " << sym->getLabel() << endl;
			}
			else {  //global
				*value = "0000";
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
				relocationTable->put(currentSection, rels);

				cout << "First operand is memDir on global symbol " << sym->getLabel() << endl;
			}
		}
	}

	else if (addressing == "symVal") {
		if (*operation == "ARITMETICAL") throw new runtime_error("ERROR: First operand can't be a immediate value in artihmetical operations!");
		if (*operation == "POP")throw new runtime_error("ERROR: First operand can't be a immediate value in POP instruction!");
		*src = "00000";
		*flag1 = true;
		string opp = *op1;
		string symName = opp.substr(1, opp.size());
		Symbol* sym = table->get(symName);

		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset(), 2);
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection()));
				relocationTable->put(currentSection, rels);

				cout << "First operand is symVal on local symbol " << sym->getLabel() << endl;
			}
			else {  //global
				*value = "0000";
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
				relocationTable->put(currentSection, rels);

				cout << "First operand is symVal on global symbol " << sym->getLabel() << endl;
			}
		
		}
	}

	else if (addressing == "regIndPom") {
		*flag1 = true;
		string opp = *op1;

		string pom = "";
		int regNum;
		bool found = false;
		for (int i = 0; i < opp.size(); i++) {
			if (opp[i] == '[') {
				found = true;
				regNum = opp.at(i - 1) - '0';
				continue;
			}
			if (opp[i] == ']')found = false;
			if (found == true && opp[i]!='[') {
				pom += opp[i];
			}
		}

		string reg = UtilFunctions::decimalToBinary(regNum);
		*src = "11" + reg;

		if (regex_search(pom, regexMap["OPERAND_DEC"]) || regex_search(pom, regexMap["OPERAND_HEX"])) {
			if (regex_search(pom, regexMap["OPERAND_DEC"])) {
				int v = stoi(pom);
				*value = UtilFunctions::generateCode(v, 2);

				cout << "First operand is regIndPom with immediate pomc in dec " << pom << " and register " << regNum << endl;
			}
			else {
				pom = pom.substr(3, pom.size());
				while (pom.size() < 4) {
					pom = "0" + pom;
				}
				char arr[] = { pom[2], pom[3], pom[0], pom[1] };
				string code(arr);
				code = code.substr(0, 4);
				*value = code;

				cout << "First operand is regIndPom with immediate pomc in hex " << pom << " and register " << regNum << endl;
			}
		}

		else if(regex_search(pom, regexMap["OPERAND_MEMDIR"])) { //regex is the same!!!!!!!!
			Symbol* sym = table->get(pom);
			if (sym == 0) {
				throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table!");
			}
			else {
				if (sym->getLocGlo() == "local") {
					if(regNum!=7)*value = UtilFunctions::generateCode(sym->getOffset(), 2);
					else *value = UtilFunctions::generateCode(sym->getOffset() - 2, 2); //pcrel
					//RELOCATION SYMBOL
					string address = UtilFunctions::decimalToHexa(locationCounter + 2);
					RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection()));
					relocationTable->put(currentSection, rels);
					 
					cout << "First operand is regIndPom with local  symbol " << sym->getLabel() << " and register " << regNum << endl;
				}
				else {  //global
					if(regNum!=7)*value = "0000";
					else *value = UtilFunctions::generateCode(-2, 2); //pcrel
					//RELOCATION SYMBOL
					string address = UtilFunctions::decimalToHexa(locationCounter + 2);
					RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
					relocationTable->put(currentSection, rels);

					cout << "First operand is regIndPom with global symbol " << sym->getLabel() << " and register " << regNum << endl;
				}
			}
		}
		
		else throw new runtime_error("ERROR: Bad syntax for regIndPom addressing");
	}

	else if (addressing == "pcrel") {
		*src = "11111";
		*flag1 = true;
		string opp = *op1;
		string symName = opp.substr(1, opp.size());
		Symbol* sym = table->get(symName);

		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset() - 2, 2); //pcrel
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, true, UtilFunctions::getSectionNumber(sym->getSection()));
				relocationTable->put(currentSection, rels);

				cout << "First operand is pcrel with local symbol " << sym->getLabel() << " and register " << "pc" << endl;
			}
			else { //global
				*value = UtilFunctions::generateCode(-2, 2); //pcrel
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
				relocationTable->put(currentSection, rels);

				cout << "First operand is pcrel with global symbol " << sym->getLabel() << " and register " << "pc" << endl;
			}
		}
	}

	else {
		throw new runtime_error("ERROR: Can not find specified addressing");
	}
	
}

void Compiler::process_second_operand(string* operation, string* op2, string* dst, bool* flag2, string* value) {
	string addressing = findAddressing(*op2);

	if (addressing == "immediateDec" || addressing == "immediateHex") {
		*dst = "00000";
		*flag2 = true;

		if (addressing == "immediateDec") {
			int v = stoi(*op2);
			*value = UtilFunctions::generateCode(v, 2);

			cout << "Second operand is immidiateDec value is " << v << endl;
		}
		else {
			string opp = *op2;
			opp = opp.substr(3, opp.size());
			while (opp.size() < 4) {
				opp = "0" + opp;
			}
			char arr[] = { opp[2], opp[3], opp[0], opp[1] };
			string code(arr);
			code = code.substr(0, 4);
			*value = code;

			cout << "Second operand is immidiateHex value is " << opp << endl;

		}
	}

	else if (addressing == "psw") {
		*dst = "00111";

		cout << "Second operand is immidiateDec value is psw" << endl;
	}

	else if (addressing == "regDir" || addressing == "regDirSpec") {
		if (addressing == "regDir") {
			string opp = *op2;
			int regNum =opp.at(1) - '0'; //register number
			string reg = UtilFunctions::decimalToBinary(regNum);
			*dst = "01" + reg;

			cout << "Second operand is regDir register is " << regNum << endl;

		}
		else {
			string opp = *op2;
			if (opp == "sp") *dst = "01110"; //r6
			else if (opp == "pc") *dst = "01111"; //r7

			cout << "Second operand is regDirSpec register is " << opp << endl;

		}
	}

	else if (addressing == "immAddr" || addressing == "immAddrHex") {
		*dst = "10000";
		*flag2 = true;

		if (addressing == "immAddr") {
			string opp = *op2;
			string num = opp.substr(1, opp.size());
			int v = stoi(num);
			*value = UtilFunctions::generateCode(v, 2);

			cout << "Second operand is immAddr value is " << v << endl;

		}
		else {
			string opp = *op2;
			opp = opp.substr(3, opp.size());
			while (opp.size() < 4) {
				opp = "0" + opp;
			}
			char arr[] = { opp[2], opp[3], opp[0], opp[1] };
			string code(arr);
			code = code.substr(0, 4);
			*value = code;

			cout << "Second operand is immAddrHex value is " << opp << endl;

		}
	}

	else if (addressing == "memDir") {
		*dst = "10000";
		*flag2 = true;
		string symName = *op2;
		Symbol * sym = table->get(symName);
		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset(), 2);
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection())); //section entry number
				relocationTable->put(currentSection, rels);

				cout << "Second operand is memDir with local symbol " << sym->getLabel() << endl;
			}
			else {  //global
				*value = "0000";
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber()); //entry number
				relocationTable->put(currentSection, rels);

				cout << "Second operand is memDir with global symbol " << sym->getLabel() << endl;
			}
		}

	}

	else if (addressing == "symVal") {
		*dst = "00000";
		*flag2 = true;
		string opp = *op2;
		string symName = opp.substr(1, opp.size());
		Symbol* sym = table->get(symName);

		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset(), 2);
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection()));
				relocationTable->put(currentSection, rels);

				cout << "Second operand is symVal with local symbol " << sym->getLabel() << endl;
			}
			else {  //global
				*value = "0000";
				//RELOCATION SYMBOL
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
				relocationTable->put(currentSection, rels);

				cout << "Second operand is symVal with global symbol " << sym->getLabel() << endl;
			}

		}
	}

	else if (addressing == "regIndPom") {
		*flag2 = true;
		string opp = *op2;

		string pom = "";
		int regNum;
		bool found = false;
		for (int i = 0; i < opp.size(); i++) {
			if (opp[i] == '[') {
				found = true;
				regNum =opp.at(i - 1) - '0';
			}
			if (opp[i] == ']')found = false;
			if (found == true && opp[i]!='[') {
				pom += opp[i];
			}
		}

		string reg = UtilFunctions::decimalToBinary(regNum);
		*dst = "11" + reg;

		if (regex_search(pom, regexMap["OPERAND_DEC"]) || regex_search(pom, regexMap["OPERAND_HEX"])) {
			if (regex_search(pom, regexMap["OPERAND_DEC"])) {
				int v = stoi(pom);
				*value = UtilFunctions::generateCode(v, 2);

				cout << "Second operand is regIndPom with immediate pomc in dec " << v << " and register " << regNum << endl;
			}
			else {
				pom = pom.substr(3, pom.size());
				while (pom.size() < 4) {
					pom = "0" + pom;
				}
				char arr[] = { pom[2], pom[3], pom[0], pom[1] };
				string code(arr);
				code = code.substr(0, 4);
				*value = code;

				cout << "Second operand is regIndPom with immediate pomc in hex " << pom << " and register " << regNum << endl;
			}
		}

		else if (regex_search(pom, regexMap["OPERAND_MEMDIR"])) { //regex is the same!!!!!!!!
			Symbol* sym = table->get(pom);
			if (sym == 0) {
				throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table!");
			}
			else {
				if (sym->getLocGlo() == "local") {
					if (regNum != 7)*value = UtilFunctions::generateCode(sym->getOffset(), 2);
					else *value = UtilFunctions::generateCode(sym->getOffset() - 2, 2); //pcrel
					//RELOCATION SYMBOL
					string address = UtilFunctions::decimalToHexa(locationCounter + 2);
					RelocationSymbol* rels = new RelocationSymbol(address, false, UtilFunctions::getSectionNumber(sym->getSection()));
					relocationTable->put(currentSection, rels);

					cout << "Second operand is regIndPom with local symbol " << sym->getLabel() << " and register " << regNum << endl;
				}
				else {  //global
					if (regNum != 7)*value = "0000";
					else *value = UtilFunctions::generateCode(-2, 2); //pcrel
					//RELOCATION SYMBOL
					string address = UtilFunctions::decimalToHexa(locationCounter + 2);
					RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
					relocationTable->put(currentSection, rels);

					cout << "Second operand is regIndPom with global symbol " << sym->getLabel() << " and register " << regNum << endl;
				}
			}
		}
		
		else throw new runtime_error("ERROR: Bad syntax for regIndPom addressing");
	}

	else if (addressing == "pcrel") {
		*dst = "11111";
		*flag2 = true;
		string opp = *op2;
		string symName = opp.substr(1, opp.size());
		Symbol* sym = table->get(symName);

		if (sym == 0) {
			throw new runtime_error("ERROR: Unexpected error, there is no symbol in the table");
		}
		else {
			if (sym->getLocGlo() == "local") {
				*value = UtilFunctions::generateCode(sym->getOffset() - 2, 2); //pcrel
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, true, UtilFunctions::getSectionNumber(sym->getSection()));
				relocationTable->put(currentSection, rels);

				cout << "Second operand is pcrel with local symbol " << sym->getLabel() << " and register " << "pc" << endl;
			}
			else {
				*value = UtilFunctions::generateCode(-2, 2); //pcrel
				string address = UtilFunctions::decimalToHexa(locationCounter + 2);
				RelocationSymbol* rels = new RelocationSymbol(address, false, sym->getNumber());
				relocationTable->put(currentSection, rels);

				cout << "Second operand is pcrel with global symbol " << sym->getLabel() << " and register " << "pc" << endl;
			}
		}
	}


	else {
		throw new runtime_error("ERROR: Can not find specified addressing");
	}

}

string Compiler::findAddressing(string op) {
	if (regex_search(op, regexMap["OPERAND_DEC"])) {
		return "immediateDec";
	}
	else if (regex_search(op, regexMap["OPERAND_HEX"])) {
		return "immediateHex";
	}

	else if (regex_search(op, regexMap["OPERAND_PSW"])) {
		return "psw";
	}

	else if (regex_search(op, regexMap["OPERAND_REG"])) {
		return "regDir";
	}
	else if (regex_search(op, regexMap["OPERAND_REGSPEC"])) {
		return "regDirSpec";
	}

	else if (regex_search(op, regexMap["OPERAND_SYMBOL"])) {
		return "symVal";
	}

	else if (regex_search(op, regexMap["OPERAND_MEMDIR"])) {
		return "memDir";
	}

	else if (regex_search(op, regexMap["OPERAND_IMMADDR"])) {
		return "immAddr";
	}
	else if (regex_search(op, regexMap["OPERAND_IMMADDRHEX"])) {
		return "immAddrHex";
	}

	else if (regex_search(op, regexMap["OPERAND_REGINDPOM"])) {
		return "regIndPom";
	}

	else if (regex_search(op, regexMap["OPERAND_PCREL"])) {
		return "pcrel";
	}

	else return "not found";

}

void Compiler::writeToFile(ofstream &outFile) {
	table->print(outFile);
	outFile << endl;

	outFile << "Section name" << "\t" << "Start" << "\t\t" << "Length" << endl;
	for (int i = 0; i < sections.size(); i++) {
		Section s = sections.at(i);
		outFile << s.getName() << "\t\t" << s.getStart() << "\t\t" << s.getLength() << endl;
	}

	relocationTable->print(outFile);
	outFile << endl;

	outFile << "#.data" << endl;
	string genc = generatedCode[".data"];
	outFile << genc << endl;

	outFile << "#.text" << endl;
	genc = generatedCode[".text"];
	outFile << genc << endl;

	outFile << "#.rodata" << endl;
	genc = generatedCode[".rodata"];
	outFile << genc << endl;




}
