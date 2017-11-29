#include <iostream>
#include "libvisc.h"

/*
 *
 *
 *
 */

int main() {

	using namespace std;

	struct sampleLube {
		string name;
		int visc40;
		float visc100;
		int vi;
		float m;
		float vtc;
	};

	sampleLube lubeList[] = {
			{"Naphthenic spindle oil", 30, 4.24, 40, 4.05, 0.847},
			{"Paraffinic spindle oil", 30, 5.23, 105, 3.68, 0.819},
			{"Medium solvent extract", 120, 8.0, -50, 4.51, 0.939},
			{"Medium polyglycol     ", 120, 20.9, 200, 2.53, 0.826},
			{"Medium silicone oil   ", 120, 50.0, 424, 1.14, 0.583},
			{"Multigrade motor oil  ", 70, 11.1, 165, 2.82, 0.841},
			{"Ester oil             ", 30, 5.81, 140, 3.40, 0.806},
            {"Tribology-ABC         ", 68, 8.6,  0, 0, 0}
	};

	int list_size = sizeof(lubeList) / sizeof(lubeList[0]);

	cout << "size of the array is " << list_size << endl;

	for(int i = 0; i < list_size; i++ ) {
		if (i == 0) {
			cout << "Label\t\t\tvisc40\tvisc100\tvisc60\tvi\tm\tvtc\n";
			cout << "========================================================================\n";
		}
		sampleLube row = lubeList[i]; // saving the first row

		Lubricant lube(row.name, row.visc40, row.visc100); // initializing Lubricant instance with three parameters;

		cout << lube.label() << "\t" << lube.visc(40) << "\t" << lube.visc(100) <<"\t" << lube.visc(60) << "\t" << lube.vi() << "\t" << lube.m() << "\t" << lube.vtc() << endl;
	}
    cout << "========================================================================\n";

	return 0;
}
