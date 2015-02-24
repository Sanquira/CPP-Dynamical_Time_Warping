#include <iostream>
#include <vector>
#include <math.h>
//#include "dtw.h"

using namespace std;

double cepstral_dist_c1(vector<double> frst, vector<double> scnd){
    double csm=0;
    for (int i=0;i<frst.size();i++){
        csm+=pow(frst[i]-scnd[i],2);
    }
    return 4.3429*sqrt(2*csm);
}

double cepstral_dist_c0(vector<double> frst, vector<double> scnd){
    double csm=0;
    for (int i=1;i<frst.size();i++){
        csm+=pow(frst[i]-scnd[i],2);
    }
    return 4.3429*sqrt(2*csm + pow(frst[0]-scnd[0],2));
}

double minimum(double num1, double num2, double num3){
    return std::min(min(num1,num2),num3);
}

double DTW_dist(vector< vector<double> > pattern, vector< vector<double> > test){
    vector<double> dist0 = {0};
    for(int i=0;i<pattern.size();i++){
        dist0.push_back(dist0[i]+cepstral_dist_c0(test[0],pattern[i]));
    }
    dist0[0] = dist0[1];

    vector<double> dist1 = {0};
    for(int i=0;i<test.size();i++){
        dist1.push_back(dist1[i]+cepstral_dist_c0(test[i],pattern[0]));
    }
    dist1.erase(dist1.begin());

    for(int j = 1;j<test.size();j++){           // row
        for(int i=1;i<pattern.size()-1;i++){    // collumn
            double cost = cepstral_dist_c0(test[j],pattern[i]);
            if(i==1){
                dist0[i-1]=minimum(dist0[i],dist0[i+1],dist1[j])+cost;
            } else {
                dist0[i-1]=minimum(dist0[i],dist0[i+1],dist0[i-2])+cost;
            }
        }
        dist0.insert(dist0.begin(),dist0[0]);
        dist0.pop_back();
    }

    cout << dist0[0]-dist1[0] << endl;
return 0;
}

int main()
{
    cout << "Run" <<endl;



///*
    vector< vector<double> > test3;
    vector< vector<double> > test4;

    for(int i = 0;i<255;i++){
        vector<double> test1;
        for(int j=0;j<13;j++){
            test1.push_back((i*j)^3);
        }
        test3.push_back(test1);
    }

    for(int i = 0;i<200;i++){
        vector<double> test1;
        for(int j=0;j<13;j++){
            test1.push_back((i*j)^6);
        }
        test4.push_back(test1);
    }//*/

    DTW_dist(test3,test3);

    cout << "Done" << endl;
    return 0;
}



