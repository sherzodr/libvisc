# libvisc
Implements Ubbelohde-Walther equation (ASTM D341) and  ASTM D2270 (Viscosity Index) implementation in C++

# INSTALL


    ./configure
    make
    make install

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


For more information look into libvisc.h as well as example.cpp, which generates
a sample data. To compile this file, after *./configure* run *make example*.
Then run generated executable *example* (or *example.exe*)


        Label			visc40	visc100	visc60	vi	m	vtc
        ========================================================================
        Naphthenic spindle oil	30	4.24	13.1064	-25	4.24669	0.858667
        Paraffinic spindle oil	30	5.23	14.525	104	3.67225	0.825667
        Medium solvent extract	120	8	    36.9947	-49	4.47134	0.933333
        Medium polyglycol     	120	20.9	59.5139	200	2.52709	0.825833
        Medium silicone oil   	120	50	    86.3819	423	1.14093	0.583333
        Multigrade motor oil  	70	11.1	32.9198	150	3.08679	0.841429
        Ester oil             	30	5.81	15.2728	139	3.39742	0.806333
        Tribology-ABC         	68	8.6	    28.6455	96	3.60799	0.873529
        ========================================================================

# TODO

Vogel-Cameron equation

