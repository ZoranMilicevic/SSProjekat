#include "UtilFunctions.h"

using namespace std;

vector<string> UtilFunctions::split(string line) {
	vector<string> ret;
	string word="";

	/*
	while (getline(ss, word, ' ')) {
		ret.push_back(word);
	}
	*/

	for (int i = 0; i < line.size(); i++) {
		if (line.at(i) != ' ' && line.at(i) != ',')word += line.at(i);
		else if(word!="" && word!="\n") {
			ret.push_back(word);
			word = "";
		}
	}

	if (word != "" && word != "\n") {
		ret.push_back(word);
	}


	return ret;
}

int UtilFunctions::getDirectiveSize(string directive) {
	if (directive == ".char") return 1;
	else if (directive == ".word") return 2;
	else if (directive == ".long") return 4;
	else throw new runtime_error("ERROR: Directive with that name does not exist!");
	return 0;
}

int UtilFunctions::getSectionNumber(string section) {
	if (section == ".text") return 1;
	else if (section == ".data") return 2;
	else if (section == ".rodata") return 3;
	else if (section == ".bss") return 4;
	else throw new runtime_error("ERROR: Section with that name does not exist");
	return 0;
}

string UtilFunctions::decimalToBinary(int number) {
	string bin = "";
	while (number != 0) {
		char c = number % 2 ? '1' : '0';
		bin = c + bin;
		number = number / 2;
	}

	while (bin.length() < 3) {
		bin = "0" + bin;
	}
	return bin;
}

string UtilFunctions::decimalToHexa(int number) {
	string hexa = "";
	if (number == 0) {
		hexa = "0";
		return hexa;
	}
	else if (number < 0) {
		short n = number;
		bitset<16> consta(n);
		return binaryToHexa(consta.to_string());
	}

	while (number > 0) {
		int remainder = number % 16;
		switch (remainder) {
			case 0: hexa = "0" + hexa; break;
			case 1: hexa = "1" + hexa; break;
			case 2: hexa = "2" + hexa; break;
			case 3: hexa = "3" + hexa; break;
			case 4: hexa = "4" + hexa; break;
			case 5: hexa = "5" + hexa; break;
			case 6: hexa = "6" + hexa; break;
			case 7: hexa = "7" + hexa; break;
			case 8: hexa = "8" + hexa; break;
			case 9: hexa = "9" + hexa; break;
			case 10: hexa = "A" + hexa; break;
			case 11: hexa = "B" + hexa; break;
			case 12: hexa = "C" + hexa; break;
			case 13: hexa = "D" + hexa; break;
			case 14: hexa = "E" + hexa; break;
			case 15: hexa = "F" + hexa; break;
		}
		number = number / 16;
	}
	return hexa;
}

string UtilFunctions::binaryToHexa(string binary) {
	string hex = "", temp = "";
	int k = binary.length();
	for (int i = 0; i < k; i += 4) {
		temp = binary.substr(i, 4);
		if (temp == "0000")hex = hex + "0";
		else if (temp == "0001")hex = hex + "1";
		else if (temp == "0010")hex = hex + "2";
		else if (temp == "0011")hex = hex + "3";
		else if (temp == "0100")hex = hex + "4";
		else if (temp == "0101")hex = hex + "5";
		else if (temp == "0110")hex = hex + "6";
		else if (temp == "0111")hex = hex + "7";
		else if (temp == "1000")hex = hex + "8";
		else if (temp == "1001")hex = hex + "9";
		else if (temp == "1010")hex = hex + "A";
		else if (temp == "1011")hex = hex + "B";
		else if (temp == "1100")hex = hex + "C";
		else if (temp == "1101")hex = hex + "D";
		else if (temp == "1110")hex = hex + "E";
		else if (temp == "1111")hex = hex + "F";
		else continue;
	}
	return hex;
}

