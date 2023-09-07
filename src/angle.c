#include <stdio.h>

int main(void)
{
	double x[1024];
	printf("sizeof = %d\n", sizeof(x));
	size_t items = sizeof(x)/sizeof(x[0]);
	for(size_t i = 0; i < items; ++i)
	{
		x[i] = 360.0 / items * i;
		printf("%d)\t%lf\n", i, x[i]);
	}

	return 0;
}
