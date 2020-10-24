/*
 * example.cpp
 *
 *  Created on: Oct 25, 2020
 *      Author: sherzodr
 */


#include <iostream>
#include "libvisc.h"

int main() {

    using namespace std;

    struct sampleLube {
        string name;
        int visc40;
        float visc100;
    };

    sampleLube lubeList[] = {
        {"Naphthenic spindle oil", 30, 4.24 },
        {"Paraffinic spindle oil", 30, 5.23 },
        {"Medium solvent extract", 120, 8.0 },
        {"Medium polyglycol     ", 120, 20.9},
        {"Medium silicone oil   ", 120, 50.0},
        {"Multigrade motor oil  ", 70, 11.1 },
        {"Ester oil             ", 30, 5.81 },
        {"Tribology-ABC         ", 68, 8.6  }
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

        cout << lube.label() <<
            "\t" << lube.visc(40) <<
            "\t" << lube.visc(100) <<
            "\t" << lube.visc(60) <<
            "\t" << lube.vi() <<
            "\t" << lube.m() <<
            "\t" << lube.vtc() << endl;
    }
    cout << "========================================================================\n";

    return 0;
}


