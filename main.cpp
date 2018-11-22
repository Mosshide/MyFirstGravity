#include "SDL\SDL.h"
#include "SDL\SDL_image.h"
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>
using namespace std;

//timers 
Uint32 sdltimer;
Uint32 dsdltimer;

bool quit;

//keys
bool adown;
bool ddown;
bool spacedown;

//screen values
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//sdl stuff
SDL_Event event;

//object coords
float x = 10.0;
float y = SCREEN_HEIGHT - 73.0;

bool grounded = true;
float v = 0.0;
float a = -.1;

SDL_Surface *screen = NULL;
SDL_Surface *girl = NULL;
SDL_Surface *girlih = NULL;

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        //optimizedImage = SDL_DisplayFormat( loadedImage );
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
        
        //If the image was optimized just fine
        /*if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 255, 56, 255 );
            
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }*/
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Move Test.NOT", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
	girl = load_image( "img/girl.png" );
	girlih = load_image( "img/girlih.png" );
    
    //If everything loaded fine
    return true;    
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( girl );
    SDL_FreeSurface( girlih );

    //Quit SDL
    SDL_Quit();
}	


int main( int argc, char* args[] ) {
	init();
	load_files();
	
	while(quit == false){
		while( SDL_PollEvent( &event ) )
        {
			//If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
            
            if( event.type == SDL_KEYDOWN )
            {
				switch( event.key.keysym.sym )
                {
					case SDLK_ESCAPE: 
						quit = true;
                    	break;
                    	
                    case SDLK_a: 
						adown = true;
                    	break;
                    
                    case SDLK_d: 
						ddown = true;
                    	break;
                    	
                    case SDLK_SPACE: 
						spacedown = true;
                    	break;
				}
			}
			if( event.type == SDL_KEYUP )
            {
				switch( event.key.keysym.sym )
                {
                    case SDLK_a: 
						adown = false;
                    	break;
                    
                    case SDLK_d: 
						ddown = false;
                    	break;
                    	
                    case SDLK_SPACE: 
						spacedown = false;
                    	break;
				}
			}
		}
		
		dsdltimer = SDL_GetTicks() - sdltimer;
		sdltimer = SDL_GetTicks();
        
		if ( adown ){
			x -= dsdltimer/2.0;
		}
		if ( ddown ){
			x += dsdltimer/2.0;
		}
		
		if ( spacedown && grounded ){
			grounded = false;
			v = 7.0;
		}
		v += a*dsdltimer/4.0;
		y -= v*dsdltimer/4.0;

		if ( y > SCREEN_HEIGHT - 73.0 ){
			y = SCREEN_HEIGHT - 73.0;
			v = 0;
			grounded = true;
		} 
		
		if ( x < 0.0 ) x = 0.0;
		if ( x > SCREEN_WIDTH - 257.0 ) x = SCREEN_WIDTH - 257.0;
		
	        
	    //rendering
	    apply_surface( 0, 0, girl, screen );
	    apply_surface( int(x), int(y), girlih, screen );
		
		SDL_Flip( screen );
	}
	
	clean_up();
	return 0;
	
}
