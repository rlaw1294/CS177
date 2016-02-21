#include <iostream>
#include "cpp.h"
#include <string>
#include <stdio.h>
using namespace std;

#define TINY 1.e-20		// a very small time period
#define NUMCELLS 20	// 120 cells aka 1/4 mi 
//#define TOTALTIME 25	// 1440 minutes in a day. 24hr * 60min/hr
#define NUMCARS 3
#define CARLENGTH 2.0

facility_set *road;

class Car {
	public:
		int id;
		int nose;
		int tail;
		int speed;
		int distanceTraveled;
		bool isSlowing;
		double hold_time;
		Car():id(0), nose(0), tail(0), speed(0), distanceTraveled(0), isSlowing(false), hold_time(0) {}
		void setup_car(int index);
		void print_info(int id);
		void move_car(int id);
};

void Car::setup_car(int index) {
	cout << "setting up car " << index << endl;
	index += 1;
	id = index;
	nose = 2*index;
	tail = 2*index-1;
	speed = 1;
	distanceTraveled = 0;
	isSlowing = false;
	hold_time = index+TINY*index;
	(*road)[nose].reserve();
	(*road)[tail].reserve();
}

void Car::print_info(int id) {
	cout << "id: " << this->id << "\tnose: " << nose << "\ttail: " << tail << "\tspeed: " << speed << "\tDistance_travled: " <<  distanceTraveled << "\tisSLowing: " << isSlowing << "\thold_time: " << hold_time << endl;
}

int num_cells = NUMCELLS;
int num_cars = NUMCARS;
Car car_array[NUMCARS];
void car_maker();
void monitorCarCollision(int id);

int total_time = 25;

extern "C" void sim()		// main process
{
	trace_on();
	create("sim");
	road = new facility_set("road", NUMCELLS);
	/*for (int i=0; i<NUMCARS; i++) {
		car_array[i].setup_car(i);
		move_cars(i);
	}*/
	
	car_maker();

	hold(total_time);
	//hold (TOTALTIME);
	// Print Car Info
	for (int i=0; i<NUMCARS; i++) {
		car_array[i].print_info(i);
	}
}

void car_maker() {
	create("car_maker");
	for (int i=0; i<NUMCARS; i++) {
		car_array[i].setup_car(i);
	}

	for (int i=0; i<NUMCARS; i++) {
		car_array[i].move_car(i);
	}
}

void monitorCarCollision(int id){
	// lookAhead vals assumed to be directly proportionate to speed.
	// undefined if cell is not reserved or speed = 0
	int lookAhead = 0;
	if (car_array[id].speed == 0) { lookAhead = 0; }
	else if (car_array[id].speed == 1) { lookAhead = 2; }
	else if (car_array[id].speed == 2) { lookAhead = 3; }
	else if (car_array[id].speed == 3) { lookAhead = 4; }
	else if (car_array[id].speed == 4) { lookAhead = 6; }
	else if (car_array[id].speed == 5) { lookAhead = 8; }
	else { lookAhead = 0; }

	//create an array of potentional vehicles that we can possibly colide with
	for(int i = 0; i < lookAhead; i++){
		if( car_array[id + 1 + i].id != 0){
			//yes to crash
			if(car_array[id].speed - 2 < 0){
				car_array[id].speed = max(0, car_array[id + 1 + i].speed);
			}
			else{
				car_array[id].speed = max(car_array[id].speed - 2, car_array[id + 1 + i].speed);
			}
			break;
		} 
	}
	
}

void Car::move_car(int id) {
	create("move_car");
	
	while (clock < total_time){
		cout << "id: " << id << "\tMOVE CARS!\n";
		//cout << "clock: " << clock << "\ttotal_time: " << TOTALTIME << endl;
		//detect for car collision
		//monitorCarCollision(id);
		
		/*if(car_array[id].speed == 0){
			car_array[id].speed = 1;
			car_array[id].isSlowing = false;
		}		
		
		//adjusting speed for every car-length boundary
		if(car_array[id].distanceTraveled % 2 == 0){
			if(car_array[id].speed < 5){
				car_array[id].speed++;
			}
		}*/
		//calculate the hold time based on speed
		/*car_array[id].speed = 5;
		if(car_array[id].speed == 0){
			car_array[id].hold_time = 0;
		}
		else if(car_array[id].speed == 1){
			car_array[id].hold_time = (3.0/CARLENGTH);
		}
		else if(car_array[id].speed == 2){
			car_array[id].hold_time = ((11.0/6.0)/CARLENGTH);
		}
		else if(car_array[id].speed == 3){
			car_array[id].hold_time = (1.0/CARLENGTH);
		}
		else if(car_array[id].speed == 4){
			car_array[id].hold_time = ((2.0/3.0)/CARLENGTH);
		}
		else if(car_array[id].speed == 5){
			car_array[id].hold_time = (.5/CARLENGTH);
		} */

		// Release previous nose, Reserve nose cell it will move to
		//(*road)[car_array[id].nose % NUMCELLS].release();	
		car_array[id].nose++;
		//(*road)[car_array[id].nose % NUMCELLS].reserve();
		
		// Hold for time for car to move
		hold(car_array[id].hold_time);
		
		cout << "clock: " << clock << "\ttotal_time: " << total_time << endl;
		// Car has moved to next cell. Release previous tail cell, Reserve new tail cell
		car_array[id].distanceTraveled++;
		//(*road)[car_array[id].tail % NUMCELLS].release();
		car_array[id].tail++;
		//(*road)[car_array[id].tail % NUMCELLS].reserve();
		car_array[id].speed++;
	}
}


