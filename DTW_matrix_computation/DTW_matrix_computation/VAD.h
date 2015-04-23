#ifndef VAD_H_INCLUDED
#define VAD_H_INCLUDED

#include <vector>
using namespace std;

//
double power(vector<double> input){
    double cumsum=0;
    for(int i=0;i<input.size();i++){
        cumsum=cumsum+(input[i]*input[i]);
    }
    return cumsum/input.size();
}

bool VAD_energy(vector<double> input,double treshold)
{
	return (power(input))>=treshold?true:false;
}

void VAD(vector<vector<double> > mfcc){
    
    vector<double> out;
    
    //determine the mffc of the background
    //compute the mean value of the first n frames
    int n = 5;
    double sum = 0;
    double mean = 0;
    vector<double> background (mfcc[0].size());
    
    for (int j = 0; j<mfcc[1].size();j++) {
        sum = 0;
        mean = 0;
        for (int i = 0; i<n; i++) {
            
            sum = sum + mfcc[i][j];
            mean = sum/n;
           
        }
        background[j] = mean;
        
    }
    
 
    //compute the matrix of distances
    vector<double> distances (mfcc.size());
    for (int i = 0; i < mfcc.size(); i++) {
        distances[i] = cepstral_dist_c2(background, mfcc[i]);
    }
    
    
}

#endif // VAD_H_INCLUDED
