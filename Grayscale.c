#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <unistd.h>
#include <SDL2/SDL_pixels.h>

void Grayscale(SDL_Surface* image)
{
    Uint32 * pixels = (Uint32 *)image->pixels;

    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {
            Uint32 pixel = pixels[y * image->w + x];

            Uint8 r = pixel >> 16 & 0xFF;
            Uint8 g = pixel >> 8 & 0xFF;
            Uint8 b = pixel & 0xFF;

            //Uint8 v = (r + g + b) / 3;
            Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
            pixel = (0xFF << 24) | (v << 16) | (v << 8) | v;
            pixels[y * image->w + x] = pixel;
         }
    }
}
