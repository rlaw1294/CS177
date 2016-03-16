#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

bool in_circle(double &x, double &y);
double estimate_pi(const int &num);
double distance_from_origin(double &x, double &y);

// Returns Euclidean Distance from Origin
double distance_from_origin(double &x, double &y) {
	return sqrt(pow(x,2)+pow(y,2));
}

// Returns if coordinate (x,y) is in the unit circle
bool in_circle(double &x, double &y) {
	return (distance_from_origin(x,y) <= 1.0) ? true : false;
}

double estimate_pi(const int &num) {
	int yes = 0;
	for (int i = 0; i<num; i++) {
		double x = (rand() % 10000) / 10000.0;
		//cout << "X: " << x;
		double y = (rand() % 10000) / 10000.0;
		//cout << "  Y: " << y << endl;
		if (in_circle(x,y)) yes++;
	}
	return (double)yes/num; 
}


// A = pi*r^2
int main() {
	srand(time(NULL));
	double actual = 3.1415926;
	cout << "Monte Carlo\n";

	cout << "N\t\tEstimate\t\tDifference\n";
	for (int i=1; i<=10000000; i*=10) {
		double result = estimate_pi(i) * 4; //4 quadrants
		cout << i << "\t\t" << result << "\t\t\t" << result-actual << endl;
	}
}
