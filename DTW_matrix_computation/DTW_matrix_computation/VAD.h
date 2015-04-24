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

vector<bool> VAD(vector<vector<double> > mfcc,double perc,double Dynmin,double qmax1,double qmax2,double qmin1,double qmin2){
    
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
    
 
    //compute the vector of distances
    vector<double> Dv (mfcc.size());
    for (int i = 0; i < mfcc.size(); i++) {
        Dv[i] = cepstral_dist_c2(background, mfcc[i]);
    }
    
    //set parameters
    
    
    //vector allocation
    vector<bool> output (mfcc.size());
    vector<double> Dpv (mfcc.size());
    vector<double> Dmaxv (mfcc.size());
    vector<double> Dminv (mfcc.size());

    //find max of the first two elements
    double max;
    (Dv[0])>=Dv[1]?max=Dv[0]:max=Dv[1];
   
    
    //find min of the first two elements
    double min;
    (Dv[0])>=Dv[1]?min=Dv[1]:min=Dv[0];
   
    
    
    
    
    double Dmax = max+Dynmin/10;
    double Dmin = min-Dynmin/10;
    double Dyn = Dmax - Dmin;

    double D,Dp;
    for (int i = 0; i < mfcc.size(); i++) {
        
        D = Dv[i];
        
        if (D > Dmax) {
            Dmax = qmax1*Dmax + (1-qmax1)*D ;
        } else {
            Dmax = qmax2*Dmax + (1-qmax2)*D ;
        }
        
        if ( D < Dmin ){
            Dmin = qmin1*Dmin + (1-qmin1)*D ;
        }else{
            Dmin = qmin2*Dmin + (1-qmin2)*D ;
        }
        
        Dyn=Dmax-Dmin;
        
        Dp=Dmin+perc/100*(Dmax-Dmin);
        Dpv[i] = Dp;
        Dmaxv[i]=Dmax;
        Dminv[i]=Dmin;
        
        if (( D>Dp) && (Dyn > Dynmin)){
            output[i]=true;
        }else{
            output[i]=false;
        }
        
        
    }
    
    return output;
    
}

#endif // VAD_H_INCLUDED
