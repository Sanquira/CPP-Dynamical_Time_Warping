#ifndef SIGNAL_PREPROC_INCLUDED
#define SIGNAL_PREPROC_INCLUDED

#include <iomanip>
#include <fstream>
#include <vector>
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

#endif // SIGNAL_PREPROC_INCLUDED
