// Rachel Law (861071722)

#include <iostream>
#include "cpp.h"
#include <string.h>
#include <math.h>
using namespace std;

#define SAMPLE_SIZE 32

double A = 10.0;
double *left_input;
double *right_input;
double *uniform_input;
double *left_triangle_array;
double *right_triangle_array;
double *uniform_array;


double PDFLeftTriangle(double a) {
	//return uniform(0,a);
	return ((2/a)*(a-uniform(0,a)))/(a);
}

double PDFRightTriangle(double a) {
	return ((2/a)*(uniform(0,a)))/(a);
}

double PDFUniform(double a) {
	return 1/a;
}

void FillPDFLeftTriangle_array(double *ar, int ar_size, double a) {
	for (int i=0; i<ar_size; i++) {
		ar[i] = PDFLeftTriangle(a);
	}
}

void FillPDFRightTriangle_array(double *ar, int ar_size, double a) {
	for (int i=0; i<ar_size; i++) {
		ar[i] = PDFRightTriangle(a);
	}
}

void FillPDFUniform_array(double *ar, int ar_size, double a) {
	for (int i=0; i<ar_size; i++) {
		ar[i] = PDFUniform(a);
	}
}

void print_array(double* ar, int ar_size) {
	for (int i=0; i<ar_size; i++) {
		cout << ar[i] << endl;
	}

}

extern "C" void sim()
{
	create("sim");
	
	left_input = new double [SAMPLE_SIZE];
	right_input = new double [SAMPLE_SIZE];
	uniform_input = new double [SAMPLE_SIZE];
	left_triangle_array = new double [SAMPLE_SIZE];
	right_triangle_array = new double [SAMPLE_SIZE];
	uniform_array = new double [SAMPLE_SIZE];
	
	FillPDFLeftTriangle_array(left_triangle_array, SAMPLE_SIZE, A);
	FillPDFRightTriangle_array(right_triangle_array, SAMPLE_SIZE, A);
	FillPDFUniform_array(uniform_array, SAMPLE_SIZE, A);


	print_array(left_triangle_array, SAMPLE_SIZE);
	print_array(right_triangle_array, SAMPLE_SIZE);
	print_array(uniform_array, SAMPLE_SIZE);
	
	

}
