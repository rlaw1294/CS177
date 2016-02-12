// Rachel Law (861071722)

#include <iostream>
#include "cpp.h"
#include <string.h>
#include <math.h>
using namespace std;

double **U;
#define U_ROWS 10
#define U_COLS 10000
double t_val = 2.262; // t-val for 2-tailed, 95% CI, 9dof t-test

void fill_array(double **ar, int rows, int cols, unsigned int c_value);
void print_array(double **ar, int rows, int cols);
double calculateSampleMean(double *ar, int rows);
double calculateSampleVarianceTwo(double *ar, int rows, double mean);
double calculateSampleVarianceOne(double *ar, int rows, double mean);
double calculateConfidenceInterval(int sample_size, double mean, double std_dev, double &lower_bound, double &upper_bound, double tval); 
void lab_calculations(double **ar, unsigned int c_value);


extern "C" void sim()
{
	create("sim");
	
	U = new double* [U_ROWS];
	for (int i=0; i<U_ROWS; i++) {
		U[i] = new double [U_COLS];
	}

	lab_calculations(U, 0);
	lab_calculations(U, 10000);
	lab_calculations(U, 10000000);
	lab_calculations(U, 1000000000);

}

void lab_calculations(double **ar, unsigned int c_value) {
	
	cout << "C = " << c_value << endl;
	fill_array(ar, U_ROWS, U_COLS, c_value);
	//print_array(U, U_ROWS, U_COLS);
	
	for (int i=0; i<U_ROWS; i++) {
		double sample_mean = calculateSampleMean(ar[i], U_COLS);
		double sample_variance_two = calculateSampleVarianceTwo(ar[i], U_COLS, sample_mean);
		double sample_variance_one = calculateSampleVarianceOne(ar[i], U_COLS, sample_mean);
		double sample_std_dev = sqrt(sample_variance_one);
		double lower_bound = 0.0;
		double upper_bound = 0.0;
		calculateConfidenceInterval(U_COLS, sample_mean, sample_std_dev, lower_bound, upper_bound, t_val);
		cout << "Mean: " << sample_mean << "\tStdDev: " << sample_std_dev << "\t\tVar2: " << sample_variance_two << "\t\tVar1: " << sample_variance_one << "\t\t95\%CI: [" << lower_bound << ", " << upper_bound << "]" << endl;
	}
		cout << endl;

}

void fill_array(double **ar, int rows, int cols, unsigned int c_value) {
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			ar[i][j] = uniform(c_value,c_value+1);
		}
	}
}

void print_array(double **ar, int rows, int cols) {
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			cout << ar[i][j] << " ";
		}
		cout << endl;
	}
}

double calculateSampleMean(double *ar, int rows) {
	double sum = 0.0;
	for (int i=0; i<rows; i++) {
		sum += ar[i];
	}
	return sum/rows;
}

// 2 pass method
double calculateSampleVarianceTwo(double *ar, int rows, double mean) {
	double sum = 0.0;
	for (int i=0; i<rows; i++) {
		sum += pow(ar[i]-mean, 2);
	}
	return sum/(rows-1);
}

// 1 pass method
double calculateSampleVarianceOne(double *ar, int rows, double mean) {
	double sum = 0.0;
	for (int i=0; i<rows; i++) {
		sum += pow(ar[i], 2);
	}
	sum = sum - rows*pow(mean,2);
	return sum/(rows-1);
}

// Calculates a 2-tailed 95% CI with 9 dof
double calculateConfidenceInterval(int sample_size, double mean, double std_dev, double &lower_bound, double &upper_bound, double tval) {
	//sample_size = 10;
	double range = std_dev/sqrt(sample_size);
	range *= tval;
	lower_bound = mean - range;
	upper_bound = mean + range;
}
