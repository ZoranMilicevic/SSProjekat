#ifndef CPU_H
#define CPU_H
#include "Memory.h"
using namespace std;


class Cpu {
private:
	Memory* mem;
	int *stack;
public:
	Cpu(Memory* mem) {
		this->mem = mem;
		stack = new int[1000];
		regs[SP] = 1001;
	};
	~Cpu() {};

	bool decodeAndExec();

	static int interruptRegister;
	static const int timer_interrupt = 1;
	static const int irregular_interrupt = 2;
	static const int keyboard_interrupt = 3;




	int regs[9];
	static const int SP = 6;
	static const int PC = 7;
	static const int PSW = 8;

	static const int MASK_ZERO = 0x1;
	static const int MASK_OVERFLOW = 0x2;
	static const int MASK_CARRY = 0x4;
	static const int MASK_NEGATIVE = 0x8;

	static const int MASK_TIMER = 0x2000; //0010 0000 0000 0000
	static const int MASK_INTERRUPT = 0x8000; //1000 0000 0000 0000

	//CHECK FLAGS
	bool zeroFlag() {
		return regs[PSW] & MASK_ZERO;
	}
	bool overflowFlag() {
		return regs[PSW] & MASK_OVERFLOW;
	}
	bool carryFlag() {
		return regs[PSW] & MASK_CARRY;
	}
	bool negativeFlag() {
		return regs[PSW] & MASK_NEGATIVE;
	}
	bool timerFlag() {
		return regs[PSW] & MASK_TIMER;
	}
	bool interruptFlag() {
		return regs[PSW] & MASK_INTERRUPT;
	}

	//SET FLAGS
	void setZeroFlag(bool b) {
		if (b) regs[PSW] |= MASK_ZERO;
		else regs[PSW] &= ~MASK_ZERO;
	}
	void setOverflowFlag(bool b) {
		if (b) regs[PSW] |= MASK_OVERFLOW;
		else regs[PSW] &= MASK_OVERFLOW;
	}
	void setCarryFlag(bool b) {
		if (b) regs[PSW] |= MASK_CARRY;
		else regs[PSW] &= ~MASK_CARRY;
	}
	void setNegativeFlag(bool b) {
		if (b) regs[PSW] |= MASK_ZERO;
		else regs[PSW] &= ~MASK_ZERO;
	}
	void setTimerFlag(bool b) {
		if (b) regs[PSW] |= MASK_TIMER;
		else regs[PSW] &= ~MASK_TIMER;
	}
	void setInterruptFlag(bool b) {
		if (b) regs[PSW] |= MASK_INTERRUPT;
		else regs[PSW] &= ~MASK_INTERRUPT;
	}



};

#endif // !CPU_H