#include "SdlFuncs.h"


/*** Initialization ***/

void SDL_Init (Application* App)
{
	App->Running = true;

	CreateWindow (App);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	CreateSurface (App);
}

void CreateWindow (Application* App)
{
	int WindowFlags = 0;

	App->Window = SDL_CreateWindow("Soviet Union", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, WindowFlags);

	if (!App->Window)
	{
		system ("clear");
		printf ("Failed to create %d x %d window:\n%s\n\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		App->Running = false;
	}
}

void CreateSurface (Application* App)
{
	App->Surface = SDL_GetWindowSurface (App->Window);

	if (!App->Surface)
	{
		printf ("\n\nCan't get surface from window:\n%s\n\n", SDL_GetError());
		assert (!"Surface from window");
	}
}


/*** Input ***/

void CheckInput (Application* App)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				App->Running = false;
				break;

			default:
				break;
		}
	}
}


/*** Working with Scene ***/

void PrepareScene (Application* App)
{
	SDL_SetRenderDrawColor (App->Renderer, 96, 128, 255, 255);		// Configures renderer to draw in blue
	SDL_RenderClear (App->Renderer);								// This function actually redraws window
}

void PresentScene (Application* App)
{
	SDL_RenderPresent (App->Renderer);								// But you can see changes only after calling 
																	// this function
}


/*** Drawing ***/

uint64_t GetColour (SDL_PixelFormat* fmt, uint64_t red, uint64_t green, uint64_t blue, uint64_t alpha)
{

	uint64_t colour = 0;

	colour += red   << fmt->Rshift;
	colour += green << fmt->Gshift;
	colour += blue  << fmt->Bshift;
	colour += alpha << 24;

	return colour;
}

void RedrawPixel (SDL_Surface* Surface, int x, int y, uint32_t value)
{
	uint32_t* pixel = (uint32_t*)((uint8_t*)Surface->pixels + y * Surface->pitch + x * Surface->format->BytesPerPixel);

	*pixel = value;
}