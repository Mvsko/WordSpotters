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

            //Convert pixel in grayscale:
            // Each pixel is a 32-bit integer
            // [Alpha-Red-Green-Blue]
            // [8bit-8bit-8bit-8bit]

            // Supression of the Alpha with & 0xFF == 255
            Uint8 r = pixel >> 16 & 0xFF;
            Uint8 g = pixel >> 8 & 0xFF;
            Uint8 b = pixel & 0xFF;

            // Use of the formula for grayscale
            Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
            // Opposite method for the extraction of rgb
            pixel = (0xFF << 24) | (v << 16) | (v << 8) | v;
            pixels[y * image->w + x] = pixel;
         }
    }
}

void Toblackwhite (SDL_Surface* image)
{
    Uint32 * pixels = (Uint32 *)image->pixels;

    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w ; x++)
        {
            Uint32 pixel = pixels[y * image->w + x];
            Uint8 r = pixel >> 16 & 0xFF;
            Uint8 g = pixel >> 8 & 0xFF;
            Uint8 b = pixel & 0xFF;
            Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
            if(v < 127.5)
            {
                pixel = (0xFF << 24) | (0 << 16) | (0 << 8) | 0;
                pixels[y * image->w + x] = pixel;
            }
            else
            {
                pixel = (0xFF << 24) | (255 << 16) | (255 << 8) | 255;
                pixels[y * image->w + x] = pixel;
            }
        }
    }
}

Uint8 f(Uint8 c, int n)
{
    if(c==255|| c + n > 127)
        return 255;

    else if(c + n < 0)
        return 0;
    else
        return c + n;
}

void Brightness(SDL_Surface * image, int n)
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

            r = f(r,n);
            g = f(g,n);
            b = f(b,n);

            pixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
            pixels[y * image->w + x] = pixel;
         }
    }
}

void denoiseImage(SDL_Surface *image)
{
    int width = image->w;
    int height = image->h;

    Uint32 *pixels = (Uint32 *)image->pixels;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            //The pixel that will be modified
            Uint32 pixelcurrent = pixels[y * width + x];
            int r = 0, g = 0, b = 0;
            // Average of neighboring pixels
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    Uint32 pixel = pixels[(y + j) * width + (x + i)];
                    SDL_Color color;
                    SDL_GetRGBA(pixel, image->format, &color.r, &color.g, &color.b, &color.a);
                    r += color.r;
                    g += color.g;
                    b += color.b;
                }
            }
            r /= 9;
            g /= 9;
            b /= 9;
            // Replaces pixel colors with the average 
            pixelcurrent = (0xFF << 24) | (r << 16) | (g << 8) | b;
            pixels[y * width + x] = pixelcurrent;
        }
    }
}
