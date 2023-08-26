#include <stdbool.h>
#include <raylib.h>
#include <stdio.h>
#include <math.h>

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

// pre-set camera position
void Camera_PreSet(Camera3D *camera)
{
	camera -> position = (Vector3){0.0f, 0.0f, 10.0f};
	camera -> target = (Vector3){0.0f, 0.0f, 0.0f};	
	camera -> up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera -> fovy = 0.0f;
	camera -> projection = CAMERA_PERSPECTIVE;
}
