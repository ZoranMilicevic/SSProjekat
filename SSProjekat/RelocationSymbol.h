#ifndef RELOCATIONSYMBOL_H
#define RELOCATIONSYMBOL_H

#include <string>

using namespace std;

class RelocationSymbol {
private:
	string address;
	bool type; // true - relative addressing
				//false - apsolute
	int number;
public:
	RelocationSymbol() {}

	RelocationSymbol(string address, bool type, int number) {
		this->address = address;
		this->type = type;
		this->number = number;
	}

	~RelocationSymbol() {}

	string getAddress() {
		return this->address;
	}

	void setAddress(string address) {
		this->address = address;
	}

	bool getType() {
		return this->type;
	}

	void setType(bool type) {
		this->type = type;
	}

	int getNumber() {
		return this->number;
	}

	void getNumber(int number) {
		this->number = number;
	}

};

#endif
