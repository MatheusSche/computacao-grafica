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

void drawAALine(int x0 , int y0 , int x1 , int y1)
{
    int steep = absolute(y1 - y0) > absolute(x1 - x0) ;

    // swap the co-ordinates if slope > 1 or we
    // draw backwards
    if (steep)
    {
        swap(&x0 , &y0);
        swap(&x1 , &y1);
    }
    if (x0 > x1)
    {
        swap(&x0 ,&x1);
        swap(&y0 ,&y1);
    }

    //compute the slope
    float dx = x1-x0;
    float dy = y1-y0;
    float gradient = dy/dx;
    if (dx == 0.0)
        gradient = 1;

    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;

    // main loop
    if (steep)
    {
        int x;
        for (x = xpxl1 ; x <=xpxl2 ; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            drawPixel(iPartOfNumber(intersectY), x,
                        rfPartOfNumber(intersectY));
            drawPixel(iPartOfNumber(intersectY)-1, x,
                        fPartOfNumber(intersectY));
            intersectY += gradient;
        }
    }
    else
    {
        int x;
        for (x = xpxl1 ; x <=xpxl2 ; x++)
        {
            // pixel coverage is determined by fractional
            // part of y co-ordinate
            drawPixel(x, iPartOfNumber(intersectY),
                        rfPartOfNumber(intersectY));
            drawPixel(x, iPartOfNumber(intersectY)-1,
                          fPartOfNumber(intersectY));
            intersectY += gradient;
        }
    }

}


void bresenham(int x1, int y1, int x2, int y2)
{
    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;

    dx1=fabs(dx);
    dy1=fabs(dy);

    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        drawPixel(x,y,0);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
            drawPixel(x,y,0);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        drawPixel(x,y,0);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
            drawPixel(x,y,0);
        }
    }
}

void bresenhamz(int x1, int y1, int x2, int y2){
    int slope;
    int dx, dy, incE, incNE, d, x, y;
    // Onde inverte a linha x1 > x2
    if (x1 > x2){
        bresenham(x2, y2, x1, y1);
         return;
    }
    dx = x2 - x1;
    dy = y2 - y1;

    if (dy < 0){
        slope = -1;
        dy = -dy;
    }
    else{
       slope = 1;
    }
    // Constante de Bresenham
    incE = 2 * dy;
    incNE = 2 * dy - 2 * dx;
    d = 2 * dy - dx;
    y = y1;
    for (x = x1; x <= x2; x++){
        drawPixel(x,y,0);
        if (d <= 0){
          d += incE;
        }
        else{
          d += incNE;
          y += slope;
        }
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

		// draws a black AALine
		bresenham(10 , 10 , 10, 150);
		drawAALine(70 , 260 , 70, 350);
		drawAALine(70 , 350 , 140, 350);

        drawAALine(140 , 350 , 140, 260);
        drawAALine(140 , 260 , 70, 260);



		// show the window
		SDL_RenderPresent(pRenderer);
	}

	// clean up SDL
	SDL_Quit();
	return 0;
}


void floodFill(int x, int y, Uint32 newColor, Uint32 oldColor){

    stack<Point> st;

    if (y < 0 || y > height-1 || x < 0 || x > width-1){
        return;
    }
    st.push(getPoint(x,y));

    while (st.size() > 0){
        Point p = st.top();
        st.pop();
        int x = p.x;
        int y = p.y;

        if (y < 0 || y > height - 1 || x < 0 || x > width -1){
            continue;
        }

        if (getPixel(x,y)==oldColor){
            setPixel(x,y, newColor);
            st.push(getPoint(x+1,y));
            st.push(getPoint(x-1,y));
            st.push(getPoint(x,y+1));
            st.push(getPoint(x,y-1));
        }
    }
}
