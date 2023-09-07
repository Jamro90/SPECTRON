#ifndef CHART_MAKER
#define CHART_MAKER

#include <stdio.h>
#include "chart_maker.h"
#include "gui_maker.h"

typedef struct{
	double *x;
	double *y;
	int len;
	char *title;
	char *x_label;
	char *y_label;
	int width;
	int height;
	int font_size;
	int x_pad;
	int x_pad2;
	int y_pad;
	int y_pad2;
}Chart;

typedef struct{
	double x[1024];
	double y[1024];
	double theta[1024];
	double z[1024];
	double x_max;
	double x_min;
	double y_max;
	double y_min;
	double theta_max;
	double theta_min;
	double z_max;
	double z_min;
}DATA;

void DataCounter(DATA *data);

void PolarChart(DATA *data);

void PowerChart(DATA *data);

#endif // CHART_MAKER
