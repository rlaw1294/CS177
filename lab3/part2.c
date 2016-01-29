//Rachel Law 861071722 rlaw001@ucr.edu

// Simulation of the Hertz airport shuttle bus, which picks up passengers
// from the airport terminal building going to the Hertz rental car lot.

#include "cpp.h"
#include <string.h>

//-----------------------------------------------------------------------
// The time, a "keep going" control, and some constants of the model.
//
// There are several other global variables. Should we use them?
// Could we get rid of them? Where would they appear if we broke the
// program up into several separately-compiled modules? We certainly
// ought to do that!
//-----------------------------------------------------------------------

double simulationTime; // A major global variable: the current time.
bool simulating; // Are we still going?

const double profit = .025; // dollars per litre
const double cost = 20;
// the incremental cost in dollars to operate one pump for a day


stream * arrivalStream; // auto arrival times
stream * litreStream; // number of litres needed
stream * balkingStream; // balking probability
stream * serviceStream; // service times
stream * dieselStream;

facility_set *diesel_gas_pumps;
facility_set *diesel_only_pumps;

void departures();
void dep_cust();
long group_size();

int num_diesel_gas_pumps;
int num_diesel_only_pumps;

int totalArrivals = 0;
double totalLitresSold = 0.0;
int customersServed = 0;
int balkingCustomers = 0;
double totalLitresMissed = 0.0;
double totalWaitingTime = 0.0;
double totalServiceTime = 0.0;
int max_queue = 0;
double totalEmptyQueueTime = 0.0;
double lastCustomerTime = 0.0;

bool doesCarBalk (double litres, int queueLength);

extern "C" void sim()		// main process
{
	printf("Enter the number of pumps that serve both diesel and gas: ");
	scanf("%d", &num_diesel_gas_pumps);
	diesel_gas_pumps = new facility_set("vending machine", num_diesel_gas_pumps);

	printf("Enter the number of pumps that serve diesel only: ");
	scanf("%d", &num_diesel_only_pumps);
	diesel_only_pumps = new facility_set("vending machine", num_diesel_only_pumps);
	
	stream * arrivalStream = new stream();
	stream * litreStream = new stream();
	stream * balkingStream = new stream();
	stream * serviceStream = new stream();
	stream * dieselStream = new stream();

	int seed;
	printf("arrival stream seed: ");
	scanf("%d", &seed);
	arrivalStream->reseed(seed);
	printf("litre stream seed: ");
	scanf("%d", &seed);
	litreStream->reseed(seed);
	printf("balking stream seed: ");
	scanf("%d", &seed);
	balkingStream->reseed(seed);
	printf("service stream seed: ");
	scanf("%d", &seed);
	serviceStream->reseed(seed);
	printf("diesel stream seed: ");
	scanf("%d", &seed);
	dieselStream->reseed(seed);

	
	printf ("%9s%7s%8s%9s%8s%7s%9s%7s%8s%7s%7s\n", " Current", "Total ",
		"NoQueue", "Car->Car", "Average", "Number", "Average", "Pump ",
		"Total", " Lost ", " Max ");
	printf ("%9s%7s%8s%9s%8s%7s%9s%7s%8s%7s%6s\n", "   Time ", "Cars ",
		"Fraction", "  Time  ", " Litres ", "Balked", "  Wait ",
		"Usage ", "Profit", "Profit", "Size");
	//for (int i = 0; i < 79; i++)
	//	cout << "-";
	//cout << "\n";



	create("sim");
	departures();		// start a stream of departing customers
	hold (1440);		// wait for a whole day (in minutes) to pass
	report();
}

bool doesCarBalk (double litres, int queueLength)
{
	return queueLength > 0 && balkingStream->uniform(0,1)
		> (40 + litres) / (25 * (3 + queueLength));
}

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

void dep_cust()
{
	create("dep_cust");
	totalArrivals+=1;
		
	//Does car need diesel?
	double diesel = dieselStream->uniform(0,1);
	bool needs_diesel = false;
	if (diesel <= 0.05) needs_diesel = true;		
	
	long min_qlength = (*diesel_only_pumps)[0].qlength();	
	bool use_hybrid_pump = false;
	int min_qlength_index = 0;
	for (int i=0; i<num_diesel_gas_pumps; i++) {
		if( !needs_diesel && (*diesel_gas_pumps)[i].qlength() < min_qlength ) {
			min_qlength = (*diesel_gas_pumps)[i].qlength();
			min_qlength_index = i;
			use_hybrid_pump = true;
		}
	}
	for (int i=0; i<num_diesel_only_pumps; i++) {
		if( (*diesel_only_pumps)[i].qlength() < min_qlength ) 
		{
			min_qlength = (*diesel_gas_pumps)[i].qlength();
			min_qlength_index = i;
			use_hybrid_pump = false;
		}
	}

	//Does car balk?
	double litresNeeded = litreStream->uniform(10,60);
	if (doesCarBalk(litresNeeded, min_qlength)) {
		balkingCustomers += 1;
		totalLitresMissed += litresNeeded;
		return;
	}
	else 
	{
		totalLitresSold += litresNeeded;
		if (use_hybrid_pump) (*diesel_gas_pumps)[min_qlength_index].reserve();
		else (*diesel_only_pumps)[min_qlength_index].reserve();
		hold(uniform(0,150 + serviceStream->normal(litresNeeded/2, 30)));
		if (use_hybrid_pump) (*diesel_gas_pumps)[min_qlength_index].release();
		else (*diesel_only_pumps)[min_qlength_index].release();
		customersServed += 1;
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
