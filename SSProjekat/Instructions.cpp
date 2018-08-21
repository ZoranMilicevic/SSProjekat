#include "Instruction.h"

map<string, Instruction* > Instruction::instructions = {
	//EQ
	{"eqadd", new Instruction("eqadd", "000000")},
	{"eqsub", new Instruction("eqsub", "000001")},
	{ "eqmul", new Instruction("eqmul", "000010") },
	{ "eqdiv", new Instruction("eqdiv", "000011") },
	{ "eqcmp", new Instruction("eqcmp", "000100") },
	{ "eqand", new Instruction("eqand", "000101") },
	{ "eqor", new Instruction("eqor", "000110") },
	{ "eqnot", new Instruction("eqnot", "000111") },
	{ "eqtest", new Instruction("eqtest", "001000") },
	{ "eqpush", new Instruction("eqpush", "001001") },
	{ "eqpop", new Instruction("eqpop", "001010") },
	{ "eqcall", new Instruction("eqcall", "001011") },
	{ "eqiret", new Instruction("eqiret", "001100") },
	{ "eqmov", new Instruction("eqmov", "001101") },
	{ "eqshl", new Instruction("eqshl", "001110") },
	{ "eqshr", new Instruction("eqshr", "001111") },

	//NE
	{ "neadd", new Instruction("neadd", "010000") },
	{ "nesub", new Instruction("nesub", "010001") },
	{ "nemul", new Instruction("nemul", "010010") },
	{ "nediv", new Instruction("nediv", "010011") },
	{ "necmp", new Instruction("necmp", "010100") },
	{ "neand", new Instruction("neand", "010101") },
	{ "neor", new Instruction("neor", "010110") },
	{ "nenot", new Instruction("nenot", "010111") },
	{ "netest", new Instruction("netest", "011000") },
	{ "nepush", new Instruction("nepush", "011001") },
	{ "nepop", new Instruction("nepop", "011010") },
	{ "necall", new Instruction("necall", "011011") },
	{ "neiret", new Instruction("neiret", "001100") },
	{ "nemov", new Instruction("nemov", "011101") },
	{ "neshl", new Instruction("neshl", "011110") },
	{ "neshr", new Instruction("neshr", "011111") },

	//GT
	{ "gtadd", new Instruction("gtadd", "100000") },
	{ "gtsub", new Instruction("gtsub", "100001") },
	{ "gtmul", new Instruction("gtmul", "100010") },
	{ "gtdiv", new Instruction("gtdiv", "100011") },
	{ "gtcmp", new Instruction("gtcmp", "100100") },
	{ "gtand", new Instruction("gtand", "100101") },
	{ "gtor", new Instruction("gtor", "100110") },
	{ "gtnot", new Instruction("gtnot", "100111") },
	{ "gttest", new Instruction("gttest", "101000") },
	{ "gtpush", new Instruction("gtpush", "101001") },
	{ "gtpop", new Instruction("gtpop", "101010") },
	{ "gtcall", new Instruction("gtcall", "101011") },
	{ "gtiret", new Instruction("gtiret", "101100") },
	{ "gtmov", new Instruction("gtmov", "101101") },
	{ "gtshl", new Instruction("gtshl", "101110") },
	{ "gtshr", new Instruction("gtshr", "101111") },

	//AL
	{ "aladd", new Instruction("aladd", "110000") },
	{ "alsub", new Instruction("alsub", "110001") },
	{ "almul", new Instruction("almul", "110010") },
	{ "aldiv", new Instruction("aldiv", "110011") },
	{ "alcmp", new Instruction("alcmp", "110100") },
	{ "aland", new Instruction("aland", "110101") },
	{ "alor", new Instruction("alor", "110110") },
	{ "alnot", new Instruction("alnot", "110111") },
	{ "altest", new Instruction("altest", "111000") },
	{ "alpush", new Instruction("alpush", "111001") },
	{ "alpop", new Instruction("alpop", "111010") },
	{ "alcall", new Instruction("alcall", "111011") },
	{ "aliret", new Instruction("aliret", "111100") },
	{ "almov", new Instruction("almov", "111101") },
	{ "alshl", new Instruction("alshl", "111110") },
	{ "alshr", new Instruction("alshr", "111111") },

	//ret - same as pop
	{ "eqret", new Instruction("eqret", "001010") },
	{ "neret", new Instruction("neret", "011010") },
	{ "gtret", new Instruction("gtret", "101010") },
	{ "alret", new Instruction("alret", "111010") },

	//jmp - add or move depending on addressing
	{ "eqjmpadd", new Instruction("eqjmp", "000000") },
	{ "nejmpadd", new Instruction("nejmp", "010000") },
	{ "gtjmpadd", new Instruction("gtjmp", "100000") },
	{ "aljmpadd", new Instruction("aljmp", "110000") },

	{ "eqjmpmov", new Instruction("eqjmp", "001101") },
	{ "nejmpmov", new Instruction("nejmp", "011101") },
	{ "gtjmpmov", new Instruction("gtjmp", "101101") },
	{ "aljmpmov", new Instruction("aljmp", "111101") },


};