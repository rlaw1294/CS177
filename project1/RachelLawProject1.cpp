#include <iostream>
#include "cpp.h"
#include <string>
using namespace std;

#define TINY 1.e-20		// a very small time period
#define NUMCELLS 120	// 120 cells aka 1/4 mi 
#define TOTALTIME 1440	// 1440 minutes in a day. 24hr * 60min/hr
#define NUMCARS 5

facility_set *road;
class Car {
	public:
		int id;
		int nose;
		int tail;
		int speed;
		int hold_time;
		Car():id(0), nose(0), tail(0), speed(0), hold_time(0) {}
		void setup_car(int i);
};

void Car::setup_car(int i) {
	id = i;
	nose = 2*i;
	tail = 2*i-1;
	speed = 0;
	hold_time = 0;
	(*road)[nose].reserve();
	(*road)[tail].reserve();
}

int num_cells = NUMCELLS;
int num_cars = NUMCARS;
Car cars[NUMCARS];

extern "C" void sim()		// main process
{
	create("sim");

	cars();

	hold (TOTALTIME);
}

void cars() {
	create("cars");
	for (int i=0; i<NUMCARS; i++) {
		cars[i].setup_car(i);
	}

	
}