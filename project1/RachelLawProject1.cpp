#include <iostream>
#include "cpp.h"
#include <string>
using namespace std;

#define TINY 1.e-20		// a very small time period
#define NUMCELLS 120	// 120 cells aka 1/4 mi 
#define TOTALTIME 1440	// 1440 minutes in a day. 24hr * 60min/hr
#define NUMCARS 5
#define CARLENGTH 2

facility_set *road;

class Car {
	public:
		int id;
		int nose;
		int tail;
		int speed;
		int distanceTraveled;
		bool isSlowing;
		int hold_time;
		Car():id(0), nose(0), tail(0), speed(0), distaneTraveled(0), isSlowing(false), hold_time(0) {}
		void setup_car(int index);
		void print_info(int id);
};

void Car::setup_car(int index) {
	cout << "setting up car " << index << endl;
	index += 1;
	id = index;
	nose = 2*index;
	tail = 2*index-1;
	speed = 0;
	distanceTraveled = 0;
	isSlowing = false;
	hold_time = 0;
	(*road)[nose].reserve();
	(*road)[tail].reserve();
}

void Car::print_info(int id) {
	cout << "id: " << this->id << "\tnose: " << nose << "\ttail: " << tail << "\tspeed: " << speed << "\tDistance_travled: " <<  distanceTraveled << "\tisSLowing: " << isSlowing << "\thold_time: " << hold_time << endl;
}

int num_cells = NUMCELLS;
int num_cars = NUMCARS;
Car car_array[NUMCARS];
void move_cars(int id);
void monitorCarCollision(int id);

extern "C" void sim()		// main process
{
	create("sim");
	road = new facility_set("road", NUMCELLS);
	for (int i=0; i<NUMCARS; i++) {
		car_array[i].setup_car(i);
		move_cars(i);

	}

	hold (TOTALTIME);
	// Print Car Info
	for (int i=0; i<NUMCARS; i++) {
		car_array[i].print_info(i);
	}
}

void monitorCarCollision(int id){
	int lookAhead = 0;
	if(car_array[id].speed == 0){
		lookAhead = 1;
	}
	else if(car_array[id].speed == 1){
		lookAhead = 1;
	}
	else if(car_array[id].speed == 2){
		lookAhead = 4;
	}
	else if(car_array[id].speed == 3){
		lookAhead = 4;
	}
	else if(car_array[id].speed == 4){
		lookAhead = 6;
	}
	else if(car_array[id].speed == 5){
		lookAhead = 8;
	}
}
void move_cars(int id) {
	create("move_cars");
	while (clock < TOTALTIME){

		//detect for car collision
		monitorCarCollision(id);
	
		(*road)[car_array[id].front++ % NUMCELLS].reserve();
		car_array[id].front++;
		car_array[id].distanceTraveled++;
		(*road)[car_array[id].tail % NUMCELLS].release();
		car_array[id].tail++;
		car_array[id].speed++;
		
		if(car_array[id].speed == 0){
			car_array[id].speed = 1;
			car_array[id].isSlowing = false;
		}		
		
		//adjusting speed for every car-length boundary
		if(car_array[id].distanceTraveled % 2 == 0){
			if(car_array[id].speed < 5){
				car_array[id].speed++;
			}
		}
		//calculate the hold time based on speed
		if(car_array[id].speed == 0){
			car_array[id].hold_time = 0;
		}
		else if(car_array[id].speed == 1){
			car_array[id].hold_time = int (3.0/CARLENGTH);
		}
		else if(car_array[id].speed == 2){
			car_array[id].hold_time = int ((11.0/6.0)/CARLENGTH);
		}
		else if(car_array[id].speed == 3){
			car_array[id].hold_time = int (1.0/CARLENGTH);
		}
		else if(car_array[id].speed == 4){
			car_aray[id].hold_time = int ((2.0/3.0)/CARLENGTH);
		}
		else if(car_array[id].speed == 5){
			car_array[id].hold_time = int (.5 / CARLENGTH);
		} 
		hold(car_array[id].hold_time);
	} 

	
}


