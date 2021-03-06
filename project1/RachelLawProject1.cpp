#include <iostream>
#include "cpp.h"
#include <string>
#include <stdio.h>
#include <iomanip>
using namespace std;

#define TINY 1.e-20	// a very small time period
#define NUMCELLS 120	// 120 cells aka 1/4 mi, EASIER TO SEE TRAFFIC LIGHT RESULTS/EFFECTS WITH LESS CELLS
#define TOTALTIME 100	// 1440 minutes in a day. 24hr * 60min/hr, 1440 is too long for this simulation!!!
#define NUMCARS 10	// can have up to 10 cars for our print func to properly output nicely
#define CARLENGTH 2
#define TRAFFICLIGHT NUMCELLS-2 // traffic light at cells, 118 to 119


class Car {
	public:
		int id;
		int nose;
		int tail;
		int speed;
		int target_speed;
		int distanceTraveled;
		int acceleration;
		double hold_time;
		Car():id(0), nose(0), tail(0), speed(0), target_speed(0), distanceTraveled(0), acceleration(0), hold_time(0) {}
		void setup_car(int index);
		void print_info(int id);
		void move_car(int id);
};

facility_set *road;
Car car_array[NUMCARS];	
int num_cells = NUMCELLS;
int num_cars = NUMCARS;
void print_track();
int get_lookahead_indicator(int id);
void traffic_light();

void Car::setup_car(int index) {
	cout << "setting up car " << index << endl;
	//index += 1;
	id = index;
	index+=1; //+=1 to avoid seg fault
	nose = 2*index;
	tail = 2*index-1;
	speed = 0;
	target_speed = 5;
	distanceTraveled = 0;
	acceleration = 0;
	hold_time = 1;
}

void Car::print_info(int id) {
	print_track();
	cout 	<< "\tid: " << this->id 
		<< setw(9) << "\tclk: " << clock 
		<< setw(8) << "\tpos: " << tail << "," << nose
		//<< setw(9) << "\tnose: " << nose 
		//<< setw(9) << "\ttail: " << tail 
		<< setw(7) << "\tspd: " << speed 
		<< setw(9) << "\ttgtspd: " << target_speed
		<< setw(9) << "\ttotdist: " <<  distanceTraveled 
		<< setw(9) << "\tacc: " << acceleration; 
		//<< setw(8) << "\thold: " << hold_time << endl;
	cout << endl;
}

void print_track() {
	int track[NUMCELLS];
	for (int i=0; i<NUMCELLS; i++) track[i] = -1;
	for (int i=0; i<NUMCARS; i++) {
		track[car_array[i].nose] = car_array[i].id;
		track[car_array[i].tail] = car_array[i].id;
	}
	for (int i=0; i<NUMCELLS; i++) {
		if (track[i] == -1) cout << "-";
		else cout << track[i];
	}
	//cout << endl;
}

extern "C" void sim()		// main process
{
	//trace_on();
	create("sim");
	road = new facility_set("road", NUMCELLS);
	
	for (int i=0; i<NUMCARS; i++) car_array[i].setup_car(i);
	traffic_light();
	for (int i=0; i<NUMCARS; i++) car_array[i].move_car(i);
	
	hold (TOTALTIME);
	// Print Car Info
	//for (int i=0; i<NUMCARS; i++) {
	//	car_array[i].print_info(i);
	//}
}

// Performs the lookahead
// Return -1 if will crash -> should decelerate
// Return 0 if will NOT crash and at target speed
// Return 1 if will NOT crash and slower than target speed -> should accelerate
int get_lookahead_indicator(int id){
	// lookAhead vals assumed to be directly proportionate to speed.
	// undefined if cell is not reserved or speed = 0
	int lookAhead = 0;
	if (car_array[id].speed == 0) { lookAhead = 2; }
	else if (car_array[id].speed == 1) { lookAhead = 2; }
	else if (car_array[id].speed == 2) { lookAhead = 2; }
	else if (car_array[id].speed == 3) { lookAhead = 4; }
	else if (car_array[id].speed == 4) { lookAhead = 6; }
	else if (car_array[id].speed == 5) { lookAhead = 8; }
	else { lookAhead = 2; }

	for (int i=1; i<=lookAhead; i++) {
		if ((*road)[car_array[id].nose+i % NUMCELLS].status() == BUSY) return -1;
	}

	if (car_array[id].speed < car_array[id].target_speed) return 1;
	return 0;
}

