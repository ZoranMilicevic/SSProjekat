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
		else if(word!="") {
			ret.push_back(word);
			word = "";
		}
	}

	if (word != "") {
		ret.push_back(word);
	}


	return ret;
}

int UtilFunctions::getDirectiveSize(string directive) {
	if (directive == ".char") return 1;
	else if (directive == ".word") return 2;
	else if (directive == ".long") return 4;
	return 0;
}

string UtilFunctions::decimalToBinary(int number) {
	string bin = "";
	while (number != 0) {
		char c = number % 2 ? '0' : '1';
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
		unsigned int neo = number;
		bitset<16> consta(neo);
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
	return "hex";
}

int UtilFunctions::getSectionNumber(string section) {
	if (section == ".text") return 1;
	else if (section == ".data") return 2;
	else if (section == ".rodata") return 3;
	else if (section == ".bss") return 4;
	else return -1;
}


