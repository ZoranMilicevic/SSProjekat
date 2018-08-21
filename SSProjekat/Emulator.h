#ifndef EMULATOR_H
#define EMULATOR_H

#include <vector>
#include <fstream>

using namespace std;


class Emulator {
private:
	vector<ifstream> files;


	void readObjFile(ifstream& inFile);

public:
	Emulator(vector<ifstream> files);
	~Emulator() {};

	void load();


};

#endif 