void Car::move_car(int id) {
	create("move_car");
	
	(*road)[car_array[id].nose].reserve();
	(*road)[car_array[id].tail].reserve();
	hold(TINY);
	
	int cells_moved = CARLENGTH;
	while (clock < TOTALTIME){
		//cout << "id: " << id << "\tMOVE CARS!\n";
		//cout << "clock: " << clock << "\ttotal_time: " << TOTALTIME << endl;
		
		//calculate the hold time based on current speed
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
		} 
	
		// lookahead for collisions if speed should be adjusted after traverse the car length
		// then adjust speed later (after actually moved the car length)
		if (cells_moved>=CARLENGTH) car_array[id].acceleration = get_lookahead_indicator(id);
		
		// Only move if speed is greater than 0
		if (car_array[id].speed > 0 ) {
			// Release previous nose, Reserve nose cell it will move to
			//cout << "RELEASING " << car_array[id].nose % NUMCELLS << endl;
			(*road)[car_array[id].nose % NUMCELLS].release();	
			car_array[id].nose++;
			car_array[id].nose %= NUMCELLS;
			//cout << "RESERVING " << car_array[id].nose % NUMCELLS << endl;
			(*road)[car_array[id].nose % NUMCELLS].reserve();
			
			// Hold for time for car to move
			hold(car_array[id].hold_time);
			
			// Car has moved to next cell. Release previous tail cell, Reserve new tail cell
			(*road)[car_array[id].tail % NUMCELLS].release();
			car_array[id].tail++;
			car_array[id].tail %= NUMCELLS;
			(*road)[car_array[id].tail % NUMCELLS].reserve();
			car_array[id].distanceTraveled++;
			
			// Cells moved counter to indicate when we can lookahead/adjust the speed
			cells_moved += 1;
		}
		else {
			hold(.5/CARLENGTH); // hold enough time to see if some car in front has accelerate
		}
		// Adjust speed if car has moved a car length
		if (cells_moved>=CARLENGTH) {
			if (car_array[id].acceleration == -1) {
				car_array[id].speed -= 2;
				if (car_array[id].speed < 0) car_array[id].speed = 0;
			}
			else if (car_array[id].acceleration == 1 && car_array[id].speed < 5) car_array[id].speed += 1;
			
			// need to keep waiting/looking to accelerate if our speed gets set to 0
			if (car_array[id].speed > 0) cells_moved = 0;
		}

		// Print Info
		car_array[id].print_info(id);
	}
}

void traffic_light() {
	create("traffic_light");
	
	// start as red light
	(*road)[TRAFFICLIGHT].reserve();
	(*road)[TRAFFICLIGHT+1].reserve();
	hold(uniform(30.0/60.0,90.0/60.0));
	double hold_time = 0.0;
	while (clock < TOTALTIME) {
		// Green - exponential dist 2 minutes
		(*road)[TRAFFICLIGHT].release();
		(*road)[TRAFFICLIGHT+1].release();
		hold_time = expntl(2);
		cout << " GREEN LIGHT: " << hold_time << " minutes\n";
		hold(hold_time);
		// Yellow - 10 seconds
		(*road)[TRAFFICLIGHT].reserve();
		(*road)[TRAFFICLIGHT+1].reserve();
		hold_time = 10.0/60.0;
		cout << " YELLOW LIGHT: " << hold_time << " minutes\n";
		hold(hold_time);
		// Red - uniform between 30 and 90
		hold_time = uniform(30.0/60.0 , 90.0/60.0);
		cout << " RED LIGHT: " << hold_time << " minutes\n";
		hold(hold_time);
	}
}

