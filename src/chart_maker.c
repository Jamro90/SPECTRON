#include "plplot_h/plplot.h"
#include "chart_maker.h"

double max_number(double *array)
{
	double max = -999.9999;
	for(size_t i = 0; i < sizeof(array); ++i)
	{
		if(array[i] > max) max = array[i];
	}
	
	return max;
}

double min_number(double *array)
{
	double min = 999.9999;
	for(size_t i = 0; i < sizeof(array); ++i)
	{
		if(array[i] < min) min = array[i];
	}

	return min;
}

void PolarChart(PLFLT *x, PLFLT *y)
{
	plenv((PLFLT) min_number(x), (PLFLT) max_number(x), (PLFLT) min_number(y) * 1.1, (PLFLT) max_number(y) * 1.1, 0, 2);
	pllab("X", "Y", "Polar chart");
	plline( sizeof(x), x, y);
}

void LogChart(PLFLT *x, PLFLT *y)
{
	plenv(min_number(x), max_number(x), min_number(y) * 1.1, max_number(y) * 1.1, 0, 2);
	pllab("X", "Y", "Logaritmic chart");
	plline( sizeof(x), x, y);
}

void GeneralChart(PLFLT *x, PLFLT *y)
{
	plsdev("png");
	plsfnam("general.png");

	plscol0a(0, 245, 245, 245, 0.5); // set background color to white (alter default)
	plscol0(1, 0, 0, 0); // set color nr 1 to black (default)

	PLINT charts = 3;
	plssub(1, 2);

	plstar(1, 2);
	plsyax(charts, 0);

	PolarChart(x, y);
	LogChart(x, y);

	plflush();

	plend();
}
