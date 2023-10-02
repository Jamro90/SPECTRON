#include "chart_maker.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>
//#include "gui_maker.h"
#include <float.h>

void DataCounter(DATA *data)
{
	data->y_max = DBL_MIN;
	data->y_min = DBL_MAX;

	size_t items = sizeof(data->x) / sizeof(data->x[0]);

	for(size_t i = 0; i < items; ++i)
	{
		data->y[i] = 10 * cos(data->x[i] * PI/180) + 10 * sin(3 * data->x[i] * PI/180);
//		data->y[i] = cos(data->x[i] * PI/180);
//		data->y[i] = log(data->x[i]);
//		data->y[i] = data->x[i] * data->x[i];

		if(data->y[i] < data->y_min) data->y_min = data->y[i];
		if(data->y[i] > data->y_max) data->y_max = data->y[i];
	}
}

void PolarChart(DATA *data)
{
	int pos_x = 100;
	int pos_y = GetScreenHeight()/20;
	int flow = 450;
	int radius  = 5;
	int font_size = 30;

	size_t items = sizeof(data->x) / sizeof(data->x[0]);
	double f = (double) flow/data->y_max * 0.5;

	// background
	DrawRectangle(pos_x, pos_y, flow, flow, BLANK);
	DrawText("Polar Chart", pos_x+5, pos_y-font_size, font_size, BLACK);
	// x axis
	DrawLineEx((Vector2){pos_x, pos_y+0.5*flow}, (Vector2){pos_x+flow, pos_y+0.5*flow}, 2, BLACK);
	// y axis
	DrawLineEx((Vector2){pos_x+0.5*flow, pos_y}, (Vector2){pos_x+0.5*flow, pos_y+flow}, 2, BLACK);

	for(size_t i = 1; i < items; ++i)
	{
		if( (radius*i <= flow/2) && ((i % 5) == 0) ) DrawRing((Vector2){flow/2+pos_x, flow/2+pos_y}, (i+1)*radius,(i+1)*radius+1, 0, 360, 360, BLACK);

		DrawLineEx((Vector2){f*data->y[i-1]*cos(data->x[i-1]*PI/180)+pos_x+flow/2,
			 -f*data->y[i-1]*sin(data->x[i-1]*PI/180)+pos_y+flow/2},
			 (Vector2){f*data->y[i]*cos(data->x[i]*PI/180)+pos_x+flow/2,
			 -f*data->y[i]*sin(data->x[i]*PI/180)+pos_y+flow/2}, 2, DARKGREEN);
	}
}

void PowerChart(DATA *data)
{
	int pos_x = 100;
	int pos_y = GetScreenHeight()/2;
	int flow = 450;
	int font_size = 30;

	size_t items = sizeof(data->x) / sizeof(data->x[0]);
	double d = flow/360.0;
	double f = (double) flow/data->y_max * 0.5;
	// background
	DrawRectangle(pos_x, pos_y, flow, flow, BLANK);
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
