#ifndef CHART_MAKER
#define CHART_MAKER

#include <stdio.h>
#include "chart_maker.h"

typedef struct{
	double x[1024];
	double y[1024];
	double x_max;
	double x_min;
	double y_max;
	double y_min;
}DATA;

void DataCounter(DATA *data);

void PolarChart(DATA *data);

void PowerChart(DATA *data);

#endif // CHART_MAKER
