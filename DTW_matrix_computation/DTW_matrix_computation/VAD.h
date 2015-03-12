#ifndef VAD_H_INCLUDED
#define VAD_H_INCLUDED

#include <vector>
using namespace std;

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

#endif // VAD_H_INCLUDED
