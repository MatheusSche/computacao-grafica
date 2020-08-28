
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stack>
#include <sstream>
#include <unistd.h>

using namespace std;

// Estrutura para representar pontos
typedef struct {int x, y;} Point;

// Variáveis necessárias para o SDL
unsigned int * pixels;
int width, height;
SDL_Surface * window_surface;
SDL_Renderer * renderer;

// Título da janela
std::string titulo = "SDL Points";

// Valores RGB para a cor de funco da janela
const int VERMELHO = 0;
const int VERDE = 0;
const int AZUL = 0;

// Gera uma estrutura Point a partir de valores para x e y
Point getPoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// Obtém a cor de um pixel de uma determinada posição
Uint32 getPixel(int x, int y)
{
    if((x>=0 && x<=width) && (y>=0 && y<=height))
        return pixels[x + width * y];
    else
        return -1;
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g, b, e alpha (transparência)
// r, g, b e a variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b, int a)
{
    pixels[x + y * width] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g e b
// r, g, e b variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b)
{
    setPixel(x, y, r, g, b, 255);
}

// Mostra na barra de título da janela a posição
// corrente do mouse
void showMousePosition(SDL_Window * window, int x, int y)
{
    std::stringstream ss;
    ss << titulo << " X: " << x << " Y: " << y;
    SDL_SetWindowTitle(window, ss.str().c_str());
}

// Imprime na console a posição corrente do mouse
void printMousePosition(int x, int y)
{
    printf("Mouse on x = %d, y = %d\n",x,y);
}

// Seta um pixel em uma determinada posição,
// através de um Uint32 representando
// uma cor RGB
void setPixel(int x, int y, Uint32 color)
{
    if((x<0 || x>=width || y<0 || y>=height)) {
        printf("Coordenada inválida : (%d,%d)\n",x,y);
        return;
    }
    pixels[x + y * width] = color;
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, b e a(transparência)
// informadas. r, g, b e a variam de 0 até 255
Uint32 RGB(int r, int g, int b, int a) {
    return SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, e b
// informadas. r, g e b variam de 0 até 255
// a transparência é sempre 255 (imagem opaca)
Uint32 RGB(int r, int g, int b) {
    return SDL_MapRGBA(window_surface->format, r, g, b, 255);
}

// Retorna um componente de cor de uma cor RGB informada
// aceita os parâmetros 'r', 'R','g', 'G','b' e 'B',
Uint8 getColorComponent( Uint32 pixel, char component ) {

    Uint32 mask;

    switch(component) {
        case 'b' :
        case 'B' :
            mask = RGB(0,0,255);
            pixel = pixel & mask;
            break;
        case 'r' :
        case 'R' :
            mask = RGB(255,0,0);
            pixel = pixel & mask;
            pixel = pixel >> 16;
            break;
        case 'g' :
        case 'G' :
            mask = RGB(0,255,0);
            pixel = pixel & mask;
            pixel = pixel >> 8;
            break;
    }
    return (Uint8) pixel;
}


void bresenham_line(int x1, int y1, int x2, int y2)
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
        setPixel(x,y,127,255,0);
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
            setPixel(x,y,127,255,0);
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
        setPixel(x,y,127,255,0);
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
            setPixel(x,y,127,255,0);
        }
    }
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

void displayBresehamCircle(int xc, int yc, int x, int y){
    setPixel(xc+x, yc+y, 255);
    setPixel(xc-x, yc+y, 255);
    setPixel(xc+x, yc-y, 255);
    setPixel(xc-x, yc-y, 255);
    setPixel(xc+y, yc+x, 255);
    setPixel(xc-y, yc+x, 255);
    setPixel(xc+y, yc-x, 255);
    setPixel(xc-y, yc-x, 255);
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

void bezier_curve(int x[] , int y[]){

    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    int i = 0 ;

    for (u = 0.0; u <= 1.0; u+=0.0001){
        xu = pow(1-u,3)*x[0]+3*u*pow(1-u,2)*x[1]+3*pow(u,2)*(1-u)*x[2]+pow(u,3)*x[3];
        yu = pow(1-u,3)*y[0]+3*u*pow(1-u,2)*y[1]+3*pow(u,2)*(1-u)*y[2]+pow(u,3)*y[3];

        setPixel(xu, yu, 255);
    }
}


// Inicializa o SDL, abre a janela e controla o loop
// principal do controle de eventos
int main()
{
    // Inicializações iniciais obrigatórias

    setlocale(LC_ALL, NULL);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow(titulo.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_RESIZABLE);

    window_surface = SDL_GetWindowSurface(window);

    pixels = (unsigned int *) window_surface->pixels;
    width = window_surface->w;
    height = window_surface->h;

    // Fim das inicializações

    printf("Pixel format: %s\n",SDL_GetPixelFormatName(window_surface->format->format));



     while (1)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(window);
                    pixels = (unsigned int *) window_surface->pixels;
                    width = window_surface->w;
                    height = window_surface->h;
                    printf("Size changed: %d, %d\n", width, height);
                }
            }

            // Se o mouse é movimentado
            if(event.type == SDL_MOUSEMOTION)
            {
                // Mostra as posições x e y do mouse
                showMousePosition(window,event.motion.x,event.motion.y);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
				/*Se o botão esquerdo do mouse é pressionado */
                if(event.button.button == SDL_BUTTON_LEFT)
				{
					printf("Mouse pressed on (%d,%d)\n",event.motion.x,event.motion.y) ;
				}
            }
        }

		bresenham_line(0, 0, width-1, 0);
		// right line
		bresenham_line(width-1, 0, width-1, height-1);
		// bottom line
		bresenham_line(width-1, height-1, 0, height-1);
		// left line
		bresenham_line(0, height-1, 0, 0);

		// draw a circle
		int xc = (width-1)/2, yc = (height-1)/2, r2 = 200;

        drawBresehamCircle(xc, yc, r2);

        int x[4]={281,130,220,305} , y[4]={188,144,260,338};
        int x1[4]={281,500,360,305} , y1[4]={188,144,260,338};
        bezier_curve(x, y);
        bezier_curve(x1, y1);

		floodFill(299, 250, RGB(245,11,11), 0);
		floodFill(302, 79, RGB(11,245,65), 0);
		floodFill(63, 63, RGB(247,200,8), 0);


        SDL_UpdateWindowSurface(window);
    }
}
