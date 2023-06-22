#ifndef CHART_MAKER
#define CHART_MAKER

#include "plplot_h/plplot.h"
#include "chart_maker.h"

double max_number(double *array);

double min_number(double *array);

void PolarChart(double *x, double *y);

void LogChart(double *x, double *y);

void GeneralChart(double *x, double *y);


#endif // CHART_MAKER
