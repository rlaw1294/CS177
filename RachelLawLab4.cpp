// Rachel Law (861071722)

#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;

#define TINY 1.e-20

facility_set *road;
double *road_cells;
int num_cars;
int road_size;
int total_simulation_time;

// processes
void create_car();
void car(int cell, int car_id);

extern "C" void sim()
{
	create("sim");

	cout << "Road size: ";
	cin >> road_size;
	cout << "Num cars: ";
	cin >> num_cars;
	cout << "Total simulation time: ";
	cin >> total_simulation_time;

	road = new facility_set("road", road_size);
	road_cells = new double[road_size];
	for (int i=0; i<road_size; i++) {
		road_cells[i] = 0.0;
	}

	create_car();
	hold(total_simulation_time);
}

void create_car()
{
	create("create_car");

	if (num_cars >= road_size) {
		cout << "Num cars >= road size. Deadlock\n";
		exit(0);
	}

	for (int i=0; i<num_cars; i++) {
		car(i,i);
	}
}

void car(int cell, int car_id)
{
	create("car");
		
	int cur_cell = cell;
	while (clock < total_simulation_time) {
		(*road)[cur_cell].reserve();
		double wait_time = expntl(1);	// our R variable
		int next_cell = (cur_cell + 1) % road_size;

		// Next cell is empty
		if (road_cells[next_cell] == 0.0) {
			road_cells[cur_cell] = clock + wait_time; // clock + R
			cout << "Car #" << car_id << "\tCur_cell: " << cur_cell << "\t Next_cell: " << next_cell << "\tHold_time: " << wait_time << endl;

			hold(wait_time);
			(*road)[cur_cell].release();

			road_cells[cur_cell] = 0.0;
			cur_cell = next_cell;
		}
		// Next cell has another car
		else {
			road_cells[cur_cell] = max( road_cells[next_cell]+TINY, clock+wait_time ); //max(D[j]+epsilon, clock+R)
			cout << "Car #" << car_id << "\tCur_cell: " << cur_cell << "\t Next_cell: " << next_cell << "\tHold_time: " << road_cells[cur_cell] - clock << "\t(Car Ahead)" << endl;

			hold(road_cells[cur_cell] - clock);	// D[i] - clock
			(*road)[cur_cell].release();

			road_cells[cur_cell] = 0.0;
			cur_cell = next_cell;
		}
	}

}
