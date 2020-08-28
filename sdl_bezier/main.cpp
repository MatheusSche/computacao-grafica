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
void drawPixel( int x , int y , float brightness)
{
    int c = 255*brightness;
    SDL_SetRenderDrawColor(pRenderer, c, c, c, 255);
    SDL_RenderDrawPoint(pRenderer, x, y);
}


void bezier_curve(int x[] , int y[]){

    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    int i = 0 ;

    for (u = 0.0; u <= 1.0; u+=0.0001){
        xu = pow(1-u,3)*x[0]+3*u*pow(1-u,2)*x[1]+3*pow(u,2)*(1-u)*x[2]+pow(u,3)*x[3];
        yu = pow(1-u,3)*y[0]+3*u*pow(1-u,2)*y[1]+3*pow(u,2)*(1-u)*y[2]+pow(u,3)*y[3];

        drawPixel(xu, yu, 0);
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

	int x[4]={120,35,220,220} , y[4]={160,200,260,40};


	while (1)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		// Sets background color to white
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(pRenderer);

		// draw a bezier curve
		bezier_curve(x, y);

		// show the window
		SDL_RenderPresent(pRenderer);
	}

	// clean up SDL
	SDL_Quit();
	return 0;
}
