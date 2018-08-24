#include "Cpu.h"
#include <string>
#include "UtilFunctions.h"
#include "Instruction.h"

using namespace std;

bool Cpu::decodeAndExec() {
	cout << regs[PC] << endl;
	string opcode = mem->readRamByte(regs[PC]) + mem->readRamByte(regs[PC] + 1);
	regs[PC] += 2;
	string binary = UtilFunctions::hexToBinary(opcode);

	string op = binary.substr(0, 6);
	string cond = binary.substr(0, 2);

	string dst = binary.substr(6, 5);
	string addressing1 = dst.substr(0, 2);
	string reg1 = dst.substr(2, 3);

	string src = binary.substr(11, 5);
	string addressing2 = src.substr(0, 2);
	string reg2 = src.substr(2, 3);

	if (cond == "00") {
		if (zeroFlag() == false)return true;
	}
	if (cond == "01") {
		if (zeroFlag() == true) return true;
	}
	if (cond == "10") {
		if (!zeroFlag() && (overflowFlag() == negativeFlag())) {}
		else return true;
	}

	if (op == Instruction::instructions["eqadd"]->getOpcode() || op == Instruction::instructions["neadd"]->getOpcode() || op == Instruction::instructions["gtadd"]->getOpcode() || op == Instruction::instructions["aladd"]->getOpcode()
		|| op == Instruction::instructions["eqsub"]->getOpcode() || op == Instruction::instructions["nesub"]->getOpcode() || op == Instruction::instructions["gtsub"]->getOpcode() || op == Instruction::instructions["alsub"]->getOpcode()
		|| op == Instruction::instructions["eqmul"]->getOpcode() || op == Instruction::instructions["nemul"]->getOpcode() || op == Instruction::instructions["gtmul"]->getOpcode() || op == Instruction::instructions["almul"]->getOpcode()
		|| op == Instruction::instructions["eqdiv"]->getOpcode() || op == Instruction::instructions["nediv"]->getOpcode() || op == Instruction::instructions["gtdiv"]->getOpcode() || op == Instruction::instructions["aldiv"]->getOpcode()
		|| op == Instruction::instructions["eqand"]->getOpcode() || op == Instruction::instructions["neand"]->getOpcode() || op == Instruction::instructions["gtand"]->getOpcode() || op == Instruction::instructions["aland"]->getOpcode()
		|| op == Instruction::instructions["eqor"]->getOpcode() || op == Instruction::instructions["neor"]->getOpcode() || op == Instruction::instructions["gtor"]->getOpcode() || op == Instruction::instructions["alor"]->getOpcode()
		|| op == Instruction::instructions["eqnot"]->getOpcode() || op == Instruction::instructions["nenot"]->getOpcode() || op == Instruction::instructions["gtnot"]->getOpcode() || op == Instruction::instructions["alnot"]->getOpcode()
		|| op == Instruction::instructions["eqshl"]->getOpcode() || op == Instruction::instructions["neshl"]->getOpcode() || op == Instruction::instructions["gtshl"]->getOpcode() || op == Instruction::instructions["alshl"]->getOpcode()
		|| op == Instruction::instructions["eqshr"]->getOpcode() || op == Instruction::instructions["neshr"]->getOpcode() || op == Instruction::instructions["gtshr"]->getOpcode() || op == Instruction::instructions["alshr"]->getOpcode()
		|| op == Instruction::instructions["eqmov"]->getOpcode() || op == Instruction::instructions["nemov"]->getOpcode() || op == Instruction::instructions["gtmov"]->getOpcode() || op == Instruction::instructions["almov"]->getOpcode()) {
		
		//DST PROCESSING
		int regNum1 = -1;
		int data1 = -1;
		int opp1 = 0;
		string dstType = "";
		if (addressing1 == "00") {
			return false;
		}
		else if (addressing1 == "01") {
			regNum1 = UtilFunctions::binaryToDec(reg1);
			opp1 = regs[regNum1];
			dstType = "regDir";
		}
		else if (addressing1 == "10") {
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			data1 = UtilFunctions::hexToDecimal(val); //address

			val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
			opp1 = UtilFunctions::hexToDecimal(val);
			dstType = "mem";
		}
		else if (addressing1 == "11") {
			regNum1 = UtilFunctions::binaryToDec(reg1);
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			data1 = UtilFunctions::hexToDecimal(val); //pom
			data1 = data1 + regs[regNum1]; //address

			val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
			opp1 = UtilFunctions::hexToDecimal(val);
			dstType = "mem";
		}

		//SRC PROCESSING
		int regNum2 = -1;
		int data2 = -1;
		int opp2 = 0;
		if (addressing2 == "00") {
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			if (regNum1 == 7) opp1 += 2;
			data2 = UtilFunctions::hexToDecimal(val);
			opp2 = data2;
		}
		else if (addressing2 == "01") {
			regNum2 = UtilFunctions::binaryToDec(reg2);
			opp2 = regs[regNum2];
		}
		else if (addressing2 == "10") {
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			if (regNum1 == 7) opp1 += 2;
			data2 = UtilFunctions::hexToDecimal(val);

			val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
			opp2 = UtilFunctions::hexToDecimal(val);
		}
		else if (addressing2 == "11") {
			regNum2 = UtilFunctions::binaryToDec(reg2);
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			data2 = UtilFunctions::hexToDecimal(val);
			data2 = data2 + regs[regNum2];

			val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
			opp2 = UtilFunctions::hexToDecimal(val);
		}

		int res = 0;
		int16_t resS = 0;
		uint16_t resU = 0;
		//AND
		if (op == Instruction::instructions["eqadd"]->getOpcode() || op == Instruction::instructions["neadd"]->getOpcode() || op == Instruction::instructions["gtadd"]->getOpcode() || op == Instruction::instructions["aladd"]->getOpcode()) {
			res = opp1 + opp2;
			resS = opp1 + opp2;
			resU = opp1 + opp2;
			setZeroFlag(resS == 0);
			setZeroFlag(resS < 0);
			setOverflowFlag(res != resS);
			setCarryFlag(res != resS);
			res = resS;
		}
		//SUB
		else if (op == Instruction::instructions["eqsub"]->getOpcode() || op == Instruction::instructions["nesub"]->getOpcode() || op == Instruction::instructions["gtsub"]->getOpcode() || op == Instruction::instructions["alsub"]->getOpcode()) {
			res = opp1 - opp2;
			resS = opp1 - opp2;
			resU = opp1 - opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
			setOverflowFlag(res != resS);
			setCarryFlag(res != resS);
			res = resS;
		}
		//MUL
		else if (op == Instruction::instructions["eqmul"]->getOpcode() || op == Instruction::instructions["nemul"]->getOpcode() || op == Instruction::instructions["gtmul"]->getOpcode() || op == Instruction::instructions["almul"]->getOpcode()) {
			resS = opp1 * opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}
		//DIV
		else if (op == Instruction::instructions["eqdiv"]->getOpcode() || op == Instruction::instructions["nediv"]->getOpcode() || op == Instruction::instructions["gtdiv"]->getOpcode() || op == Instruction::instructions["aldiv"]->getOpcode()) {
			resS = opp1 / opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}
		//AND
		else if (op == Instruction::instructions["eqand"]->getOpcode() || op == Instruction::instructions["neand"]->getOpcode() || op == Instruction::instructions["gtand"]->getOpcode() || op == Instruction::instructions["aland"]->getOpcode()) {
			resS = opp1 & opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}
		//OR
		else if (op == Instruction::instructions["eqor"]->getOpcode() || op == Instruction::instructions["neor"]->getOpcode() || op == Instruction::instructions["gtor"]->getOpcode() || op == Instruction::instructions["alor"]->getOpcode()) {
			resS = opp1 | opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}
		//NOT
		else if (op == Instruction::instructions["eqnot"]->getOpcode() || op == Instruction::instructions["nenot"]->getOpcode() || op == Instruction::instructions["gtnot"]->getOpcode() || op == Instruction::instructions["alnot"]->getOpcode()) {
			resS =~ opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}
		//SHL
		else if (op == Instruction::instructions["eqshl"]->getOpcode() || op == Instruction::instructions["neshl"]->getOpcode() || op == Instruction::instructions["gtshl"]->getOpcode() || op == Instruction::instructions["alshl"]->getOpcode()) {
			resS = opp1 << opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
			setCarryFlag(regs[regNum1] & (1 << (16 - opp2)));
		}
		//SHR
		else if (op == Instruction::instructions["eqshr"]->getOpcode() || op == Instruction::instructions["neshr"]->getOpcode() || op == Instruction::instructions["gtshr"]->getOpcode() || op == Instruction::instructions["alshr"]->getOpcode()) {
			resS = opp1 >> opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
			
		}
		//MOV
		else if (op == Instruction::instructions["eqmov"]->getOpcode() || op == Instruction::instructions["nemov"]->getOpcode() || op == Instruction::instructions["gtmov"]->getOpcode() || op == Instruction::instructions["almov"]->getOpcode()) {
			resS = opp2;
			setZeroFlag(resS == 0);
			setNegativeFlag(resS < 0);
		}

		if (dstType == "regDir") {
			regs[regNum1] = resS;
		}
		else if (dstType == "mem") {
			string code = UtilFunctions::generateCode(resS, 2);
			mem->writeRamByte(data1, code.substr(0, 2));
			mem->writeRamByte(data1 + 1, code.substr(2, 2));
		}
		return true;
	}
	//TEST CMP
	else if (op == Instruction::instructions["eqcmp"]->getOpcode() || op == Instruction::instructions["necmp"]->getOpcode() || op == Instruction::instructions["gtcmp"]->getOpcode() || op == Instruction::instructions["alcmp"]->getOpcode()
		|| op == Instruction::instructions["eqtest"]->getOpcode() || op == Instruction::instructions["netest"]->getOpcode() || op == Instruction::instructions["gttest"]->getOpcode() || op == Instruction::instructions["altest"]->getOpcode()) {
		
			//DST PROCESSING
			int regNum1 = -1;
			int data1 = -1;
			int opp1 = 0;
			if (addressing1 == "00") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data1 = UtilFunctions::hexToDecimal(val);
				opp1 = data1;
			}
			else if (addressing1 == "01") {
				regNum1 = UtilFunctions::binaryToDec(reg1);
				opp1 = regs[regNum1];
			}
			else if (addressing1 == "10") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data1 = UtilFunctions::hexToDecimal(val); //address

				val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
				opp1 = UtilFunctions::hexToDecimal(val);
			}
			else if (addressing1 == "11") {
				regNum1 = UtilFunctions::binaryToDec(reg1);
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data1 = UtilFunctions::hexToDecimal(val); //pom
				data1 = data1 + regs[regNum1]; //address

				val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
				opp1 = UtilFunctions::hexToDecimal(val);
			}

			//SRC PROCESSING
			int regNum2 = -1;
			int data2 = -1;
			int opp2 = 0;
			if (addressing2 == "00") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);
				opp2 = data2;
			}
			else if (addressing2 == "01") {
				regNum2 = UtilFunctions::binaryToDec(reg2);
				opp2 = regs[regNum2];
			}
			else if (addressing2 == "10") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);

				val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
				opp2 = UtilFunctions::hexToDecimal(val);
			}
			else if (addressing2 == "11") {
				regNum2 = UtilFunctions::binaryToDec(reg2);
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);
				data2 = data2 + regs[regNum2];

				val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
				opp2 = UtilFunctions::hexToDecimal(val);
			}

			int16_t res = 0;
			if(op == Instruction::instructions["eqcmp"]->getOpcode() || op == Instruction::instructions["necmp"]->getOpcode() || op == Instruction::instructions["gtcmp"]->getOpcode() || op == Instruction::instructions["alcmp"]->getOpcode()){
				res = opp1 - opp2;
				setZeroFlag(res == 0);
				setNegativeFlag(res < 0);
			}
			else if (op == Instruction::instructions["eqtest"]->getOpcode() || op == Instruction::instructions["netest"]->getOpcode() || op == Instruction::instructions["gttest"]->getOpcode() || op == Instruction::instructions["altest"]->getOpcode()) {
				res = opp1 & opp2;
				setZeroFlag(res == 0);
				setNegativeFlag(res < 0);
			}
			return true;
		}

	//PUSH CALL
	else if (op == Instruction::instructions["eqpush"]->getOpcode() || op == Instruction::instructions["nepush"]->getOpcode() || op == Instruction::instructions["gtpush"]->getOpcode() || op == Instruction::instructions["alpush"]->getOpcode() 
		|| op == Instruction::instructions["eqcall"]->getOpcode() || op == Instruction::instructions["necall"]->getOpcode() || op == Instruction::instructions["gtcall"]->getOpcode() || op == Instruction::instructions["alcall"]->getOpcode()) {
		
		//SRC PROCESSING
			int regNum2 = -1;
			int data2 = -1;
			int opp2 = 0;
			if (addressing2 == "00") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);
				opp2 = data2;
			}
			else if (addressing2 == "01") {
				regNum2 = UtilFunctions::binaryToDec(reg2);
				opp2 = regs[regNum2];
			}
			else if (addressing2 == "10") {
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);

				val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
				opp2 = UtilFunctions::hexToDecimal(val);
			}
			else if (addressing2 == "11") {
				regNum2 = UtilFunctions::binaryToDec(reg2);
				string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
				regs[PC] += 2;
				data2 = UtilFunctions::hexToDecimal(val);
				data2 = data2 + regs[regNum2];

				val = mem->readRamByte(data2 + 1) + mem->readRamByte(data2); //little endian!
				opp2 = UtilFunctions::hexToDecimal(val);
			}

			if (op == Instruction::instructions["eqpush"]->getOpcode() || op == Instruction::instructions["nepush"]->getOpcode() || op == Instruction::instructions["gtpush"]->getOpcode() || op == Instruction::instructions["alpush"]->getOpcode()) {
				regs[SP] -= 1;
				stack[regs[SP]] = opp2;
			}
			else if (op == Instruction::instructions["eqcall"]->getOpcode() || op == Instruction::instructions["necall"]->getOpcode() || op == Instruction::instructions["gtcall"]->getOpcode() || op == Instruction::instructions["alcall"]->getOpcode()) {
				regs[SP] -= 1;
				stack[regs[SP]] = regs[PC];
				regs[PC] = opp2;
			}
			return true;
	}
	//POP
	else if (op == Instruction::instructions["eqpop"]->getOpcode() || op == Instruction::instructions["nepop"]->getOpcode() || op == Instruction::instructions["gtpop"]->getOpcode() || op == Instruction::instructions["alpop"]->getOpcode()) {

		//DST PROCESSING
		int regNum1 = -1;
		int data1 = -1;
		int opp1 = 0;
		string dstType = "";
		if (addressing1 == "00") {
			return false;
		}
		else if (addressing1 == "01") {
			regNum1 = UtilFunctions::binaryToDec(reg1);
			opp1 = regs[regNum1];
			dstType = "regDir";
		}
		else if (addressing1 == "10") {
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			data1 = UtilFunctions::hexToDecimal(val); //address

			val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
			opp1 = UtilFunctions::hexToDecimal(val);
			dstType = "mem";
		}
		else if (addressing1 == "11") {
			regNum1 = UtilFunctions::binaryToDec(reg1);
			string val = mem->readRamByte(regs[PC]+1) + mem->readRamByte(regs[PC]); //little endian!
			regs[PC] += 2;
			data1 = UtilFunctions::hexToDecimal(val); //pom
			data1 = data1 + regs[regNum1]; //address

			val = mem->readRamByte(data1 + 1) + mem->readRamByte(data1); //little endian!
			opp1 = UtilFunctions::hexToDecimal(val);
			dstType = "mem";
		}

		int w = stack[regs[SP]];
		regs[SP]++;
		if (dstType == "regDir") {
			regs[regNum1] = w;
		}
		else {
			string code = UtilFunctions::generateCode(w, 2);
			mem->writeRamByte(data1, code.substr(0, 2));
			mem->writeRamByte(data1 + 1, code.substr(2, 2));
		}

		return true;
	}

	//IRET
	else if (op == Instruction::instructions["eqiret"]->getOpcode() || op == Instruction::instructions["neiret"]->getOpcode() || op == Instruction::instructions["gtiret"]->getOpcode() || op == Instruction::instructions["aliret"]->getOpcode()) {
		regs[PC] = stack[regs[SP]];
		regs[SP]++;
		regs[PSW] = stack[regs[SP]];
		regs[SP]++;
		return true;
	}
}

