# libvisc
Implements Ubbelohde-Walther equation (ASTM D341) and  ASTM D2270 (Viscosity Index) implementation in C++

# Description

libvisc.h describes interface of the  **Lubricant** class.

# Example

    #include "libvisc.h"
    #include <iostream>

    int main() {

        using namespace std;
        
        /* Initializing the lubricant with label, viscosity @ 40 and viscosity at 100 */
        Lubricant lube("Paraffinic spindle oil", 30, 5.23);

        cout << "vi = " << lube.vi() << endl;
        cout << "Viscosity @ 50C is " << lube.visc(50) << endl;
        cout << "Viscosity @ 0C is " << lube.visc(0) << endl;
        cout << "V-T constant is " << lube.vtc() << endl;
        cout << "m constant is " << lube.m() << endl;

        return 0;
    }


For more information look into libvisc.h as well as main.cpp, which generates sample data.

Once you download it, just cd into the libvisc/ and type *make*. It will generate a **main** executable.

# TODO

Vogel-Cameron equation

