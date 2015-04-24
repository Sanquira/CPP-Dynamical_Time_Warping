#include <iostream>
#include <vector>
#include <math.h>
#include "DTW.h"
#include "signal_preproc.h"
#include "cepstral_proc.h"
#include "VAD.h"
#include <fstream>

using namespace std;

/**
* Komentar k hlavni fci aby bylo poznat ze se neco deje.
*/
int main()
{
    //cout << "Run" <<endl;
    {//hide some scrap
/*
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
    }
    //end of scrap

   //vector<double> rsig = record_signal();
   vector<double> rsig = load_signal("/Users/jirifiala/Documents/C++/CPP/DTW_matrix_computation/DTW_matrix_computation/SA176S01.CS0");
    vector<vector<double> > mfcc = make_mfcc(rsig, 25, 10, 16000, 24, 0, 8000, 13);
    save_matrix(mfcc);
    double perc = 20;
    double Dynmin = 50;
    double qmax1 = 0.8;
    double qmax2 = 0.995;
    double qmin1 = 0.8;
    double qmin2 = 0.9999;
    VAD(mfcc,perc,Dynmin,qmax1,qmax2,qmin1,qmin2);
    cout << "done" << endl;
    
    //vector<vector<double> > mfcc = make_mfcc(rsig, 25, 10, 16000, 24, 0, 8000, 13);
    
    //print_vector2(mfcc);

 //   for(int i=0 ;i<rsig.size();i++){
 //       cout << [i] << " ";
 //   }
 //   cout << endl;
/*
    vector<vector<double> > sigs = {load_signal("jedna.raw"),load_signal("dva.raw"),load_signal("tri.raw"),load_signal("ctyry.raw")};
    vector<double> pattern = load_signal("jedna_test.raw");
    for(int i = 0;i<sigs.size();i++){
        cout << DTW_dist(make_mfcc(sigs[i], 25, 10, 16000, 24, 0, 8000, 13),make_mfcc(pattern, 25, 10, 16000, 24, 0, 8000, 13),cepstral_dist_c0) << endl;
    }//*/

//    make_mfcc(load_signal("audio.raw"), 25, 10, 16000, 24, 0, 8000, 13);

    //cout << "Done" << endl;
    return 0;
}



