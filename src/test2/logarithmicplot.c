
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<locale.h>
#include<stdlib.h>

#include"data.h"
#include"svg.h"

//--------------------------------------------------------
// FUNCTION PROTOTYPES
//--------------------------------------------------------
void print_data(double* data, double* labels, int size);
void draw_logarithmic_plot(int width,
                           int height,
                           char* title,
                           double* data,
                           double* labels,
                           int size,
                           int maxpower,
                           char* filename);

//--------------------------------------------------------
// FUNCTION main
//--------------------------------------------------------
int main(int argc, char* argv[])
{
    puts("--------------------");
    puts("| codedrome.com    |");
    puts("| Logarithmic Plot |");
    puts("--------------------\n");

    double data[12];
    double labels[12];

    populate_data(data, labels);

    print_data(data, labels, 12);

    draw_logarithmic_plot(720, 540, "Logarithmic Plot", data, labels, 12, 6, "logarithmicplot1.svg");

    return EXIT_SUCCESS;
}

//--------------------------------------------------------
// FUNCTION print_data
//--------------------------------------------------------
void print_data(double* data, double* labels, int size)
{
    puts("       label         data  log10(data)\n--------------------------------------");

    for(int i = 0; i < size; i++)
    {
        printf("%12.0lf %12.0lf %12.6lf\n", labels[i], data[i], log10(data[i]));
    }
}

//--------------------------------------------------------
// FUNCTION draw_logarithmic_plot
//--------------------------------------------------------
void draw_logarithmic_plot(int width, int height, char* title, double* data, double* labels, int size, int maxpower, char* filename)
{
    int topmargin = 64;
    int bottommargin = 32;
    int leftmargin = 86;
    int rightmargin = 32;

    int graph_height = height - topmargin - bottommargin;
    int graph_width = width - leftmargin - rightmargin;
    double pixels_per_unit_x = (double)graph_width / (double)(size - 1);
    double pixels_per_unit_y = (double)graph_height / (double)maxpower;
    double x;
    double y;
    char number_string[8];

    // Create svg struct
    svg* psvg;
    psvg = svg_create(width, height);

    if(psvg == NULL)
    {
        puts("psvg is NULL");
    }
    else
    {
        svg_fill(psvg, "#FFFFFF");

        // header text and border lines
        svg_text(psvg, width/2, 38, "sans-serif", 16, "#000000", "#000000", "middle", title);
        svg_line(psvg, "#808080", 2, leftmargin, topmargin, leftmargin, height - bottommargin);
        svg_line(psvg, "#808080", 2, leftmargin, height - bottommargin, width - rightmargin, height - bottommargin);

        // y axis indexes and values
        y = height - bottommargin;
        for(int power = 0; power <= maxpower; power++)
        {
            svg_line(psvg, "#808080", 1, leftmargin - 8, y, leftmargin, y);

            sprintf(number_string, "%.0lf", pow(10, power));
            svg_text(psvg, leftmargin - 12, y + 4, "sans-serif", 10, "#000000", "#000000", "end", number_string);

            y -= pixels_per_unit_y;
        }

        // x axis indexes and values
        x = leftmargin;
        for(int i = 0; i < size; i++)
        {
            svg_line(psvg, "#808080", 1, x, height - bottommargin, x, height - bottommargin + 8);

            sprintf(number_string, "%.0lf", labels[i]);
            svg_text(psvg, x, height - bottommargin + 24, "sans-serif", 10, "#000000", "#000000", "middle", number_string);

            x += pixels_per_unit_x;
        }

        // plot data
        x = leftmargin;

        for(int d = 0; d < size; d++)
        {
            y = height - bottommargin - (log10(data[d]) * pixels_per_unit_y);

            svg_circle(psvg, "#0000FF", 0, "#0000FF", 3, x, y);

            x += pixels_per_unit_x;
        }

        // finish off
        svg_finalize(psvg);

        svg_save(psvg, filename);

        puts("File saved");

        svg_free(psvg);
    }
}
