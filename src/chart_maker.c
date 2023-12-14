#include "chart_maker.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
//#include "gui_maker.h"
#include <float.h>
#include <raymath.h>

#include "structs.h"

int DataCounter(DATA *data, Model *model, Radar *radar, float *progress)
{

		data->make = false;
		data->y_max = DBL_MIN;
		data->y_min = DBL_MAX;
		uint16_t items = sizeof(data->x) / sizeof(data->x[0]);
		
		float x1 = radar->x;
		float y1 = radar->y;
		float z1 = radar->z;
	for(uint16_t i = 0; i < items; ++i)
	{
		Ray ray_front = {(Vector3){x1, y1, z1}, (Vector3){-x1, -y1, -z1}};	
		Ray ray_back = {(Vector3){-x1, -y1, -z1}, (Vector3){x1, y1, z1}};	
		Reflection(&ray_front, model);
		Reflection(&ray_back, model);
	}
}

void DrawCharts(DATA *data)
{
	int pos_x = 100;
	int pos_y_power = GetScreenHeight()/2;
	int pos_y_polar = GetScreenHeight()/20;
	int flow = 450;
	int radius  = 5;
	int font_size = 30;

	double d = flow/360.0;
	double f = (double) flow/data->y_max * 0.5;
	uint16_t items = sizeof(data->x) / sizeof(data->x[0]);

	data->y[0] = 10 * cos(data->x[0] * PI/180) + 10 * sin(3 * data->x[0] * PI/180);
	if(data->y[0] < data->y_min) data->y_min = data->y[0];
	else if(data->y[0] > data->y_max) data->y_max = data->y[0];

	for(uint16_t i = 1; i < items; ++i)
	{
		data->y[i] = 10 * cos(data->x[i] * PI/180) + 10 * sin(3 * data->x[i] * PI/180);
		if(data->y[i] < data->y_min) data->y_min = data->y[i];
		else if(data->y[i] > data->y_max) data->y_max = data->y[i];


		if( (radius*(int)i <= flow/2) && ((i % 5) == 0) ) DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y_polar}, (i+1)*radius,(i+1)*radius+1, 0, 360, 360, BLACK);

		// POLAR
		DrawLineEx((Vector2){f*data->y[i-1]*cos(data->x[i-1]*PI/180)+pos_x+flow/2,
			 -f*data->y[i-1]*sin(data->x[i-1]*PI/180)+pos_y_polar+flow/2},
			 (Vector2){f*data->y[i]*cos(data->x[i]*PI/180)+pos_x+flow/2,
			 -f*data->y[i]*sin(data->x[i]*PI/180)+pos_y_polar+flow/2}, 2, DARKGREEN);

		// POWER
		DrawLine(d*data->x[i-1]+pos_x, -f*data->y[i-1]+pos_y_power+flow+f*data->y_min, d*data->x[i]+pos_x, -f*data->y[i]+pos_y_power+flow+f*data->y_min, BLUE);

	}

	// POLAR CHART
	// background
	DrawRectangle(pos_x, pos_y_polar, flow, flow, BLANK);
	DrawText("Polar Chart", pos_x+5, pos_y_polar-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y_polar+0.5*flow}, (Vector2){pos_x+flow, pos_y_polar+0.5*flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x+0.5*flow, pos_y_polar}, (Vector2){pos_x+0.5*flow, pos_y_polar+flow}, 2, BLACK);
	
	// POWER CHART
	// background
	DrawRectangle(pos_x, pos_y_power, flow, flow, BLANK);
	DrawText("Power Chart", pos_x+5, pos_y_power-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y_power+flow}, (Vector2){pos_x+flow, pos_y_power+flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x, pos_y_power}, (Vector2){pos_x, pos_y_power+flow}, 2, BLACK);

	// border values
	DrawText(TextFormat("%.0lf%s", data->x_min, "\u00B0"), pos_x, pos_y_power+flow, font_size, BLACK);
	DrawText(TextFormat("%.0lf%s", data->x_max, "\u00B0"), pos_x+flow-30, pos_y_power+flow, font_size, BLACK);
	
	DrawText(TextFormat("%.2lf", data->y_min), pos_x-100, pos_y_power+flow-font_size/2, font_size, BLACK);
	DrawText(TextFormat("%.2lf", data->y_max), pos_x-100, pos_y_power-15, font_size, BLACK);

}

void Reflection(Ray *ray, Model *model)
{
	RayCollision col = GetRayCollisionMesh(*ray, *model->meshes, MatrixIdentity());
	if(col.hit)
	{
		Vector3 ref = Vector3Reflect(ray->direction, col.normal);
		Vector3 new = Vector3Add(col.point, Vector3Scale(ref, 1));
		if(IsKeyDown(KEY_K)) DrawLine3D(ray->position, col.point, RED);
		if(IsKeyDown(KEY_L)) DrawLine3D(new, col.point, GREEN);

	}
}
