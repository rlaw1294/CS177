// Toy program to test race conditions in CSIM - version 2 using inspectors
#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;

#define TINY 1.e-20		// a very small time period

facility_set podium ("podium",2);	// Where you can speak your mind
double podium_done[2];	// Holds promised release time by current speaker

void hiho(int);	// process declarations
string whoami[5] = {"Hi ", "Ho ", "Hum", "Foo", "Bar"}; // their names

extern "C" void sim(int argc, char* argv[])		// main process
{
	int players;
	create("sim");
	if (argc>1) players = atoi(argv[1]);
	else players = 2;
	cout << "Number of players is " << players << endl;
	for(int i=0;i<players;i++)
		hiho(i);		// start a talking thread
	hold (25);	// wait for a whole day (in minutes) to pass
}

void hiho(int me)
{
	create ("hiho");
	int myChoice;	// which podium I will speak at next
	double myStart, myTurn;	// clock times I started trying, expect I won
	while(clock < 25.)	//
	{
		myChoice = uniform_int(0,1);	// pick a location
		myStart = clock;
		while (podium[myChoice].status() == BUSY)  // wait if occupied
		{
			hold(podium_done[myChoice] -  clock);
//			hold(podium_done[myChoice] -  clock - TINY);
		}
		myTurn = clock; // Done waiting, hope I can talk now
		podium[myChoice].reserve();
		cout << whoami[me] << " gets podium " << myChoice << " at " << clock;
		cout << " (waited " << myTurn - myStart
		<< ", blocked " << clock - myTurn << ")" << endl;
		podium_done[myChoice] = clock +1;
		hold (1);	// blah! blah! blah!
		podium[myChoice].release();
	}
}

