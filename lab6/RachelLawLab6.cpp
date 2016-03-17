// Rachel Law (861071722)

#include <iostream>
#include "cpp.h"
#include <string.h>
#include <math.h>
using namespace std;

#define A 10.0
#define MAX_SAMPLE_SIZE 32
double *samples;
double *left_tri_results;
double *right_tri_results;
double *uniform_results;


double PDFLeftTriangle(double A, double x) return ((2/A)*(A-x))/A;
double PDFRightTriangle(double A, double x) return ((2/A)*(x))/A;
double PDFUniform(double a) return 1/a;

// Sample size should be same as results size
void DoPDF(double *samples, double *results, int samples_size, double A, double (*PDFFunc)(double,double)) {
	// Get Samples
	for (int i=0; i<samples_size; i++) {
		double samples[i] = uniform(0,1);
		results[i] = PDFFunc(A, samples[i]);
	}
}

void PrintArray(double* ar, int ar_size) {
	for (int i=0; i<ar_size; i++) {
		cout << ar[i] << endl;
	}
}

void printCSV(vector<double> v) {
	fstream myfile;
	myfile.open("lab6.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	myfile << "NEW RUN\n";
	for (unsigned int i = 0; i < v.size(); i++) {
		myfile << v.at(i) << endl;
	}
	myfile << ", ";
}

extern "C" void sim()
{
	// Edit this vector for all sample sizes you want to try
	vector<int> samples_sizes[] = {MAX_SAMPLE_SIZE};

	create("sim");
	samples = new double [MAX_SAMPLE_SIZE];
	left_tri_results = new double [MAX_SAMPLE_SIZE];
	right_tri_results = new double [MAX_SAMPLE_SIZE];
	uniform_results = new double [MAX_SAMPLE_SIZE];

	for (int i=0; i<samples_sizes.size(); i++) {
		DoPDF(samples, left_tri_results, samples_sizes.at(i), A, PDFLeftTriangle);
		DoPDF(samples, right_tri_results, samples_sizes.at(i), A, PDFRightTriangle);
		DoPDF(samples, uniform_results, samples_sizes.at(i), A, PDFUniform);
	}
	
	print_array(left_triangle_array, MAX_SAMPLE_SIZE);
	print_array(right_triangle_array, MAX_SAMPLE_SIZE);
	print_array(uniform_array, MAX_SAMPLE_SIZE);
}
