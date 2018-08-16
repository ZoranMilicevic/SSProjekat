#ifndef SECTION_H
#define SECTION_H

#include <string>

using namespace std;

class Section {
private:
	string name;
	int length;
	int start;


public:
	Section() {}

	Section(string name, int start, int length) {
		this->name = name;
		this->start = start;
		this->length = length;
	}
	
	~Section() {}

	string getName() {
		return this->name;
	}

	void setName(string name) {
		this->name = name;
	}

	int getLength() {
		return this->length;
	}

	void setLength(int length) {
		this->length = length;
	}

	int getStart() {
		return this->start;
	}

	void setStart(int start) {
		this->start = start;
	}

};

#endif
