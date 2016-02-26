// Rachel Law (861071722)

#include <iostream>
#include "cpp.h"
#include <string.h>
#include <math.h>
using namespace std;

#define MEAN 143.772928
#define STDDEV 2.266593764
#define NUMDATA 125

extern "C" void sim()
{
	create("sim");
	
	double data[NUMDATA];
	for (int i=0; i<NUMDATA; i++) {
		data[i] = normal(MEAN, STDDEV);
		cout << data[i] << endl;
	}	

}

