#ifndef SIGNAL_PREPROC_INCLUDED
#define SIGNAL_PREPROC_INCLUDED

#include <iomanip>
#include <fstream>
#include <vector>
#include "portaudio.h"

using namespace std;

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
PaStreamCallbackFlags statusFlags,
void *userData )
{

}

/**
* Tahle funkce dela tohle. OMG OMG OMG
*/
vector<double> record_signal()
{
    cout << "Init PA" << endl;
    cout << Pa_GetErrorText(Pa_Initialize()) << endl ;

    cout << "Open PA Stream" << endl;
    PaStream *stream;
    cout << Pa_GetErrorText(Pa_OpenDefaultStream(&stream,1,0,paInt16,16000,160000,callbackFce,)) << endl ;




}

#endif // SIGNAL_PREPROC_INCLUDED
