#include <iostream>
#include <vector>
#include <math.h>
#include "DTW.h"

using namespace std;



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

    cout << DTW_dist(test4,test3) << endl;

    print_vector2(make_mfcc("audio.raw", 25, 10, 16000, 24, 0, 8000, 13));

    cout << "Done" << endl;
    return 0;
}



