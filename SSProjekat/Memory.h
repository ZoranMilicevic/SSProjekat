#ifndef MEMORY_H
#define MEMORY_H

#include <map>

using namespace std;

typedef unsigned char byte;


class Memory {
private:
	map<int, byte> ram;
	map<int, byte> io;
public:
	Memory() {};
	~Memory() {};

	void writeRamByte(int address, byte data);
	void writeIoByte(int address, byte data);
	byte readRamByte(int address);
	byte readIoByte(int address);

	void writeRamInt(int address, int data, bool little = false);
	void writeIoInt(int address, int data, bool little = false);
	int readRamInt(int address, bool little = false);
	int readIoInt(int address, bool little = false);


};


#endif // !MEMORY_H

