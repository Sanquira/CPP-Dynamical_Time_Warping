#ifndef DTW_INCLUDED
#define DTW_INCLUDED

#include <vector>
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
    dist0.erase(dist0.begin());

    double cd1old, cd1new = cepstral_dist_c0(test[0],pattern[0]);

    for(int j = 1;j<test.size();j++){           // row
        cd1old = cd1new;
        cd1new = cd1old + cepstral_dist_c0(test[j],pattern[0]);
        for(int i=1;i<pattern.size();i++){    // collumn
            double cost = cepstral_dist_c0(test[j],pattern[i]);
            if(i==1){
                dist0[i-1]=minimum(dist0[i],cd1old,cd1new)+cost;
            } else {
                dist0[i-1]=minimum(dist0[i-1],dist0[i],dist0[i-2])+cost;
            }
        }
        dist0.insert(dist0.begin(),0);
        dist0.pop_back();
    }

return dist0.back();
}

#endif // DTW_INCLUDED
