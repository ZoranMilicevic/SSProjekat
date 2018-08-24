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

	Emulator* e = new Emulator();

	e->load(argc, argv);
	e->run();

	int n;
	cin >> n;
}


