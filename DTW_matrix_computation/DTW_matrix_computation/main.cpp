#include <iostream>
#include <vector>
#include <math.h>
#include "DTW.h"
#include "signal_preproc.h"
#include "cepstral_proc.h"
#include "VAD.h"

using namespace std;

vector<vector<double> > make_mfcc(vector<double> signal, int frame_lenght_ms, int frame_step_ms, int fs, int num_bands, int fmin, int fmax, int num_ceps)
{
	//short time analysis
	int frame_lenght = (frame_lenght_ms / 1000.0) * fs;
	int frame_step = (frame_step_ms / 1000.0) * fs;
	int frame_count = ((signal.size() - frame_lenght) / frame_step) + 1;
	vector<double> output;
	vector<vector<double> > matrix;
	vector<double> frame(frame_lenght);

	// generovani vahovaciho okna
	vector<double> hamming_window = hamming(frame_lenght);

	//vypocet banky filtru
	vector<vector<double> > filter_bank = compute_fbank(num_bands, fs, frame_lenght, fmin, fmax);

	for (int i = 0; i < frame_count; i++) {
		for (int j = 0; j < frame_lenght; j++) {
			frame[j] = (signal[(i * frame_step) + j]);
		}
//		if(VAD_energy(frame,power(load_signal("ticho.raw"))*2)){
            output = (compute_mfcc(filter_bank, hamming_window, frame, num_ceps));
            matrix.push_back(output);
//		}
	}

	return matrix;
}


int main()
{
    cout << "Run" <<endl;

///*
    vector< vector<double> > test3;
    vector< vector<double> > test4;

    for(int i = 0;i<10;i++){
        vector<double> test1;
        for(int j=0;j<5;j++){
            test1.push_back((i*j)^3);
            //cout << test1.back() << ", ";
        }
        test3.push_back(test1);
        //cout<<endl;
    }

    for(int i = 0;i<10;i++){
        vector<double> test1;
        for(int j=0;j<5;j++){
            test1.push_back((i*j)^6);
            //cout << test1.back() << ", ";
        }
        test4.push_back(test1);
        //cout<<endl;
    }//*/

    vector<vector<double> > sigs = {load_signal("jedna.raw"),load_signal("dva.raw"),load_signal("tri.raw"),load_signal("ctyry.raw")};
    vector<double> pattern = load_signal("jedna_test.raw");
    for(int i = 0;i<sigs.size();i++){
        cout << DTW_dist(make_mfcc(sigs[i], 25, 10, 16000, 24, 0, 8000, 13),make_mfcc(pattern, 25, 10, 16000, 24, 0, 8000, 13),cepstral_dist_c0) << endl;
    }

//    make_mfcc(load_signal("audio.raw"), 25, 10, 16000, 24, 0, 8000, 13);

    cout << "Done" << endl;
    return 0;
}



