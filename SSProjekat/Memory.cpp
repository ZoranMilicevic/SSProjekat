#include "Memory.h"
#include <iostream>
using namespace std;

void Memory::writeRamByte(int address, string data) {
	ram[address] = data;
}

void Memory::writeIoByte(int address, string data) {
	io[address] = data;
}

string Memory::readRamByte(int address) {
	return ram[address];
}

string Memory::readIoByte(int address) {
	return io[address];
}

void Memory::print(ofstream& out) {
	for (map<int, string>::iterator it = ram.begin(); it != ram.end(); it++) {
		int add = it->first;
		string dat = it->second;
		out << add << "-" << dat << endl;
	}

	int p = 1;
	for (map<int, string>::iterator it = ram.begin(); it != ram.end(); it++) {
		int add = it->first;
		string dat = it->second;
		out << dat;
		p = (p + 1) % 2;
		if (p == 1) out << " ";
	}
}

