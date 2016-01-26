// Simulation of the Hertz airport shuttle bus, which picks up passengers
// from the airport terminal building going to the Hertz rental car lot.

#include "cpp.h"
#include <string.h>

#define NUM_SEATS 6		// number of seats available on shuttle

#define TINY 1.e-20		// a very small time period

#define TERMNL 0		// named constants for labelling event set
#define CARLOT 1

qtable termnl ("termnl");	// customer queue at airport terminal
qtable carlot ("carlot");	// customer queue at rental car lot
qtable rest ("rest");		// dummy facility indicating an idle shuttle

mailbox on_termnl("terminal boarding pass");
mailbox on_carlot("carlot boarding pass");

event get_off_now ("get_off_now"); // all customers can get off shuttle

event_set shuttle_called ("call button", 2);
				// call buttons at terminal and car lot

void arrivals();	// function declarations
void arr_cust();
void departures();
void dep_cust();
void shuttle();
long group_size();

extern "C" void sim(int argc, char* argv[]) // main process
{
	long new_seed_val;
	if (argc>1) reseed(NIL, atoi(argv[1]));
	create("sim");
	arrivals();		// start a stream of arriving customers
	departures();		// start a stream of departing customers
	shuttle();		// create a single shuttle
	hold (1440);		// wait for a whole day (in minutes) to pass
	report();
}

// Model segment 1a: generate groups of customers arriving at the airport

void arrivals()
{
	create("arrivals");

	while(clock < 1440.)	//
	{
		hold(expntl(10)); // exponential interarrivals, mean 10 minutes
		long group = group_size();
		for (long i=0;i<group;i++)
			arr_cust();	// new customer appears at the airport
	}
}

// Model segment 1b: activities followed by individual  airport customers

void arr_cust()
{
	long shuttle_id;
	create("arr_cust");

	termnl.note_entry();	// join the queue at the airport terminal
	shuttle_called[TERMNL].set();	// make sure shuttle call button is set
	on_termnl.receive(&shuttle_id);	// wait for ticket from shuttle driver
	termnl.note_exit();	// I leave queue, driver clears call button when empty
	get_off_now.wait();	// everybody off when shuttle reaches car lot
}

// Model segment 2a: generate groups of customers at car lot, heading to airport

void departures()		// this model segment spawns departing customers
{
	create("departures");

	while(clock < 1440.)	//
	{
		hold(expntl(10)); // exponential interarrivals, mean 10 minutes
		long group = group_size();
		for (long i=0;i<group;i++)
			dep_cust();	// new customer appears at the car lot
	}
}

// Model segment 2b: activities followed by individual car lot customers

void dep_cust()
{
	long shuttle_id;
	create("dep_cust");

	carlot.note_entry();	// join the queue at the car lot
	shuttle_called[CARLOT].set();	// make sure shuttle call button is set
	on_carlot.receive(&shuttle_id);	// wait for a ticket from shuttle driver
	carlot.note_exit();	// I leave queue, driver clears call button when empty
	get_off_now.wait();	// everybody off when shuttle reaches car lot
}

// Model segment 3: the shuttle bus

void shuttle()
{
	create ("shuttle");
	long my_ID =1;
	long seats_used = 0;	// shuttle is initially empty
	while(1)		// loop forever
	{
		// start off in idle state, waiting for the first call...
		rest.note_entry();
		long who_pushed = shuttle_called.wait_any();
			// relax at garage till called from either location
		shuttle_called[who_pushed].set();
			// hack to satisfy loop exit, below
		rest.note_exit();
		hold(2);	// 2 minutes to get to car lot stop

		// go around the loop until there are no calls waiting
		while ((shuttle_called[TERMNL].state()==OCC)||
			(shuttle_called[CARLOT].state()==OCC))
		{
			// ready to load customers at car lot
			// invite them in, one by one, till all seats are full

			while((seats_used < NUM_SEATS) && (on_carlot.queue_cnt() > 0))
			{
				on_carlot.send(my_ID);// one person gets a ticket
				seats_used++;
				hold(TINY);
			}

			if (on_carlot.queue_cnt() == 0) // nobody left behind
				shuttle_called[CARLOT].clear();

			hold (uniform(3,5));	// drive to airport terminal

//trace_on();
			// drop off any passengers on board

			if(seats_used > 0)
			{
				get_off_now.set(); // open door and let them off
				seats_used = 0;
			}

			// ready to load customers at airport terminal
			// invite them in, one by one, till all seats are full

			while((seats_used < NUM_SEATS) && (on_termnl.queue_cnt() > 0))
			{
				on_termnl.send(my_ID);// one person gets a ticket
				seats_used++;
				hold(TINY);
			}

			if (on_termnl.queue_cnt() == 0) // nobody left behind
				shuttle_called[TERMNL].clear();

//trace_off();
			hold (uniform(3,5));	// drive to Hertz car lot

			// drop off any passengers on board

			if(seats_used > 0)
			{
				get_off_now.set(); // open door and let them off
				seats_used = 0;
			}
		}
	}
}

long group_size()	// function gives the number of customers in a group
{
	double x = prob();
	if (x < 0.3) return 1;
	else
	{
		if (x < 0.7) return 2;
		else
			return 4;
	}
}
