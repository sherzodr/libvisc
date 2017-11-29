/*
 * libvisc.h
 * Purpose: implements lubricant class
 *
 * @author Sherzod Ruzmetov
 * @version 1.0
 */

#ifndef __H_LUBRICANT
#define __H_LUBRICANT

#include <string>
#include <map>
#include <stdexcept>
#include <math.h>
#include <assert.h>

class Lubricant {

public:
/*
 * Constructors
 * There are more than one way of creating constructors
 * 	Lubricant lube;
 * 	Lubricant lube("Paraffinic spindle oil");
 * 	Lubricant lube("Paraffinic spindle oil", visc40, visc100);
 * 	Lubricant lube(visc40, visc100)
 *
 * 	Since most Lubricant's Technical Data Sheets provide viscosity points at 40 and 100C constructor has accept these values during
 * 	initilization for convenience purposes only.
 *
 */
	Lubricant();
	Lubricant(float, float);
	Lubricant(std::string);
	Lubricant(std::string, float, float);

	/*
	 * Returns label of the current instance:
	 *
	 */

	std::string label(void);

	/*
	 * Returns density of the lubricant, if set
	 */
	float density(void);

	/*
	 * float Lubricant::visc(short T)
	 *
	 * Calculates viscosity of the lubricant at temperature T (in Celcius). If viscosity is already known simply returns
	 * from its internal stack of precalculated calculation points. If this value does not exist, and at least two calibration
	 * points are already known it will calculate this viscosity in cst using Ubbelohde-Walter equation.
	 *
	 * Once the value is calculated, this point is stored in the private stack as a calibration point, so that when it
	 * is requested next time no calculation takes place.
	 */
	float visc(short);


	/*
	 * float Lubricant::vtc(void)
	 *
	 * Variable-Temperature-Constant. Used to accurately predict temperature-sensitivity of the lubricant in Ubbelohde-Walter equation.
	 * If VTC cannot be calculated it defaults to 0.8. For it to be able to calculate VTC calibrations at 40 and 100C must be set!
	 *
	 */
	float vtc(void);

	/*
	 *
	 * V-T coefficient of the lubricant. Used in Ubbelohde-Walter equation.
	 */
	float m(void);

	/*
	 * Experimental. Intermittant value used in Ubbelohde-Walter equation calculations. I needed to access this value during testing
	 *
	 */
	float a(void);



	/*
	 * Returns viscosity index of the lubricant, given viscosities of the lubricant can be calculated at 40 and 100C. It uses
	 * ASTM D2270 table to lookup the values for L and H constants.
	 *
	 * If value cannot be found at ASTM D2270 table it resorts to interopolation, as suggested the the said manual.
	 *
	 * IMPORTANT: Viscosity Index is defined for all the lubricants with viscosity of at least 2cst at 100C. If viscosity of the lubricant
	 * falls below 2cst at 100C this lubricant does not have Viscosity Index defined. runtime_error exception thrown in such case.
	 */
	int vi();

	// setters

	/*
	 * Sets lubricant's label after lubricant has been initialized
	 *
	 */
	void label(std::string);

	/*
	 * Sets lubricant' density D
	 *
	 */
	void density(float);

	/*
	 * void Lubricant::visc(short T, float V)
	 *
	 * Given temperature T and viscosity V sets lubricant's calibration point. The next time the user asks for this point no calculation
	 * takes place, but known value simply gets returned;
	 *
	 */
	void visc(short, float);


	/*
	 * void Lubricant::vi(int VI)
	 *
	 * Given viscosity index VI sets lubricants viscosity index. The next time when int Lubricant::vi(void) is invoked no calculation takes place and
	 * known value is returned.
	 *
	 */
	void vi(int);

	/*
	 * float* Lubricant::LH(void)
	 *
	 * Used to calculate Viscosity Index of the lubricant (see int Lubricant::vi(void) ) according to ASTM D2270. It returns an array
	 * of three floats, first is the vicosity at 100C, second is L-constant, third is the H constant. These constants are used
	 * by int Lubricant::vi() to calculate the Viscosity Index.
	 *
	 * It uses ASM D2270 L&H table to look up the value based on lublricant's viscosity at 100C. If it fails to look it up it interopalates these
	 * vales using linear conversion, based on one lower and one upper bounds that exists in the said table.
	 *
	 * If lubricant viscosity at 100C is less than 2cst it throws std::runtime_error exception.
	 *
	 */
	float* LH(void);


