#include <iostream>
#include <fstream>
#include <exception>

#include "Compiler.h"

using namespace std;
/*
int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "Please call this program as ./compiler inputFile outputFile" << endl;
		return 1;
	}

	ifstream inFile(argv[1]);
	ofstream outFile(argv[2]);
	int startAddress = 0;
	if (argc >= 3) {
		string s = argv[3];
		startAddress = stoi(s);
	}

	if (!inFile.is_open()) {
		cerr << "There was an error while opening the input file" << endl;
		return 2;
	}

	if (!outFile.is_open()) {
		cerr << "There was an error while opening the output file" << endl;
		return 3;
	}

	Compiler* c = new Compiler();
	c->compile(inFile, outFile, startAddress);

	delete c;
	inFile.close();
	outFile.close();

	cout << "Success" << endl;

	int in;
	cin >> in;

	return 0;

}*/
