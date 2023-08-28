#include "raylib.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void plot(double *x, double *y, int WIDTH, int HEIGHT);

int main(void)
{

	int WIDTH = 800;
	int HEIGHT = 600;
	InitWindow(WIDTH, HEIGHT, "Ray_test"); 

	int font_size = 25;
	int pad = 50;
	int x_pad = WIDTH - pad; 
	int y_pad = HEIGHT/2;
	int y_pad2 = HEIGHT - pad;

	size_t Array_len = x_pad - pad;

	srand(time(0));
	double x[Array_len];
	double y[Array_len];

	Vector2 canvas_sy = {pad, y_pad2};
	Vector2 canvas_sx = {pad, y_pad};
	Vector2 canvas_ey = {pad, pad};
	Vector2 canvas_ex = {x_pad, y_pad};
		
	double y_max;

	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GRAY);

		if(IsKeyDown(KEY_R))
		{
			for(size_t i = 0; i < Array_len; ++i)
			{
				x[i] = i;
				y[i] = rand()%250 * pow(-1,i);
				if(y[i] > y_max) y_max = y[i];
			}
		}

		plot(x, y, WIDTH, HEIGHT);
	/*	
		for(size_t i = 0; i < y_max/10; ++i)
		{
			DrawCircle(WIDTH/2, HEIGHT/2, y_max*i/100, BLACK);
			DrawCircle(WIDTH/2, HEIGHT/2, y_max*i/100 - 10, GRAY);
		}
*/
		EndDrawing();
	}
}

void plot(double *x, double *y, int WIDTH, int HEIGHT)
{
	int font_size = 25;
	int pad = 50;
	int x_pad = WIDTH - pad; 
	int y_pad = HEIGHT/2;
	int y_pad2 = HEIGHT - pad;

	Vector2 canvas_sy = {pad, y_pad2};
	Vector2 canvas_sx = {pad, y_pad};
	Vector2 canvas_ey = {pad, pad};
	Vector2 canvas_ex = {x_pad, y_pad};
	
	size_t Array_len = x_pad - pad;

	for(size_t i = 0; i + 1 < Array_len; ++i)
	{
		DrawLine(x[i] + pad, -y[i] + y_pad, x[i+1] + pad, -y[i+1] + y_pad, RED);
	}
	
	DrawLineEx(canvas_sx, canvas_ex, 5.0f, BLACK);
	DrawText(TextFormat("x"), x_pad, y_pad + font_size, font_size, BLACK);
	DrawLineEx(canvas_sy, canvas_ey, 5.0f, BLACK);
	DrawText(TextFormat("f(x)"), pad/4, pad - font_size, font_size, BLACK);
}
