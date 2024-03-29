#ifndef CHART_MAKER
#define CHART_MAKER

#include <stdio.h>
#include "chart_maker.h"

#include "structs.h"

void DataCounter(DATA *data, Model *model, Radar *radar, bounding * bound, float *progress);

void SortData(DATA *data);

void DrawCharts(DATA *data, bool *screenshot);

void PolarChart(DATA *data);

void PowerChart(DATA *data);

Vector3 Reflection(Ray *ray, Model *model, Radar *radar);

#endif // CHART_MAKER
