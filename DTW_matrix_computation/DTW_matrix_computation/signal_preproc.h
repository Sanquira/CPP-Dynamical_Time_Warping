#ifndef SIGNAL_PREPROC_INCLUDED
#define SIGNAL_PREPROC_INCLUDED

#include <iomanip>
#include <fstream>
#include <vector>
#include "portaudio.h"

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

    short** pData = (short**) inputBuffer;

    for(long int i = 0;i<framesPerBuffer;i++){
        recSig.push_back(pData[0][i]);
    }

    return paContinue;

}

/**
* Tahle funkce dela tohle. OMG OMG OMG
*/
vector<double> record_signal()
{
    cout << "Init PA" << endl;
    cout << Pa_GetErrorText(Pa_Initialize()) << endl ;

    cout << "Open PA Stream" << endl;
    /*
    PaStreamParameters  inputParameters;
    inputParameters.channelCount=1;
    inputParameters.device=Pa_GetDefaultInputDevice();
    inputParameters.sampleFormat=paInt16;
    inputParameters.suggestedLatency=Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;//*/

    PaStream* stream;

    cout << Pa_GetDefaultInputDevice() << endl;
    cout << Pa_GetErrorText(Pa_OpenDefaultStream(&stream,1,0,paInt16,16000,512,callbackFce,NULL)) << endl;

    Pa_StartStream(stream);
    cout << "Recording" << endl;

    int delayCntr = 0;
    while( delayCntr++ < 1 )
    {
        Pa_Sleep(1000);
    }

    Pa_CloseStream(stream);
    Pa_Terminate();

    cout << "Recorded" << endl;

    cout << recSig.size() << endl;




}

#endif // SIGNAL_PREPROC_INCLUDED
