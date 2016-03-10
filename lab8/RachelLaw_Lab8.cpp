#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#define NUM_SAMPLES 100000

using namespace std;

vector<int> rand_samples;
vector<double> bin_results;

void fill_rand_samples(vector<int> &v, int modulo);
void fill_bin_counts(vector<int> sample_vector, vector<double> &bin_vector);
void run_lab_generator(vector<int> &sample_vector, vector<double> &bin_vector, int bins, int sample_size);
void print_vector(vector<int> v) { for (int i=0; i<v.size(); i++) cout << v.at(i) << endl; }
void print_vector(vector<double> v) { for (int i=0; i<v.size(); i++) cout << v.at(i) << endl; }
void print_bin_vector(vector<double> v) { cout <<"bin_count / sample_size:\n"; for (int i=0; i<v.size(); i++) cout << "Bin " << i << ": " << v.at(i) << endl; }

void fill_rand_samples(vector<int> &v, int modulo) {
	for (int i=0; i<v.size(); i++) {
		v.at(i) = rand() % modulo;
	}
}

void fill_bin_counts(vector<int> sample_vector, vector<double> &bin_vector) {
	int one = 1;
	for (int i=0; i<sample_vector.size(); i++) {
		for (int j=0; j<bin_vector.size(); j++) {
			if ((sample_vector.at(i) >> j) & 1 == 1) bin_vector.at(j)++;
			//if (j==31) cout << j/*((sample_vector.at(i) >> j) & one)*/ << endl;
		}
	}

	// convert each bin to a percent
	for (int i=0; i<bin_vector.size(); i++) {
		bin_vector.at(i) /= sample_vector.size();
	}
}

void run_lab_generator(vector<int> &sample_vector, vector<double> &bin_vector, int bins, int sample_size) {
	cout << "Num Samples: " << sample_size << endl;
	cout << "Num Bins: " << bins << endl;

	// Clear
	sample_vector.clear();
	bin_vector.clear();
	sample_vector.resize(sample_size);
	bin_vector.resize(bins);

	// Do stuff
	fill_rand_samples(sample_vector, pow(2,bins));
	fill_bin_counts(sample_vector, bin_vector);
	print_bin_vector(bin_vector);
}


int main() {
	srand(time(NULL));
	int bins = 0;

	// 2 bins
	bins = 2;
	run_lab_generator(rand_samples, bin_results, bins, NUM_SAMPLES);

	cout << endl;
	// 32 bins
	bins = 32;
	run_lab_generator(rand_samples, bin_results, bins, NUM_SAMPLES);
}