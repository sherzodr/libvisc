#include "libvisc.h"

// Constructors

Lubricant::Lubricant() {
	this->__label = "Unnamed Lubricant";
}

Lubricant::Lubricant(std::string label) {
	this->__label = label;
	this->__vi = 0;
}

Lubricant::Lubricant(float visc40, float visc100) {
	this->calibration_points[40] = visc40;
	this->calibration_points[100] = visc100;
	this->__label = "Unnamed Lubricant";
	this->__vi = 0;
}

Lubricant::Lubricant(std::string label, float visc40, float visc100) {
	this->__label = label;
	this->calibration_points[40] = visc40;
	this->calibration_points[100] = visc100;
	this->__vi = 0;
}

void Lubricant::label(std::string arg) {
	this->__label = arg;
}

std::string Lubricant::label(void) {
	return this->__label;
}

void Lubricant::density(float density) {
	this->__density = density;
}

float Lubricant::density(void) {
	return this->__density;
}

float Lubricant::vtc(void) {
    std::map<short,float> cal = this->calibration_points;

    if( ( cal.find(40) == cal.end() ) || ( cal.find(100) == cal.end()) ) return 0.8;
	return ((this->visc(40) - this->visc(100)) / this->visc(40));
}

float Lubricant::visc(short T) {
    if (this->calibration_points.find(T) != this->calibration_points.end()) return this->calibration_points[T];

    // visc could not be found. We check to see if we have
    // at least two calibration points to see if we can generate
    // requested viscosity at temperature T
    if (this->calibration_points.size() < 2) {
        throw std::runtime_error("Not enough calibration points");
    }

    // if we come this far, we have at least two calibration points
    // and we can attempt to generate the viscosity at the requested
    // temperature (T) using Ubbelohde-Walter equation

    //float c2_temp = __c2k(this->calibration_points.rbegin()->first);
    //float c2_visc = this->calibration_points.rbegin()->second;

    float vtc = this->vtc();
    float m = this->m();

    //float a = log10(log10(c2_visc + vtc)) + (m * log10(c2_temp));
    float a = this->a();

    float c = a * 100;
    float d = m * 25;
    float visc_T = pow(exp(1),
            (log(10)
             * pow(exp(1),
                 (((c * log(10)) / 100)
                  - ((d * log(__c2k(T))) / 25))))) - vtc;
    // we have to store the result as the new calibration point, so that the next call
    // with the same argument gets intercepted.
    this->visc(T, visc_T);
    return visc_T;
}

float Lubricant::m() {
	if (this->calibration_points.size() < 2) {
		throw std::runtime_error(
				"m(): at least two calibraiton points must be present");
	}

	float c1_temp = __c2k(this->calibration_points.begin()->first);
	float c1_visc = this->calibration_points.begin()->second;

	float c2_temp = __c2k(this->calibration_points.rbegin()->first);
	float c2_visc = this->calibration_points.rbegin()->second;

	using namespace std;

	float vtc = this->vtc();

	assert(c1_temp && c1_visc && c2_temp && c2_visc);

	return (log(log(c1_visc + vtc)) - log(log(c2_visc + vtc)))
			/ (log(c2_temp) - log(c1_temp));
}


float Lubricant::a(void) {
    std::map<short,float>::iterator last_calpoint = this->calibration_points.begin();

    float c2_temp = __c2k(last_calpoint->first);
    float c2_visc = last_calpoint->second;

    float vtc = this->vtc();
    float m = this->m();

    float a = log10(log10(c2_visc + vtc)) + (m * log10(c2_temp));

    return a;
}

void Lubricant::visc(short T, float v) {
	this->calibration_points[T] = v;
}

float Lubricant::__c2k(float c) {
	return (c + 273.15);
}

float* Lubricant::LH(void) {

	float visc100;

	visc100 = this->visc(100);

	if (!visc100) {
		throw std::runtime_error("LH(): Cannot calculate viscosity @ 100C, which is needed to lookup L&H coefficients");
	}

	if (visc100 < 2) {
		throw std::runtime_error("LH(): L&H are not defined for lubricants below 2 cst @ 100C");
	}

	if (visc100 > 70) {
		throw std::runtime_error("LH(): current it doesnt' support vi calculation for this product");
	}

	float *one_before, *one_after;
	short array_size = ( sizeof(table_astmd2270) / sizeof(table_astmd2270[0]) );
	bool one_before_set = false;
	bool one_after_set = false;
	for(short i = 0; i < array_size; i++) {
		float *row = table_astmd2270[i];
		if (row[0] == visc100) return row; 		// if we find the value we're looking for we return the row
		if (row[0] < visc100) {
			one_before = row;
			one_before_set = true;
			continue;
		}
		if (row[0] > visc100) {
			one_after = row;
			one_after_set = true;
			break;
		}
	}

	if (( one_before_set == false ) || ( one_after_set == false )) {
		throw std::runtime_error("No rows found to interoplate");
	}

	float visc1, visc2, visc_delta, L1, L2, L_delta;
	float H1, H2, H_delta;
	float L1_per_unit, H1_per_unit, L, H, *RV;

	visc1 = one_before[0];
	visc2 = one_after[0];
	visc_delta = visc2 - visc1;

	L1 = one_before[1];
	L2 = one_after[1];
	L_delta = L2 - L1;

	H1 = one_before[2];
	H2 = one_after[2];
	H_delta = H2 - H1;

	L1_per_unit = L_delta / visc_delta;
	H1_per_unit = H_delta / visc_delta;

	L = L1 + ((visc100 - visc1) * L1_per_unit);
	H = H1 + ((visc100 - visc1) * H1_per_unit);

	RV = new float[3];
	RV[0] = visc100;
	RV[1] = L;
	RV[2] = H;

	return RV;
}

int Lubricant::vi(void) {

	if (this->__vi != 0) return this->__vi;
	int vi = this->__vi_lt_100();
	if (vi > 100) {
		vi = this->__vi_gt_100();
	}
	this->__vi= vi;
	return this->__vi;
}


int Lubricant::__vi_lt_100(void) {
	float visc40 = this->visc(40);
	float* LH = this->LH();
	float L = LH[1];
	float H = LH[2];

	return ( (L - visc40) / (L-H) ) *100;
}


int Lubricant::__vi_gt_100(void) {
	float *LH = this->LH();
	float H = LH[2];
	float visc40 = this->visc(40);
	float visc100 = this->visc(100);

	float N = ( log10(H) - log10(visc40) ) / log10(visc100);
	return (( pow(10, N) - 1 ) / 0.00715 ) + 100;
}


void Lubricant::vi(int new_vi) {
	this->__vi = new_vi;
}

