#ifndef MEMORY_H
#define MEMORY_H

#include <map>
#include <string>
#include <fstream>

using namespace std;

typedef unsigned char byte;


class Memory {
private:
	map<int, string> ram;
	map<int, string> io;
public:
	Memory() {};
	~Memory() {};

	void writeRamByte(int address, string data);
	void writeIoByte(int address, string data);
	string readRamByte(int address);
	string readIoByte(int address);

	void print(ofstream& out);
};


#endif // !MEMORY_H

