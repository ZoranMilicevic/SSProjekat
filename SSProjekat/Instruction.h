#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>
#include <map>
using namespace std;

class Instruction {
private:
	string name;
	string opcode;

public:
	Instruction() {}

	Instruction(string name, string opcode) {
		this->name = name;
		this->opcode = opcode;
	}

	~Instruction() {}

	string getName() {
		return this->name;
	}

	void setName(string name) {
		this->name = name;
	}

	string getOpcode() {
		return this->opcode;
	}

	void setOpcode(string opcode) {
		this->opcode = opcode;
	}

public:
	static map<string, Instruction*> instructions;
};

#endif
