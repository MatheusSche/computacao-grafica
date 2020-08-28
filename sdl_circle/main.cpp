// C program to implement Xiaolin Wu's line drawing
// algorithm.
// We must install SDL library using above steps
// to run this prorgram
#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<stdio.h>
#include <functional>
#include <algorithm>
#include <utility>

// SDL stuff
SDL_Window* pWindow = 0;
SDL_Renderer* pRenderer = 0;

// draws a pixel on screen of given brightness
// 0<=brightness<=1. We can use your own library
// to draw on screen
void swap(int* a , int*b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// returns absolute value of number
float absolute(float x )
{
    if (x < 0) return -x;
    else return x;
}

//returns integer part of a floating point number
int iPartOfNumber(float x)
{
    return (int)x;
}

//rounds off a number
int roundNumber(float x)
{
    return iPartOfNumber(x + 0.5) ;
}

//returns fractional part of a number
float fPartOfNumber(float x)
{
    if (x>0) return x - iPartOfNumber(x);
    else return x - (iPartOfNumber(x)+1);

}

//returns 1 - fractional part of number
float rfPartOfNumber(float x)
{
    return 1 - fPartOfNumber(x);
}

// draws a pixel on screen of given brightness
// 0<=brightness<=1. We can use your own library
// to draw on screen
void drawPixel( int x , int y , float brightness)
{
    int c = 255*brightness;
    SDL_SetRenderDrawColor(pRenderer, c, c, c, 255);
    SDL_RenderDrawPoint(pRenderer, x, y);
}

void displayBresehamCircle(int xc, int yc, int x, int y){
    drawPixel(xc+x, yc+y, 0);
    drawPixel(xc-x, yc+y, 0);
    drawPixel(xc+x, yc-y, 0);
    drawPixel(xc-x, yc-y, 0);
    drawPixel(xc+y, yc+x, 0);
    drawPixel(xc-y, yc+x, 0);
    drawPixel(xc+y, yc-x, 0);
    drawPixel(xc-y, yc-x, 0);
}

void drawBresehamCircle(int xc, int yc, int radius){
    int x = 0, y = radius;
    int decesionParameter = 3 - 2 * radius;
    displayBresehamCircle(xc, yc, x, y);

    while (y >= x){
        x++;
        if (decesionParameter > 0){
            y--;
            decesionParameter = decesionParameter + 4 * (x - y) + 10;
        } else {
            decesionParameter = decesionParameter + 4 * x + 6;
        }

        displayBresehamCircle(xc, yc, x, y);
    }
}

// Driver code
int main(int argc, char* args[])
{

	SDL_Event event;

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// if succeeded create our window
		pWindow = SDL_CreateWindow("Algoritmo de linhas de Bresenham",
					SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
														640, 480,
												SDL_WINDOW_SHOWN);

		// if the window creation succeeded create our renderer
		if (pWindow != 0)
			pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	}
	else
		return 1; // sdl could not initialize

	while (1)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		// Sets background color to white
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(pRenderer);

		// draw a circle
		int xc = 250, yc = 250, r2 = 180;

        drawBresehamCircle(xc, yc, r2);    // function call

		// show the window
		SDL_RenderPresent(pRenderer);
	}

	// clean up SDL
	SDL_Quit();
	return 0;
}
