#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <unistd.h>
#include <SDL2/SDL_pixels.h>

#include "Grayscale.c"

int main(int argc, char ** argv)
{
    int quit = 0;
    SDL_Event event;
    float frotate=0.0f;

    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    IMG_Init(IMG_INIT_JPG);

    SDL_Window * window = SDL_CreateWindow("SDL2 Grayscale",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface * image = IMG_Load(argv[1]);

    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Texture * texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
    image->w, image->h);


    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

    while (quit==0)
    {

        SDL_UpdateTexture(texture, NULL, image->pixels,
        image->w * sizeof(Uint32));

        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {

                case SDLK_g:
                    Grayscale(image);
                    break;
                case SDLK_r:
                    frotate += 0.5f;
                    break;
                }
                break;
        }
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, frotate, NULL,SDL_FLIP_NONE );
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
