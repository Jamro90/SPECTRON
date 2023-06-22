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

void inline PolarChart(PLFLT *x, PLFLT *y)
{
	plenv((PLFLT) min_number(x), (PLFLT) max_number(x), (PLFLT) min_number(y), (PLFLT) max_number(y), 0, 2);
	plwind((PLFLT) min_number(x), (PLFLT) max_number(x), (PLFLT) min_number(y), (PLFLT) max_number(y));
	plcol0(4);
	pllab("X", "Y", "Polar chart");
	plcol0(3);
	plline( 1024, x, y);
	plflush();
}

void inline LogChart(PLFLT *x, PLFLT *y)
{
	plenv(min_number(x), max_number(x), min_number(y), max_number(y), 0, 2);
	plwind((PLFLT) min_number(x), (PLFLT) max_number(x), (PLFLT) min_number(y), (PLFLT) max_number(y));
	plcol0(4);
	pllab("X", "Y", "Polar chart");
	plcol0(3);
	plline( 1024, x, y);
	plflush();	
}

void GeneralChart(PLFLT *x, PLFLT *y)
{
	plsdev("png");
	plsfnam("general.png");

	PLINT charts = 3;
	plssub(1, 2);

	plstar(1, 2);
	plsyax(charts, 0);

	PolarChart(x, y);
	LogChart(x, y);

	plend();
}
