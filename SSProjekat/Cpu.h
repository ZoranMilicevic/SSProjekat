#ifndef CPU_H
#define CPU_H

using namespace std;


class Cpu {
public:
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
	bool negativeFlaf() {
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
	void setOverflowFlaf(bool b) {
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