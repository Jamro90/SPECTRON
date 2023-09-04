#ifndef CALC
#define CALC

#include "calc.h"

void Cartesian2Polar(float *x, float *y, float *z, float *d, float *b, float *e);

void Polar2Cartesian(float *x, float *y, float *z, float *d, float *b, float *e);

void Camera_PreSet(Camera3D *camera);

//float freq2wave(float *freq, char *fix_in, char *fix_out);

//float wave2freq(float *wave_length, char *fix_in, char *fix_out);

#endif
