#include <SDL2/SDL.h>
#include <assert.h>


struct Application
{
	bool Running;
	SDL_Window* Window;
	SDL_Surface* Surface;
	SDL_Renderer* Renderer;
};

const int WINDOW_WIDTH  = 1280;
const int WINDOW_HEIGHT = 720;



/*** Initialization ***/

void SDL_Init (Application* App);
void CreateWindow  (Application* App);
void CreateSurface (Application* App);


/*** Input ***/

void CheckInput (Application* App);


/*** Working with Scene ***/

void PrepareScene (Application* App);

void PresentScene (Application* App);


/*** Drawing ***/

uint64_t GetColour (SDL_PixelFormat* fmt, uint64_t red, uint64_t green, uint64_t blue, uint64_t alpha);

void RedrawPixel (SDL_Surface* Surface, int x, int y, uint32_t value);