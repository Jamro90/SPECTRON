#include "chart_maker.h"
#include "calc.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <raymath.h>

#include "structs.h"

void DataCounter(DATA *data, Model *model, Radar *radar, bounding *bound, float *progress){
	uint16_t items = sizeof(data->point.x) / sizeof(data->point.x[0]);
	uint16_t row = pow(items, 1/3);

	#if 1
	if(data->iter <= items){
		data->iter += 1;
		*progress = 100 * (float) data->iter/items;
		
		if((row % data->iter) == 0){
			bound->p_x = 0;
			bound->p_y += 1;
		}
	
		if(bound->p_x == row/2) bound->p_x += 1;
		if(bound->p_y == row/2) bound->p_x += 1;

	
		float y1 = (bound->ratio * bound->p_y/2);
		float x1;
		float z1;

		if((radar->elevation > 45) || (radar->elevation < -45)){	
			x1 = (bound->ratio * bound->p_x/2);
			z1 = 1;
		}
		else{
			x1 = 1;
			z1 = (bound->ratio * bound->p_x/2);
		}

		Ray ray_front = {(Vector3){radar->x+x1, radar->y+y1, radar->z+z1}, (Vector3){-radar->x+2*x1, -radar->y+2*y1, -radar->z+2*z1}};	
		Vector3 result = Reflection(&ray_front, model, radar);
		//result.x = result.x * cos((radar->azymuth - result.y) * PI/180) * sin((radar->elevation- result.z) * PI/180);
		data->point.x[data->iter] = result.y;
		data->point.y[data->iter] = result.x * cos(result.y * PI/180) * sin(result.z * PI/180);
		/*
		// sorting algorithm
		int i = 0;
		while(true)
		{
			if(result.y < data->point.x[data->iter-i])
			{
				double buffer_x = data->point.x[data->iter-i];
				double buffer_y = data->point.y[data->iter-i];

				data->point.x[data->iter-i] = result.y; 
				data->point.y[data->iter-i] = cos(result.y * PI/180) * sin(result.z * PI/180);
; 

				data->point.x[data->iter] = buffer_x;
				data->point.y[data->iter] = buffer_y;
					
				i += 1;
			}	
			else 
			{
				break;
			}
		}
		*/
	}
	#else
	int xi = -16;
	int yi = -16;
	int zi = 1; 
	for(int i = 0; i < items; ++i)
	{
		if(xi == 0) xi += 1;
		if(yi == 0) yi += 1;
		if(xi == 16)
		{
			xi = -16;
			yi += 1;
			if(yi == 16)
			{
				yi = -16;
		//		zi += 1;
			}
		}
			
		float y1 = yi*bound->ratio;
		float x1;
		float z1;
		if((radar->elevation < -45) || (radar->elevation > 45))
		{
		x1 = xi*bound->ratio; 
		z1 = zi*bound->ratio; 
		}
		else 
		{
		x1 = zi*bound->ratio; 
		z1 = xi*bound->ratio;
		}
		Ray ray_front = {(Vector3){radar->x+x1, radar->y+y1, radar->z+z1}, (Vector3){-radar->x, -radar->y, -radar->z}};	
		Reflection(&ray_front, model, radar);
		xi += 1;	
	}
	#endif
}

void DrawCharts(DATA *data){
	int pos_x = 100;
	int pos_y_power = GetScreenHeight()/1.9;
	int pos_y_polar = GetScreenHeight()/20;
	int flow = 450;
	int radius = 5;
	int font_size = 30;

	DrawRectangle(pos_x, pos_y_polar, flow, flow, RAYWHITE);
	DrawRectangle(pos_x, pos_y_power, flow, flow, RAYWHITE);
	
	data->x_max = 360;
	data->x_min = 0;
	
	uint16_t items = sizeof(data->point.x) / sizeof(data->point.x[0]);

	if(data->point.y[0] < data->y_min) data->y_min = data->point.y[0];
	else if(data->point.y[0] > data->y_max) data->y_max = data->point.y[0];

	for(uint16_t i = 1; i < items; ++i){
		if(data->point.y[i] < data->y_min) data->y_min = data->point.y[i];
		else if(data->point.y[i] > data->y_max) data->y_max = data->point.y[i];

		double f = (double) flow/data->y_max;

		double d = (double) flow/data->x_max;

		if( (radius*(int)i <= flow/2) && ((i % 5) == 0) ) DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y_polar}, (i+1)*radius,(i+1)*radius+1, 0, 360, 360, BLACK);

		// POLAR
		DrawLineEx((Vector2){f/2*data->point.y[i-1]*cos(data->point.x[i-1]*PI/180)+pos_x+flow/2,
			 -f/2*data->point.y[i-1]*sin(data->point.x[i-1]*PI/180)+pos_y_polar+flow/2},
			 (Vector2){f/2*data->point.y[i]*cos(data->point.x[i]*PI/180)+pos_x+flow/2,
			 -f/2*data->point.y[i]*sin(data->point.x[i]*PI/180)+pos_y_polar+flow/2}, 2, DARKGREEN);

		// POWER
		DrawLine(d*data->point.x[i-1]+pos_x, -10*log10(f*data->point.y[i-1]*1000)+pos_y_power+flow+f*data->y_min, d*data->point.x[i]+pos_x, -10*log10(f*data->point.y[i]*1000)+pos_y_power+flow+f*data->y_min, BLUE);
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
	DrawText(TextFormat("%.0lf%s", data->x_min, "\u00B0"), pos_x, pos_y_power+flow, font_size, BLACK);
	DrawText(TextFormat("%.0lf%s", data->x_max, "\u00B0"), pos_x+flow-40, pos_y_power+flow, font_size, BLACK);
	
	DrawText(TextFormat("%.2lf", data->y_min), pos_x-100, pos_y_power+flow-font_size/2, font_size, BLACK);
	DrawText(TextFormat("%.2lf", data->y_max), pos_x-100, pos_y_power-15, font_size, BLACK);

}

Vector3 Reflection(Ray *ray, Model *model, Radar *radar){
	float dist = 0;
	float azym = 0;
	float elev = 0;

	RayCollision col = GetRayCollisionMesh(*ray, *model->meshes, MatrixIdentity());
	if(col.hit){
		Vector3 ref = Vector3Reflect(ray->direction, col.normal);
		Vector3 new = Vector3Add(col.point, Vector3Scale(ref, 1));
		if(IsKeyDown(KEY_K)) DrawLine3D(ray->position, col.point, RED);
		if(IsKeyDown(KEY_L)) DrawLine3D(new, col.point, GREEN);
	
		Ray reflect = {col.point, ref};
		RayCollision sphere = GetRayCollisionSphere(reflect, (Vector3){0, 0, 0}, radar->distance);
		if(sphere.hit){	
			Cartesian2Polar(&sphere.point.x, &sphere.point.y, &sphere.point.z, &dist, &azym, &elev);
		}
	}
	return (Vector3){dist, azym, elev};
}
