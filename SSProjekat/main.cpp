#include <iostream>
#include <vector>
#include <fstream>

#include "Emulator.h"

using namespace std;

int main(int argc, char** argv) {
	if (argc < 1){
		cout << "Please call this program as ./emulator inputfile [inputfiles]+" << endl;
		return 1;
	}

	vector<ifstream> files;

	for (int i = 1; i < argc; i++) {
		ifstream file(argv[1]);
		files.push_back(file);
	}

	Emulator* e = new Emulator(files);

	e->load();






}