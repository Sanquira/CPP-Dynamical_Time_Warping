#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include <fftw3.h>

using namespace std;
vector<double> sig;

vector<double> compute_fft_power(vector<double> vector_in)
{
	int N = vector_in.size();
	fftw_complex in[N], out[N];	/* double [2] */
	fftw_plan p;
	vector<double> output(N / 2 + 1);

	for (int i = 0; i < N; i++) {
		in[i][0] = vector_in[i];
		in[i][1] = 0;
	}

	/* forward Fourier transform, save the result in 'out' */
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);

	for (int j = 0; j < (N / 2) + 1; j++) {
		output[j] = sqrt(out[j][0] * out[j][0] + out[j][1] * out[j][1]);
		output[j] = output[j] * output[j];
	}

	fftw_destroy_plan(p);
	fftw_cleanup();
	return output;
}

void load_signal(string name)
{
	int16_t i;
	ifstream file(name, ios::binary);

	if (!file) {
		cerr << "Error opening file" << "\n";
		exit(EXIT_FAILURE);
	}

	while (file.read(reinterpret_cast < char *>(&i), sizeof(i))) {
		sig.push_back(i / 32768.0);	//normalize<-1,1>
	}

	file.close();
}

vector<double> multiple_vectors(vector<double> vector1, vector<double> vector2)
{
	vector<double> result(vector1.size());

	for (int i = 0; i < vector1.size(); i++) {
		result[i] = vector1[i] * vector2[i];
	}

	return result;
}

void print_vector(vector<double> vec)
{
	cout << "\n";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
	}
	cout << "\n";
}

void print_vector2(vector<vector<double> >vec)
{
	cout << "\n";
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < vec[0].size(); j++) {
			cout << vec[i][j] << " ";
		}
		cout << ";" << "\n";
	}
	cout << "\n";
}

void print_array(double *arr, int size)
{
	for (int i = 0; i < size; i++) {
		cout << arr[i] << "\n";
	}
}

vector<double> zeros(int size)
{
	vector<double> output(size);
	for (int i = 0; i < size; i++) {
		output[i] = 0;
	}
	return output;
}

vector<double> hamming(int size)
{
	vector < double >hamming_window(size);
	for (int i = 0; i < size; i++) {
		hamming_window[i] = 0.54 - 0.46 * cos(2 * M_PI * i / (size - 1));
	}
	return hamming_window;
}

double mel(int value)
{
	return 1125 * log(1 + value / 700.0);
}

double inv_mel(int value)
{
	return 700 * (pow(2.71828, (value / 1125.0)) - 1);
}

vector<double> DCT(vector<double> x)
{
	int N = x.size();
	vector<double> X(N);

	for (int k = 0; k < N; ++k) {
		double sum = 0.;
		double s = (k == 0) ? sqrt(.5) : 1.;

		for (int n = 0; n < N; ++n) {
			sum += s * x[n] * cos(M_PI * (n + .5) * k / N);
		}
		X[k] = sum * sqrt(2. / N);
	}
	return X;
}

vector<vector<double> > compute_fbank(int num_bands, int fs, int num_dft, int fmin, int fmax)
{
	double melmin = mel(fmin);
	double melmax = mel(fmax);
	double step = (melmax - melmin) / (num_bands + 1);
	vector<double> melbank(num_bands + 2);
	vector<double> fbank(num_bands + 2);
	vector<double> f(num_bands + 2);
	double Wmel[num_bands][num_dft / 2 + 1];
	vector<double> H(num_dft / 2 + 1);
	vector<vector<double> > FilterBank;

	for (int i = 0; i < num_bands; i++) {
		for (int j = 0; j < num_dft / 2 + 1; j++) {
			Wmel[i][j] = 0;
		}
	}

	melbank[0] = melmin;
	melbank[num_bands + 2] = melmax;
	fbank[0] = inv_mel(melbank[0]);
	fbank[num_bands + 2] = inv_mel(melbank[num_bands + 2]);

	for (int i = 1; i < num_bands + 2; i++) {
		melbank[i] = melbank[i - 1] + step;
		fbank[i] = inv_mel(melbank[i]);
	}

	for (int i = 0; i < num_bands + 2; i++) {
		f[i] = floor((num_dft + 1) * fbank[i] / fs);
	}

	for (int m = 0; m < num_bands; m++) {
		for (int k = 0; k < H.size(); k++) {
			if (k < f[(m + 1) - 1]) {
				H[k] = 0;
			}
			if (f[(m + 1) - 1] <= k && k <= f[(m + 1)]) {
				H[k] = (k - f[(m + 1) - 1]) / (f[(m + 1)] - f[(m + 1) - 1]);
			}
			if (f[(m + 1)] <= k && k <= f[(m + 1) + 1]) {
				H[k] =(f[(m + 1) + 1] - k) / (f[(m + 1) + 1] - f[(m + 1)]);
			}
			if (k > f[(m + 1) + 1]) {
				H[k] = 0;
			}
		}
		FilterBank.push_back(H);
	}

	return FilterBank;
}

vector<double> compute_mfcc(vector<vector<double> > filter_bank, vector<double> window, vector<double> input, int ceps_coef)
{
	vector<double> power = compute_fft_power(multiple_vectors(input, window));	//checked
	vector<double> output = zeros(filter_bank.size());

	//filtering in frequency domain

	for (int radek = 0; radek < filter_bank.size(); radek++) {
		for (int sloupec = 0; sloupec < filter_bank[0].size();
		     sloupec++) {
			output[radek] = output[radek] + power[sloupec] * filter_bank[radek][sloupec];
		}
		output[radek] = log(output[radek]);
	}

	//apply DCT
	vector<double> dct = DCT(output);
	vector<double> result = zeros(ceps_coef);

	//select first N coefficients
	for (int i = 0; i < ceps_coef; i++) {
		result[i] = dct[i];
	}

	return result;
}

vector<vector<double> > make_mfcc(string signal_name, int frame_lenght_ms, int frame_step_ms, int fs, int num_bands, int fmin, int fmax, int num_ceps)
{
	load_signal(signal_name);

	//short time analysis
	int frame_lenght = (frame_lenght_ms / 1000.0) * fs;
	int frame_step = (frame_step_ms / 1000.0) * fs;
	int frame_count = ((sig.size() - frame_lenght) / frame_step) + 1;
	vector<double> output;
	vector<vector<double> > matrix;
	vector<double> frame(frame_lenght);

	// generovani vahovaciho okna
	vector<double> hamming_window = hamming(frame_lenght);

	//vypocet banky filtru
	vector<vector<double> > filter_bank = compute_fbank(num_bands, fs, frame_lenght, fmin, fmax);

	for (int i = 0; i < frame_count; i++) {
		for (int j = 0; j < frame_lenght; j++) {
			frame[j] = (sig[(i * frame_step) + j]);
		}
		output = (compute_mfcc(filter_bank, hamming_window, frame, num_ceps));
		matrix.push_back(output);
	}

	return matrix;
}
