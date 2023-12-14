#ifndef CHART_MAKER
#define CHART_MAKER

#include <stdio.h>
#include "chart_maker.h"

#include "structs.h"

int DataCounter(DATA *data, Model *model, Radar *radar, float *progress);

void DrawCharts(DATA *data);

void PolarChart(DATA *data);

void PowerChart(DATA *data);

void Reflection(Ray *ray, Model *model);

#endif // CHART_MAKER
