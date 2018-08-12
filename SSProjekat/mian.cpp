#include <iostream>
#include <fstream>
#include <exception>

#include "Compiler.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc < 3) {
		cout << "Please call this program as ./compiler inputFile outputFile" << endl;
		return 1;
	}

	ifstream inFile(argv[1]);
	ofstream outFile(argv[2]);

	if (!inFile.is_open()) {
		cerr << "There was an error while opening the input file" << endl;
		return 2;
	}

	if (!outFile.is_open()) {
		cerr << "There was an error while opening the output file" << endl;
		return 3;
	}

	Compiler* c = new Compiler();
	c->compile(inFile, outFile);
	delete c;
	
	cout << "Success" << endl;

	int in;
	cin >> in;

	return 0;

}