string UtilFunctions::generateCode(int num, int size) {
	string hex = decimalToHexa(num);

	if (size == 4) {
		if (hex.size() > 8) {
			throw new runtime_error("ERROR: too big");
		}
		while (hex.size() < 8) {
			hex = "0" + hex;
		}
		char arr[8];
		arr[0] = hex.at(6);
		arr[1] = hex.at(7);
		arr[2] = hex.at(4);
		arr[3] = hex.at(5);
		arr[4] = hex.at(2);
		arr[5] = hex.at(3);
		arr[6] = hex.at(0);
		arr[7] = hex.at(1);
		string ret(arr);
		return ret.substr(0, 8);
	}

	else if (size == 2) {
		if (hex.size() > 4) {
			int k;
			int l;
			throw new runtime_error("ERROR: too big");
		}
		while (hex.size() < 4) {
			hex = "0" + hex;
		}
		char arr[4];
		arr[0] = hex.at(2);
		arr[1] = hex.at(3);
		arr[2] = hex.at(0);
		arr[3] = hex.at(1);
		string ret(arr);
		return ret.substr(0, 4);
	}

	else if (size == 1) {
		if (hex.size() > 2) {
			throw new runtime_error("ERROR: too big");
		}
		while (hex.size() < 2) {
			hex = "0" + hex;
		}
		char arr[2];
		arr[0] = hex.at(0);
		arr[1] = hex.at(1);
		string ret(arr);
		return ret.substr(0, 2);
	}

	else throw runtime_error("ERROR: Unexpected error");	
}

int UtilFunctions::hexToDecimal(string num) {
	int ret = 0;
	int i = num.size() - 1;
	bool flag = false;
	if (num.size()==4 &&(num.at(0) == '8' || num.at(0) == '9' || num.at(0) == 'A' || num.at(0) == 'B' || num.at(0) == 'C' || num.at(0) == 'D' || num.at(0) == 'E' || num.at(0) == 'F')) flag = true;
	for (int k = 0; i >= 0; i--, k++) {
		char c = num.at(i);
		int mul = pow(16, k);
		switch (c) {
		case '0': ret += 0; break;
		case '1': ret += mul; break;
		case '2': ret += 2 * mul; break;
		case '3': ret += 3 * mul; break;
		case '4': ret += 4 * mul; break;
		case '5': ret += 5 * mul; break;
		case '6': ret += 6 * mul; break;
		case '7': ret += 7 * mul; break;
		case '8': ret += 8 * mul; break;
		case '9': ret += 9 * mul; break;
		case 'A': ret += 10 * mul; break;
		case 'B': ret += 11 * mul; break;
		case 'C': ret += 12 * mul; break;
		case 'D': ret += 13 * mul; break;
		case 'E': ret += 14 * mul; break;
		case 'F': ret += 15 * mul; break;
		}
	}
	if (flag) ret -= 65536;
	return ret;
}

string UtilFunctions::hexToBinary(string hex) {
	string ret = "";

	for (int i = 0; i < hex.size(); i++) {
		char c = hex[i];
		switch (c) {
			case '0': ret += "0000"; break;
			case '1': ret += "0001"; break;
			case '2': ret += "0010"; break;
			case '3': ret += "0011"; break;
			case '4': ret += "0100"; break;
			case '5': ret += "0101"; break;
			case '6': ret += "0110"; break;
			case '7': ret += "0111"; break;
			case '8': ret += "1000"; break;
			case '9': ret += "1001"; break;
			case 'A': ret += "1010"; break;
			case 'B': ret += "1011"; break;
			case 'C': ret += "1100"; break;
			case 'D': ret += "1101"; break;
			case 'E': ret += "1110"; break;
			case 'F': ret += "1111"; break;
		}
	}
	return ret;
}

int UtilFunctions::binaryToDec(string bin) {
	int ret = 0;
	int i = bin.size() - 1;
	for (int k = 0; i >= 0; i--, k++) {
		char c = bin.at(i);
		int mul = pow(2, k);
		switch (c) {
			case '0': ret += 0; break;
			case '1': ret += mul; break;
		}
	}
	return ret;

}

