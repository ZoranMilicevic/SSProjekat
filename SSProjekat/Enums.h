#ifndef ENUMS_H
#define ENUMS_H

enum InstructionTypes {ARTIHMETIC, LOGICAL, MOVESHIFT, LOADSTORE, CALL};

class Enums {
public:
	enum InstructionSymbol {
		ADD = 0,
		SUB = 1,
		MUL = 2,
		DIV = 3,
		CMP = 4,
		AND = 5,
		OR = 6,
		NOT = 7,
		TEST = 8,
		PUSH = 9,
		POP = 10,
		CALL = 11,
		IRET = 12,
		MOV = 13,
		SHL = 14,
		SHR = 15
	};

	enum InstructionCondition {
		EQ = 0,
		NE = 1,
		GT = 2,
		AL = 3
	};
};


#endif
