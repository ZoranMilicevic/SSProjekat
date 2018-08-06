#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

using namespace std;

class Symbol {
private:
	string label;
	string section;
	int offset;
	string locGlo;
	int number;

public:

	Symbol() {}

	Symbol(string label, string section, int offset, string locGlo, int number) {
		this->label = label;
		this->section = section;
		this->offset = offset;
		this->locGlo = locGlo;
		this->number = number;
	}

	~Symbol() {}

	string getLabel() {
		return this->label;
	}

	void setLabel(string label) {
		this->label = label;
	}

	string getSection(){
		return this->section;
	}

	void setSection(string section) {
		this->section = section;
	}

	int getOffset() {
		return this->offset;
	}

	void setOffset(int offset) {
		this->offset = offset;
	}

	string getLocGlo() {
		return this->locGlo;
	}

	void setLocGlo(string locGlo) {
		this->locGlo = locGlo;
	}

	int getNumber() {
		return this->number;
	}

	void setNumber(int number) {
		this->number = number;
	}

};

#endif
