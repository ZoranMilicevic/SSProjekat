#include "Memory.h"

using namespace std;

void Memory::writeRamByte(int address, byte data) {
	ram[address] = data;
}

void Memory::writeIoByte(int address, byte data) {
	io[address] = data;
}

byte Memory::readRamByte(int address) {
	return ram[address];
}

byte Memory::readIoByte(int address) {
	return io[address];
}

