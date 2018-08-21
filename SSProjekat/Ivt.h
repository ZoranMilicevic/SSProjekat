#ifndef IVT_H
#define IVT_H

#include "Memory.h"

using namespace std;


class Ivt {
private:
	Memory* memory;
public:
	Ivt(Memory *memory) {
		this->memory = memory;
	}
	~Ivt(){}

	void init();
	int getInterruptRoutine(int ivt_entry);

};


#endif // !IVT_H
