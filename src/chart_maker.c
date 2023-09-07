#include "chart_maker.h"
#include <math.h>
#include <stdio.h>
#include "gui_maker.h"
#include <float.h>

void DataCounter(DATA *data)
{
	data->x_max = DBL_MIN;
	data->x_min = DBL_MAX;

	data->y_max = DBL_MIN;
	data->y_min = DBL_MAX;

	data->theta_max = DBL_MIN;
	data->theta_min = DBL_MAX;

	data->z_max = DBL_MIN;
	data->z_min = DBL_MAX;

	FILE *f = fopen("dane.txt", "w");
	size_t items = sizeof(data->x) / sizeof(data->x[0]);
	DrawText(TextFormat("%d", items), 100, 100, 40, BLUE);
	for(size_t i = 0; i < items; ++i)
	{
		data->x[i] = 360*i/items; 
		data->y[i] = 10 * cos(data->x[i] * PI/180) + 10 * sin(3 * data->x[i] * PI/180);
	
		if( (data->y[i] >= 0) && (data->x[i] > 0) ) data->theta[i] = atan(data->y[i]/data->x[i]);
		else if( (data->y[i] >= 0) && (data->x[i] < 0)) data->theta[i] = atan(data->y[i]/data->x[i]) + PI/2;
		else if( (data->y[i] < 0) && (data->x[i] < 0) ) data->theta[i] = atan(data->y[i]/data->x[i]) + PI;
		else if( (data->y[i] < 0) && (data->x[i] > 0) ) data->theta[i] = atan(data->y[i]/data->x[i]) + 3/2 * PI;
		else if( (data->x[i] == 0) && (data->y[i] > 0) ) data->theta[i] = 180;
		else if( (data->x[i] == 0) && (data->y[i] < 0) ) data->theta[i] = -180;

		data->z[i] = sqrt(data->x[i]*data->x[i]+data->y[i]*data->y[i]); 

		if(data->x[i] < data->x_min) data->x_min = data->x[i];
		if(data->x[i] > data->x_max) data->x_max = data->x[i];
		
		if(data->y[i] < data->y_min) data->y_min = data->y[i];
		if(data->y[i] > data->y_max) data->y_max = data->y[i];

		if(data->theta[i] < data->theta_min) data->theta_min = data->theta[i];
		if(data->theta[i] > data->theta_max) data->theta_max = data->theta[i];

		if(data->z[i] < data->z_min) data->z_min = data->z[i];
		if(data->z[i] > data->z_max) data->z_max = data->z[i];
		fprintf(f, "%lf\t", data->x[i]);
		fprintf(f, "%lf\t", data->y[i]);
		fprintf(f, "%lf\t", data->theta[i]);
		fprintf(f, "%lf\n", data->z[i]);
	}
	fclose(f);
}

void PolarChart(DATA *data)
{
	int pos_x = 700;
	int pos_y = GetScreenHeight()/2;
	int flow = 500;
	int radius  = 5;
	int font_size = 30;

	size_t items = sizeof(data->x) / sizeof(data->x[0]);

	// background
	DrawRectangle(pos_x, pos_y, flow, flow, LIGHTGRAY);
	DrawText("Polar Chart", pos_x+5, pos_y-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y+0.5*flow}, (Vector2){pos_x+flow, pos_y+0.5*flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x+0.5*flow, pos_y}, (Vector2){pos_x+0.5*flow, pos_y+flow}, 2, BLACK);

	for(size_t i = 1; i < items; ++i)
	{
		if(radius*i < flow/2) DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y}, (i+1)*radius,(i+1)*radius+2, 0, 360, 360, BLACK);
	}

	double start = 0;
	for(size_t i = 0; i < items; ++i)
	{
		DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y}, data->z[i]*exp(data->theta[i]), (data->z[i]+2)*exp(data->theta[i]), start, data->theta[i], 10, GREEN);
		start += data->theta[i];
	}
}

void PowerChart(DATA *data)
{
	int pos_x = 100;
	int pos_y = GetScreenHeight()/2;
	int flow = 500;
	int font_size = 30;

	size_t items = sizeof(data->x) / sizeof(data->x[0]);
	double d = flow/360.0;
	double f = (double) flow/data->y_max * 0.5;
	// background
	DrawRectangle(pos_x, pos_y, flow, flow, LIGHTGRAY);
	DrawText("Power Chart", pos_x+5, pos_y-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y+flow}, (Vector2){pos_x+flow, pos_y+flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x, pos_y+flow}, 2, BLACK);

	// border values
	DrawText(TextFormat("%.0lf%s", data->x_min, "\u00B0"), pos_x, pos_y+flow, font_size, BLACK);
	DrawText(TextFormat("%.0lf%s", data->x_max, "\u00B0"), pos_x+flow-30, pos_y+flow, font_size, BLACK);
	
	DrawText(TextFormat("%.2lf", data->y_min), pos_x-100, pos_y+flow-font_size/2, font_size, BLACK);
	DrawText(TextFormat("%.2lf", data->y_max), pos_x-100, pos_y-15, font_size, BLACK);

	// enable points
//	DrawCircle(data->x[0]+pos_x, -data->y[0]+pos_y+flow+f*data->y_min, 5, RED);
	for(size_t i = 1; i < items; ++i)
	{
		DrawLine(d*data->x[i-1]+pos_x, -f*data->y[i-1]+pos_y+flow+f*data->y_min, d*data->x[i]+pos_x, -f*data->y[i]+pos_y+flow+f*data->y_min, BLUE);

		// enable points
//		DrawCircle(d*data->x[i]+pos_x, -f*data->y[i]+pos_y+flow+f*data->y_min, 5, RED);
	}
}
