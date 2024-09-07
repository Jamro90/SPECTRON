#ifndef CALC
#define CALC

#include "gui_maker.h"
#include "calc.h"

void Cartesian2Polar(float *x, float *y, float *z, float *d, float *b, float *e);

void Polar2Cartesian(float *x, float *y, float *z, float *d, float *b, float *e);

void Camera_PreSet(Camera3D *camera);

float freq2wave(Radar *radar);

float wave2freq(Radar *radar);

#endif
