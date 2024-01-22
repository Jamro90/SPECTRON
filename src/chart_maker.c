#include "chart_maker.h"
#include "calc.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <raymath.h>

#include "structs.h"

#define dBm(x) 10 * log10(fabs(x)*1000)
#define MIN_VALUE 0.01

void DataCounter(DATA *data, Model *model, Radar *radar, bounding *bound, float *progress){
	uint16_t items = sizeof(data->point.x) / sizeof(data->point.x[0]);
	uint16_t row = sqrt(items);
//	int data->iter = 0;
	if(data->iter == 0){
		bound->p_x = -32;
		bound->p_y = -32;
	}
//	for(data->iter = 0; data->iter < items; ++data->iter){
	if(data->iter < items){
		data->iter += 1;
		*progress = 100 * (float) data->iter/items;
		if(bound->p_x == 16){
			bound->p_x = -16;
			bound->p_y += 1;
		}

		if((row % data->iter) == 0){
			bound->p_x = 0;
			bound->p_y += 1;
		}
	
		bound->p_x += 1;
	
		float y1 = (bound->ratio * bound->p_y);
		float x1;
		float z1;

		if((radar->elevation > 45) || (radar->elevation < -45)){	
			x1 = (bound->ratio * bound->p_x);
			z1 = 1;
		}
		else{
			x1 = 1;
			z1 = (bound->ratio * bound->p_x);
		}

		Vector3 begin = {radar->x, radar->y, radar->z};
		Vector3 direct = {-radar->x, -radar->y, -radar->z};
		Vector3 offset = {x1, y1, z1};

		Vector3 ray_start = Vector3Add(begin, offset);
		Vector3 ray_direct = direct;
		
		Ray ray_front = {ray_start, ray_direct};
		Vector3 result = Reflection(&ray_front, model, radar);

		data->point.x[data->iter] = result.x;
		data->point.y[data->iter]= fabs(result.y * cos(result.x * PI/180) * sin(result.z * PI/180));
	}
}

// SortData() made by using code example:
// https://www.geeksforgeeks.org/c-program-to-sort-an-array-in-ascending-order/
void SortData(DATA *data){
	int min_idx; 
	int min_idy;
	uint16_t items = sizeof(data->point.x) / sizeof(data->point.x[0]);
	for (int i = 0; i < items - 1; ++i) { 
		min_idx = i; 
		min_idy = i;
		for (int j = i + 1; j < items; ++j) if (data->point.x[j] < data->point.x[min_idx]) min_idx = j; 
		for (int j = i + 1; j < items; ++j) if (data->point.y[j] < data->point.y[min_idy]) min_idy = j; 
  
		int buffer_x = data->point.x[min_idx]; 
		int buffer_y = data->point.y[min_idx]; 
		data->point.x[min_idx] = data->point.x[i];
		data->point.y[min_idx] = data->point.y[i];
		data->point.x[i] = buffer_x; 
		data->point.y[i] = buffer_y; 
	}
}

void DrawCharts(DATA *data, bool *screenshot){
	int pos_x = 100;
	int pos_y_power = GetScreenHeight()/1.9;
	int pos_y_polar = GetScreenHeight()/20;
	int flow = 450;
	int radius = 5;
	int font_size = 30;

	Color color;
	if(*screenshot) color = RAYWHITE;
	else color = BLANK;

	DrawRectangle(pos_x, pos_y_polar, flow, flow, color);
	DrawRectangle(pos_x, pos_y_power, flow, flow, color);
	
	uint16_t items = sizeof(data->point.x) / sizeof(data->point.x[0]);

	if((data->point.y[0] < data->y_min) && (data->point.y[0] != 0)) data->y_min = data->point.y[0];
	else if(data->point.y[0] > data->y_max) data->y_max = data->point.y[0];

	for(uint16_t i = 1; i < items; ++i){
		if((data->point.y[i] < data->y_min) && (data->point.y[i] != 0)) data->y_min = data->point.y[i];
		else if(data->point.y[i] > data->y_max) data->y_max = data->point.y[i];

		double f = (double) flow/data->y_max;

		if( (radius*(int)i <= flow/2) && ((i % 5) == 0) ) DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y_polar}, (i+1)*radius,(i+1)*radius+1, 0, 360, 360, BLACK);

		// POLAR
		DrawLineEx((Vector2){f/2*data->point.y[i-1]*cos(data->point.x[i-1]*PI/180)+pos_x+flow/2,
			 f/2*data->point.y[i-1]*sin(data->point.x[i-1]*PI/180)+pos_y_polar+flow/2},
			 (Vector2){f/2*data->point.y[i]*cos(data->point.x[i]*PI/180)+pos_x+flow/2,
			 f/2*data->point.y[i]*sin(data->point.x[i]*PI/180)+pos_y_polar+flow/2}, 2, DARKGREEN);

		// POWER
		DrawLine(data->point.x[i-1]+pos_x, -f*data->point.y[i-1]+pos_y_power+flow, 
		data->point.x[i]+pos_x, -f*data->point.y[i]+pos_y_power+flow, BLUE);
	}

	// POLAR CHART
	DrawText("Polar Chart", pos_x+5, pos_y_polar-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y_polar+0.5*flow}, (Vector2){pos_x+flow, pos_y_polar+0.5*flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x+0.5*flow, pos_y_polar}, (Vector2){pos_x+0.5*flow, pos_y_polar+flow}, 2, BLACK);
	
	// POWER CHART
	DrawText("Power Chart", pos_x+5, pos_y_power-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y_power+flow}, (Vector2){pos_x+flow, pos_y_power+flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x, pos_y_power}, (Vector2){pos_x, pos_y_power+flow}, 2, BLACK);

	// border values
	DrawText(TextFormat("%d%s", 0, "\u00B0"), pos_x, pos_y_power+flow, font_size, BLACK);
	DrawText(TextFormat("%d%s", 360, "\u00B0"), pos_x+flow-40, pos_y_power+flow, font_size, BLACK);
	
	DrawText(TextFormat("%.2lf", dBm(data->y_min)), pos_x-100, pos_y_power+flow-font_size/2, font_size, BLACK);
	DrawText(TextFormat("%.2lf", dBm(data->y_max)), pos_x-100, pos_y_power-15, font_size, BLACK);
}

Vector3 Reflection(Ray *ray, Model *model, Radar *radar){
	float dist = 0;
	float azym = 0;
	float elev = 0;

	float vec_dist = 0;
	RayCollision col = GetRayCollisionMesh(*ray, *model->meshes, MatrixIdentity());
	if(col.hit){
		Vector3 ref = Vector3Reflect(ray->direction, col.normal);
		Vector3 new = Vector3Add(col.point, Vector3Scale(ref, 1));
		if(IsKeyDown(KEY_K)) DrawLine3D(ray->position, col.point, RED);
		if(IsKeyDown(KEY_L)) DrawLine3D(new, col.point, GREEN);
		
		vec_dist = Vector3Length(ref);
		Ray reflect = {col.point, new};
		RayCollision sphere = GetRayCollisionSphere(reflect, (Vector3){0, 0, 0}, radar->distance);

		if(sphere.hit) Cartesian2Polar(&sphere.point.x, &sphere.point.y, &sphere.point.z, &dist, &azym, &elev);
	}
	return (Vector3){azym, vec_dist, elev};
}