	/*
	 * std::map<short,float> calibration_points
	 *
	 * This is an instance map, which is accessible publically. It keeps key-value pair of temperature-cst values.
	 */

	std::map<short, float> calibration_points;

private:
	std::string __label;
	float __density;
	float __c2k(float);
	int __vi;
	int __vi_lt_100(void);
	int __vi_gt_100(void);

    float table_astmd2270[311][3] = {
        {2.00, 7.994, 6.394}, {2.10, 8.640, 6.894}, {2.20, 9.309, 7.410}, {2.30, 10.00, 7.944}, {2.40, 10.71, 8.496}, {2.50, 11.45, 9.063}, {2.60, 12.21, 9.647}, {2.70, 13.00, 10.25},
        {2.80, 13.80, 10.87}, {2.90, 14.63, 11.50}, {3.00, 15.49, 12.15}, {3.10, 16.36, 12.82}, {3.20, 17.26, 13.51}, {3.30, 18.18, 14.21}, {3.40, 19.12, 14.93}, {3.50, 20.09, 15.66},
        {3.60, 21.08, 16.42}, {3.70, 22.09, 17.19}, {3.80, 23.13, 17.97}, {3.90, 24.19, 18.77}, {4.00, 25.32, 19.56}, {4.10, 26.50, 20.37}, {4.20, 27.75, 21.21}, {4.30, 29.07, 22.05},
        {4.40, 30.48, 22.92}, {4.50, 31.96, 23.81}, {4.60, 33.52, 24.71}, {4.70, 35.13, 25.63}, {4.80, 36.79, 26.57}, {4.90, 38.50, 27.53}, {5.00, 40.23, 28.49}, {5.10, 41.99, 29.46},
        {5.20, 43.76, 30.43}, {5.30, 45.53, 31.40}, {5.40, 47.31, 32.37}, {5.50, 49.09, 33.34}, {5.60, 50.87, 34.32}, {5.70, 52.64, 35.29}, {5.80, 54.42, 36.26}, {5.90, 56.20, 37.23},
        {6.00, 57.97, 38.19}, {6.10, 59.74, 39.17}, {6.20, 61.52, 40.15}, {6.30, 63.32, 41.13}, {6.40, 65.18, 42.14}, {6.50, 67.12, 43.18}, {6.60, 69.16, 44.24}, {6.70, 71.29, 45.33},
        {6.80, 73.48, 46.44}, {6.90, 75.72, 47.51}, {7.00, 78.00, 48.57}, {7.10, 80.25, 49.61}, {7.20, 82.39, 50.69}, {7.30, 84.53, 51.78}, {7.40, 86.66, 52.88}, {7.50, 88.85, 53.98},
        {7.60, 91.04, 55.09}, {7.70, 93.20, 56.20}, {7.80, 95.43, 57.31}, {7.90, 97.72, 58.45}, {8.00, 100.0, 59.60}, {8.10, 102.3, 60.74}, {8.20, 104.6, 61.89}, {8.30, 106.9, 63.05},
        {8.40, 109.2, 64.18}, {8.50, 111.5, 65.32}, {8.60, 113.9, 66.48}, {8.70, 116.2, 67.64}, {8.80, 118.5, 68.79}, {8.90, 120.9, 69.94}, {9.00, 123.3, 71.10}, {9.10, 125.7, 72.27},
        {9.20, 128.0, 73.42}, {9.30, 130.4, 74.57}, {9.40, 132.8, 75.73}, {9.50, 135.3, 76.91}, {9.60, 137.7, 78.08}, {9.70, 140.1, 79.27}, {9.80, 142.7, 80.46}, {9.90, 145.2, 81.67},
        {10.0, 147.7, 82.87}, {10.1, 150.3, 84.08}, {10.2, 152.9, 85.30}, {10.3, 155.4, 86.51}, {10.4, 158.0, 87.72}, {10.5, 160.6, 88.95}, {10.6, 163.2, 90.19}, {10.7, 165.8, 91.40},
        {10.8, 168.5, 92.65}, {10.9, 171.2, 93.92}, {11.0, 173.9, 95.19}, {11.1, 176.6, 96.45}, {11.2, 179.4, 97.71}, {11.3, 182.1, 98.97}, {11.4, 184.9, 100.2}, {11.5, 187.6, 101.5},
        {11.6, 190.4, 102.8}, {11.7, 193.3, 104.1}, {11.8, 196.2, 105.4}, {11.9, 199.0, 106.7}, {12.0, 201.9, 108.0}, {12.1, 204.8, 109.4}, {12.2, 207.8, 110.7}, {12.3, 210.7, 112.0},
        {12.4, 213.6, 113.3}, {12.5, 216.6, 114.7}, {12.6, 219.6, 116.0}, {12.7, 222.6, 117.4}, {12.8, 225.7, 118.7}, {12.9, 228.8, 120.1}, {13.0, 231.9, 121.5}, {13.1, 235.0, 122.9},
        {13.2, 238.1, 124.2}, {13.3, 241.2, 125.6}, {13.4, 244.3, 127.0}, {13.5, 247.4, 128.4}, {13.6, 250.6, 129.8}, {13.7, 253.8, 131.2}, {13.8, 257.0, 132.6}, {13.9, 260.1, 134.0},
        {14.0, 263.3, 135.4}, {14.1, 266.6, 136.8}, {14.2, 269.8, 138.2}, {14.3, 273.0, 139.6}, {14.4, 276.3, 141.0}, {14.5, 279.6, 142.4}, {14.6, 283.0, 143.9}, {14.7, 286.4, 145.3},
        {14.8, 289.7, 146.8}, {14.9, 293.0, 148.2}, {15.0, 296.5, 149.7}, {15.1, 300.0, 151.2}, {15.2, 303.4, 152.6}, {15.3, 306.9, 154.1}, {15.4, 310.3, 155.6}, {15.5, 313.9, 157.0},
        {15.6, 317.5, 158.6}, {15.7, 321.1, 160.1}, {15.8, 324.6, 161.6}, {15.9, 328.3, 163.1}, {16.0, 331.9, 164.6}, {16.1, 335.5, 166.1}, {16.2, 339.2, 167.7}, {16.3, 342.9, 169.2},
        {16.4, 346.6, 170.7}, {16.5, 350.3, 172.3}, {16.6, 354.1, 173.8}, {16.7, 358.0, 175.4}, {16.8, 361.7, 177.0}, {16.9, 365.6, 178.6}, {17.0, 369.4, 180.2}, {17.1, 373.3, 181.7},
        {17.2, 377.1, 183.3}, {17.3, 381.0, 184.9}, {17.4, 384.9, 186.5}, {17.5, 388.9, 188.1}, {17.6, 392.7, 189.7}, {17.7, 396.7, 191.3}, {17.8, 400.7, 192.9}, {17.9, 404.6, 194.6},
        {18.0, 408.6, 196.2}, {18.1, 412.6, 197.8}, {18.2, 416.7, 199.4}, {18.3, 420.7, 201.0}, {18.4, 424.9, 202.6}, {18.5, 429.0, 204.3}, {18.6, 433.2, 205.9}, {18.7, 437.3, 207.6},
        {18.8, 441.5, 209.3}, {18.9, 445.7, 211.0}, {19.0, 449.9, 212.7}, {19.1, 454.2, 214.4}, {19.2, 458.4, 216.1}, {19.3, 462.7, 217.7}, {19.4, 467.0, 219.4}, {19.5, 471.3, 221.1},
        {19.6, 475.7, 222.8}, {19.7, 479.7, 224.5}, {19.8, 483.9, 226.2}, {19.9, 488.6, 227.7}, {20.0, 493.2, 229.5}, {20.2, 501.5, 233.0}, {20.4, 510.8, 236.4}, {20.6, 519.9, 240.1},
        {20.8, 528.8, 243.5}, {21.0, 538.4, 247.1}, {21.2, 547.5, 250.7}, {21.4, 556.7, 254.2}, {21.6, 566.4, 257.8}, {21.8, 575.6, 261.5}, {22.0, 585.2, 264.9}, {22.2, 595.0, 268.6},
        {22.4, 604.3, 272.3}, {22.6, 614.2, 275.8}, {22.8, 624.1, 279.6}, {23.0, 633.6, 283.3}, {23.2, 643.4, 286.8}, {23.4, 653.8, 290.5}, {23.6, 663.3, 294.4}, {23.8, 673.7, 297.9},
        {24.0, 683.9, 301.8}, {24.2, 694.5, 305.6}, {24.4, 704.2, 309.4}, {24.6, 714.9, 313.0}, {24.8, 725.7, 317.0}, {25.0, 736.5, 320.9}, {25.2, 747.2, 324.9}, {25.4, 758.2, 328.8},
        {25.6, 769.3, 332.7}, {25.8, 779.7, 336.7}, {26.0, 790.4, 340.5}, {26.2, 801.6, 344.4}, {26.4, 812.8, 348.4}, {26.6, 824.1, 352.3}, {26.8, 835.5, 356.4}, {27.0, 847.0, 360.5},
        {27.2, 857.5, 364.6}, {27.4, 869.0, 368.3}, {27.6, 880.6, 372.3}, {27.8, 892.3, 376.4}, {28.0, 904.1, 380.6}, {28.2, 915.8, 384.6}, {28.4, 927.6, 388.8}, {28.6, 938.6, 393.0},
        {28.8, 951.2, 396.6}, {29.0, 963.4, 401.1}, {29.2, 975.4, 405.3}, {29.4, 987.1, 409.5}, {29.6, 998.9, 413.5}, {29.8, 1011, 417.6}, {30.0, 1023, 421.7}, {30.5, 1055, 432.4},
        {31.0, 1086, 443.2}, {31.5, 1119, 454.0}, {32.0, 1151, 464.9}, {32.5, 1184, 475.9}, {33.0, 1217, 487.0}, {33.5, 1251, 498.1}, {34.0, 1286, 509.6}, {34.5, 1321, 521.1},
        {35.0, 1356, 532.5}, {35.5, 1391, 544.0}, {36.0, 1427, 555.6}, {36.5, 1464, 567.1}, {37.0, 1501, 579.3}, {37.5, 1538, 591.3}, {38.0, 1575, 603.1}, {38.5, 1613, 615.0},
        {39.0, 1651, 627.1}, {39.5, 1691, 639.2}, {40.0, 1730, 651.8}, {40.5, 1770, 664.2}, {41.0, 1810, 676.6}, {41.5, 1851, 689.1}, {42.0, 1892, 701.9}, {42.5, 1935, 714.9},
        {43.0, 1978, 728.2}, {43.5, 2021, 741.3}, {44.0, 2064, 754.4}, {44.5, 2108, 767.6}, {45.0, 2152, 780.9}, {45.5, 2197, 794.5}, {46.0, 2243, 808.2}, {46.5, 2288, 821.9},
        {47.0, 2333, 835.5}, {47.5, 2380, 849.2}, {48.0, 2426, 863.0}, {48.5, 2473, 876.9}, {49.0, 2521, 890.9}, {49.5, 2570, 905.3}, {50.0, 2618, 919.6}, {50.5, 2667, 933.6},
        {51.0, 2717, 948.2}, {51.5, 2767, 962.9}, {52.0, 2817, 977.5}, {52.5, 2867, 992.1}, {53.0, 2918, 1007}, {53.5, 2969, 1021}, {54.0, 3020, 1036}, {54.5, 3073, 1051},
        {55.0, 3126, 1066}, {55.5, 3180, 1082}, {56.0, 3233, 1097}, {56.5, 3286, 1112}, {57.0, 3340, 1127}, {57.5, 3396, 1143}, {58.0, 3452, 1159}, {58.5, 3507, 1175},
        {59.0, 3563, 1190}, {59.5, 3619, 1206}, {60.0, 3676, 1222}, {60.5, 3734, 1238}, {61.0, 3792, 1254}, {61.5, 3850, 1270}, {62.0, 3908, 1286}, {62.5, 3966, 1303},
        {63.0, 4026, 1319}, {63.5, 4087, 1336}, {64.0, 4147, 1352}, {64.5, 4207, 1369}, {65.0, 4268, 1386}, {65.5, 4329, 1402}, {66.0, 4392, 1419}, {66.5, 4455, 1436},
        {67.0, 4517, 1454}, {67.5, 4580, 1471}, {68.0, 4645, 1488}, {68.5, 4709, 1506}, {69.0, 4773, 1523}, {69.5, 4839, 1541}, {70.0, 4905, 1558}
    };

};

#endif
