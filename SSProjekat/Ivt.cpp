#include "Ivt.h"

using namespace std;


void Ivt::init() {
	

}

int Ivt::getInterruptRoutine(int ivt_entry) {
	return memory->readRamInt(4 * ivt_entry);
}