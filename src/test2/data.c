
#include<string.h>

//--------------------------------------------------------
// FUNCTION populate_data
//--------------------------------------------------------
void populate_data(double data[12], double labels[12])
{
    memcpy(labels, (double[12]){1910,1920,1930,1940,1950,1960,1970,1980,1990,2000,2010,2020}, sizeof(double[12]));

    memcpy(data, (double[12]){2,6, 29,84, 364,622, 4106, 6951, 15994, 81022, 198240, 765008}, sizeof(double[12]));
}
