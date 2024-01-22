#ifndef STRUCTS
#define STRUCTS

#include <raylib.h>
#include "structs.h"

#define samples 4096

// signals for windows
typedef struct{
	int message_status;
	int new_status;
	int info_status;
	int import_message;
	bool save;
	bool loop_hole;
	bool control;
	bool error_progress;
}Signals;

// radar properities
typedef struct{
	float x;
	float y;
	float z;
	float distance;
	float azymuth;
	float elevation;
	int combo;
	float lambda;
	char lambda_fix[8];
	float freq;
	int freq_fix;
}Radar;

// camera view properities
typedef struct{
	float x;
	float y;
	float z;
	float distance;
	float azymuth;
	float elevation;
	int combo;
}Cam;

typedef struct{
	float panel_width;
	float btn_width;
	float btn_height;
	int pad_y; 
	float group_width;
	float slider_width;
}Geometry;

typedef struct{
	bool gizmo;
	bool box;
	bool model;
	bool radar;
	bool grid;
	bool fps;
	bool plot;
	bool wave;
	bool progress;
	bool error_progress;
}VIS;

typedef struct{
	float x;
	float y;
	float z;
	float cylinder_d;
	float cylinder_h;
	float cone_d;
	float cone_h;
	int segments;
	Vector3 start;
	Vector3 endRed;
	Vector3 endGreen;
	Vector3 endBlue;
}Gizmo;

typedef struct{
	double x[samples];
	double y[samples];
}point;

typedef struct{
	point point;
	double x_max;
	double x_min;
	double y_max;
	double y_min;
	int iter;
}DATA;

typedef struct{
	float x;
	float y;
	float z;
	float p_x;
	float p_y;
	float ratio;
}bounding;

#endif

