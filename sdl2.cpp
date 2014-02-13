#include <SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
void close();
SDL_Window* gWindow = 0;
SDL_Renderer* gRenderer = 0;

bool init()
{
	bool success = 1;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL initialization failed. SDL Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            printf( "Warning: Linear texture filtering not enabled!" );
		gWindow = SDL_CreateWindow( "SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == 0 )
		{
			printf( "Window creation failed. SDL Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == 0 )
			{
				printf( "Renderer creation failed. SDL Error: %s\n", SDL_GetError() );
				success = 0;
			}
		}
	}
	return success;
}

void close()
{
    SDL_DestroyWindow( gWindow );
	gWindow = 0;
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	if( init() )
	{
        bool quit = 0;
        SDL_Event e;
        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT || ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE ) )
                    quit = 1;
            }
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );
            SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            SDL_SetRenderDrawColor( gRenderer, 0x88, 0x00, 0x88, 0xFF );
            SDL_RenderFillRect( gRenderer, &fillRect );
            SDL_RenderPresent( gRenderer );
        }
	}
	close();
	return 0;
}
