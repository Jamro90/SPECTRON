#include "../plplot.h" 
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>

void ploter(void)
{
	PLFLT x[100]; // arguments
	PLFLT y[100]; // values
	
	PLFLT xmin = 0, xmax = 100, ymin = 0, ymax = 10;
	srand(time(NULL));
	plsdev("png");
	plsfnam("test.png");
	for(int i = 0; i < 100; ++i)
	{
		x[i] = (PLFLT) i;
		y[i] = (PLFLT) ( (int) rand() % 10 );
		printf("%lf,%lf\n", x[i], y[i]);
	}
//	plparseopts(&argc, argv, PL_PARSE_FULL);
	plinit(); // start plplot
	plenv(xmin, xmax, ymin, ymax, 0, 0);
	pllab("arguments", "values", "Make title for my plot");
	plline(100, x, y);

	plend();	// end plplot
}
