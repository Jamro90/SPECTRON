#include <stdbool.h>

#include "raylib_binaries/raylib/src/raylib.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

// Cartesian -> Polar 
void Cartesian2Polar(float *x, float *y, float *z, float *d, float *b, float *e)
{
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
void Polar2Cartesian(float *x, float *y, float *z, float *d, float *b, float *e)
{
	*x = (*d) * cos((*e) * PI/180) * cos((*b) * PI/180);
	*y = (*d) * cos((*e) * PI/180) * sin((*b) * PI/180);
	*z = (*d) * sin((*e) * PI/180);
}
/*
// frequency -> wave length
float freq2wave(float *freq, char *fix_in, char *fix_out) 
{	
	float fr = (*freq);
	if(!strcmp(fix_in, "THz")) (fr) * 1e12;
	else if(!strcmp(fix_in, "GHz")) (fr) * 1e9;
	else if(!strcmp(fix_in, "MHz")) (fr) * 1e6;
	else if(!strcmp(fix_in, "kHz")) (fr) * 1e3;

	float wave_length = 3000000/(fr);

	if(wave_length > 1e3)
	{
		strcpy(fix_out, "km"); 
		wave_length *= 1e-3;
	}
	else if(wave_length > 1) strcpy(fix_out, "m");
	else if(wave_length > 1e-3) 
	{
		strcpy(fix_out, "mm");
		wave_length *= 1e3;
	}
	else if(wave_length > 1e-6) 
	{
		strcpy(fix_out, "um");
		wave_length *= 1e6;
	}
	else 
	{
		strcpy(fix_out, "nm");
		wave_length *= 1e9;
	}

	return wave_length;
}

// wave length -> frequency
float wave2freq(float *wave_length, char *fix_in, char *fix_out)
{
	float wave = (*wave_length);
	if(!strcmp(fix_in, "km")) wave *= 1e3;
	// if for meters is pointless
	else if(!strcmp(fix_in, "mm")) wave *= 1e-3;
	else if(!strcmp(fix_in, "um")) wave *= 1e-6;
	else if(!strcmp(fix_in, "nm")) wave *= 1e-9;

	float freq = 3000000/(wave);
	
	if(freq > 1e12) 
	{
		strcpy(fix_out, "THz");
		freq *= 1e-12;
	}
	else if(freq > 1e9)
	{
		strcpy(fix_out, "GHz");
		freq *= 1e-9;
	}
	else if(freq > 1e6)
	{
		strcpy(fix_out, "MHz");
		freq *= 1e-6;
	}
	else if(freq > 1e3)
	{
		strcpy(fix_out, "kHz");
		freq *= 1e-3;
	}
	else strcpy(fix_out, "Hz");

	return freq; 
}
*/
// pre-set camera position
void Camera_PreSet(Camera3D *camera)
{
	camera -> position = (Vector3){0.0f, 0.0f, 10.0f};
	camera -> target = (Vector3){0.0f, 0.0f, 0.0f};	
	camera -> up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera -> fovy = 0.0f;
	camera -> projection = CAMERA_PERSPECTIVE;
}
