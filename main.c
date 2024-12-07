#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include <SDL2/SDL_pixels.h>
#include <time.h>
#include <stdlib.h>

#include "Preparation/Treatment.c"
#include "Tools/Save.c"

int main(int argc, char ** argv)
{
    //Variables
    int quit = 0;
    SDL_Event event;
    double frotate=0;
    // Set size for main window
    int mainSizeX = 600;
    int mainSizeY = 600;
    // Set the size for sub window based on those of main window
    int subSizeX = mainSizeX / 2;
    int subSizeY = mainSizeY / 2;

    // Argument Tests
    if (argc < 2)
        errx(EXIT_FAILURE, "Parameters Issue :  image-file && Angle(NULL) ");
    if(argc >=3)
       frotate +=atoi(argv[2]);

    // Initialisation of SDL
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    IMG_Init(IMG_INIT_JPG);

    //Set up main window
    SDL_Window * mainWindow = SDL_CreateWindow("Main Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mainSizeX, mainSizeY, 0);
    if(mainWindow == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
    if (mainRenderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_SetRenderDrawColor( mainRenderer , 0, 0, 0, 0 );

    //Set up sub window
    SDL_Window* subWindow  = SDL_CreateWindow( "Sub Window" ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, subSizeX, subSizeY, 0 );
    SDL_Renderer* subRenderer  = SDL_CreateRenderer( subWindow, -1, 0 );
    SDL_SetRenderDrawColor( subRenderer , 255, 255, 255, 255 );

    //Import the Picture
    SDL_Surface * image = IMG_Load(argv[1]);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Texture * texture = SDL_CreateTexture(subRenderer,
    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
    image->w, image->h);


    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);
    SDL_Point mouse_coords;
    SDL_Rect Texture_dst_rect = {0, 0, 200, 100};

    while (quit==0)
    {
        SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
        //Update the image
        SDL_UpdateTexture(texture, NULL, image->pixels,
        image->w * sizeof(Uint32));
        SDL_SetRenderDrawColor(subRenderer,255,255,255,255);
        SDL_RenderClear(subRenderer);
        SDL_RenderCopyEx(subRenderer, texture, NULL, NULL, frotate, NULL,SDL_FLIP_NONE );
        SDL_RaiseWindow(subWindow);

        // SDL EVENT Manager
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE){
                    quit = 1;
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
	        {
		    if (SDL_PointInRect(&mouse_coords, &Texture_dst_rect))
		    {
                        frotate+=0.5f;
                        break;
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_g:
                    Grayscale(image);
                    break;
                case SDLK_b:
                    Toblackwhite(image);
                    break;
                case SDLK_l:
                    Brightness(image, -63);
                    break;
                case SDLK_d:
                    denoiseImage(image);
                    Brightness(image,-63);
                    break;
                case SDLK_r:
                    frotate += 0.5;
                    break;
                case SDLK_s:
                    SDL_Surface * screenshot = SDL_CreateRGBSurfaceWithFormat(0, subSizeX, subSizeY, 32, SDL_PIXELFORMAT_ARGB8888);
                    SDL_RenderReadPixels(subRenderer, NULL, SDL_PIXELFORMAT_ARGB8888,screenshot->pixels, screenshot->pitch);
                    if(IMG_SavePNG(screenshot, "Pictures/out.png")==1)
                    {
                        errx(EXIT_FAILURE, "%s", SDL_GetError());
                    }
                    SDL_FreeSurface(screenshot);
                    break;
                }
                break;
        }
        SDL_RenderPresent(subRenderer);
        SDL_RenderPresent(mainRenderer);
    }
    //SDL DESTRUCTION PROCESS
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(subRenderer);
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(subWindow);
    SDL_DestroyWindow(mainWindow);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
