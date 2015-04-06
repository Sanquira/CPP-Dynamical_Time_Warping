#ifndef SIGNAL_PREPROC_INCLUDED
#define SIGNAL_PREPROC_INCLUDED

#include <iomanip>
#include <fstream>
#include <vector>
#include "portaudio.h"
#include <float.h>

using namespace std;

vector<double> recSig;

vector<double> load_signal(string name)
{
    vector<double> sig;
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
	return sig;
}


static int callbackFce( const void *inputBuffer, void *outputBuffer,
unsigned long framesPerBuffer,
const PaStreamCallbackTimeInfo* timeInfo,
PaStreamCallbackFlags statusFlags,void *userdata)
{

    short* pData = (short*) inputBuffer;

    for(int i = 0;i<framesPerBuffer;i++){
        recSig.push_back((*pData++)/32768.0);
    }

    return paContinue;

}

/**
* Tahle funkce dela tohle. OMG OMG OMG
*/
vector<double> record_signal()
{
    //cout << "Init PA" << endl;
    //cout << Pa_GetErrorText(Pa_Initialize()) << endl ;
    Pa_Initialize();
    //cout << "Open PA Stream" << endl;
    /*
    PaStreamParameters  inputParameters;
    inputParameters.channelCount=1;
    inputParameters.device=Pa_GetDefaultInputDevice();
    inputParameters.sampleFormat=paInt16;
    inputParameters.suggestedLatency=Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;//*/

    PaStream* stream;

    Pa_OpenDefaultStream(&stream,1,0,paInt16,16000,512,callbackFce,NULL);
    Pa_StartStream(stream);

    int delayCntr = 0;
    while( delayCntr++ < 2 )
    {
        Pa_Sleep(1000);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return recSig;

}

#endif // SIGNAL_PREPROC_INCLUDED
