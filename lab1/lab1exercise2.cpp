#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <vector>
using namespace std;

bool in_circle(double &x, double &y);
vector<double> estimate_pi(int num);
double distance_from_origin(double &x, double &y);

// Returns Euclidean Distance from Origin
double distance_from_origin(double &x, double &y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

// Returns if coordinate (x,y) is in the unit circle
bool in_circle(double &x, double &y) {
	return (distance_from_origin(x, y) <= 1.0) ? true : false;
}




vector<double> estimate_pi(int num = 500) {
	vector<double> v;
	double yes = 0.0;
	for (int i = 0; i<num; i++) {
		double x = (rand() % 10000) / 10000.0;
		//cout << "X: " << x;
		double y = (rand() % 10000) / 10000.0;
		//cout << "  Y: " << y << endl;
		if (in_circle(x, y)) yes++;
		v.push_back((yes / (v.size() + 1)) * 4.0);
	}
	return v;
}

void printCSV(vector<double> v) {
	fstream myfile;
	myfile.open("test.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	myfile << "NEW RUN\n";
	for (unsigned int i = 0; i < v.size(); i++) {
		myfile << v.at(i) << endl;
	}
	myfile << ", ";
}

// A = pi*r^2
int main() {
	srand(time(NULL));
	vector<double> v;
	for (int i = 0; i < 6; i++) {
		v = estimate_pi();
		printCSV(v);
	}
}


