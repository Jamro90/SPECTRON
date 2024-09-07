#include "calc.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Cartesian -> Polar 
void Cartesian2Polar(float *x, float *y, float *z, float *d, float *b, float *e){
	*d = sqrt( (*x) * (*x) + (*y) * (*y) + (*z) * (*z) );

	if(((*x) > 0) && ((*y) == 0)) *b = 0;
	else if(((*x) == 0) && ((*y) == 0)) *b = 0;
	else if(((*x) == 0) && ((*y) > 0)) *b = 90;
	else if(((*x) < 0) && ((*y) == 0)) *b = 180;
	else if(((*x) == 0) && ((*y) < 0)) *b = 270;
	else if(((*x) > 0) && ((*y) > 0)) *b = atan( ((*y) / (*x)) ) * (180/PI);
	else if(((*x) < 0) && ((*y) > 0)) *b = atan( (-(*x) / (*y)) ) * (180/PI) + 90;
	else if(((*x) < 0) && ((*y) < 0)) *b = atan( (-(*y) / -(*x)) ) * (180/PI) + 180;
	else if(((*x) > 0) && ((*y) < 0)) *b = atan( ((*x) / -(*y)) ) * (180/PI) + 270;

	if((*z) == 0) *e = 0;
	else *e = atan( (*z) / sqrt( (*x) * (*x) + (*y) * (*y) ) ) * (180/PI);

}

// Polar -> Cartesian
void Polar2Cartesian(float *x, float *y, float *z, float *d, float *b, float *e){
	*x = (*d) * cos((*e) * PI/180) * cos((*b) * PI/180);
	*y = (*d) * cos((*e) * PI/180) * sin((*b) * PI/180);
	*z = (*d) * sin((*e) * PI/180);
}

// frequency -> wave length
float freq2wave(Radar *radar){	
	float fr = (radar->freq);
	if((radar->freq_fix) == 3) fr *= 1e12;
	else if((radar->freq_fix) == 2) fr *= 1e9;
	else if((radar->freq_fix) == 1) fr *= 1e6;

	radar->lambda = 3000000/fr;

	if(radar->lambda >= 1) sprintf(radar->lambda_fix, "m");
	else if((radar->lambda >= 1e-3) && (radar->lambda <= 1)){
		sprintf(radar->lambda_fix, "mm");
		radar->lambda *= 1e3;
	}
	else if((radar->lambda >= 1e-6) && (radar->lambda <= 1e-3)){
		sprintf(radar->lambda_fix, "um");
		radar->lambda *= 1e6;
	}

	return radar->lambda;
}

// wave length -> frequency
float wave2freq(Radar *radar){
	float wave = radar->lambda;
	// if for meters is pointless
	if(!strcmp(radar->lambda_fix, "um")) wave *= 1e-6;
	else if(!strcmp(radar->lambda_fix, "mm")) wave *= 1e-3;

	float freq = 3000000/(wave);
	
	if(freq >= 1e12){
		radar->freq_fix = 3;
		freq *= 1e-12;
	}
	else if((freq >= 1e9) && (freq < 1e12)){
		radar->freq_fix = 2;
		freq *= 1e-9;
	}
	else if((freq >= 1e6) && (freq < 1e9)){
		radar->freq_fix = 1;
		freq *= 1e-6;
	}

	return freq; 
}
