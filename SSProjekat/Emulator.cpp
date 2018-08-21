#include "Emulator.h"

using namespace std;

Emulator::Emulator(vector<ifstream> files) {
	this->files = files;
}

void Emulator::load() {
	for (int i = 0; i < files.size(); i++) {
		readObjFile(files.at(i));
	}
}

void Emulator::readObjFile(ifstream& inFile) {
	if (!inFile.is_open()) {
		throw new runtime_error("ERROR: There was an error while opening input file");
	}



}