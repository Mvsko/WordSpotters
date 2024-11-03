#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <unistd.h>
#include <SDL2/SDL_pixels.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Tools/Save.c"

//Step 1: List of character positions
int **initList(int n)
{
    int **list = malloc(n * sizeof(size_t));
    return list;
}

int isInList(int **list, int *listSize, int x1, int y1, int x2, int y2, int size)
{
    for (int i = 0; i < *listSize; i++)
    {
        if (list[i][0] == 0 || (list[i][0] == x1 && list[i][1] == y1 && list[i][2] == x2 && list[i][3] == y2 && list[i][4] == size))
            return -1;
    }
    return 0;
}

void addToList(int **list,int *listSize,int x1,int y1,int x2,int y2,int size)
{
    if (isInList(list,listSize,x1,y1,x2,y2,size) == -1)
    {
        return;
    }
    *listSize = (*listSize) + 1;
    list[*listSize - 1] = calloc(5, sizeof(size_t));
    list[*listSize - 1][0] = x1;
    list[*listSize - 1][1] = y1;
    list[*listSize - 1][2] = x2;
    list[*listSize - 1][3] = y2;
    list[*listSize - 1][4] = size;
}

void freeList(int **list,size_t height)
{
    for (size_t i = 0; i < height; i++)
    {
        free(list[i]);
    }
    free(list);
}

//Step 2: Function to draw the found characters
void draw_rectangle(SDL_Surface* surface, int x, int y, int x2, int y2)
{
    int width = x2;
    int height = y2;
    Uint32 * pixels = (Uint32 *)surface->pixels;
    for (int dy = y; dy < height; dy++)
    {
        Uint32 pixel = (0xFF << 24) | (255 << 16) | (100 << 8) | 0;
        pixels[(dy) * surface->w + x] = pixel;
    }
    for (int dx = x; dx < width; dx++)
    {
        Uint32 pixel = (0xFF << 24) | (255 << 16) | (100 << 8) | 0;
        pixels[y * surface->w + dx] = pixel;
    }
    for (int dy = y; dy < height; dy++)
    {
        Uint32 pixel = (0xFF << 24) | (255 << 16) | (100 << 8) | 0;
        pixels[(dy) * surface->w + x2] = pixel;
    }
    for (int dx = x; dx < width; dx++)
    {
        Uint32 pixel = (0xFF << 24) | (255 << 16) | (100 << 8) | 0;
        pixels[(y2) * surface->w + dx] = pixel;
    }
}

//Step 3: Getting the number of characters (mandatory for list management)
void getCharNb(SDL_Surface* image,int x, int y, int* listSize)
{
    Uint32 * pixels = (Uint32 *)image->pixels;
    int x1 = x;
    int y1 = y;
    int x2 = x+1;
    int y2 = y+1;
    int lastx1 = -1;
    int lasty1 = -1;
    int lastx2 = -1;
    int lasty2 = -1;
    while (x1 != lastx1 || y1 != lasty1 || x2 != lastx2 || y2 != lasty2)
    {
        lastx1 = x1;
        lasty1 = y1;
        lastx2 = x2;
        lasty2 = y2;
        if(x1 > 0)
        {
            int cont = 0;
            int i = y1;
            while (i < y2 && cont == 0)
            {
                Uint32 pixel = pixels[i * image->w + x1];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(i) * image->w + x1 + 1];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > y1 && i < y2-1)
                {
                    pixel = pixels[(i-1) * image->w + x1 + 1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(i+1) * image->w + x1 + 1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    x1--;
                    cont = 1;
                }
                i++;
            }
        }
        if(x2+1 < image->w)
        {
            int cont = 0;
            int i = y1;
            while (i < y2 && cont == 0)
            {
                Uint32 pixel = pixels[i * image->w + x2];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(i) * image->w + x2 - 1];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > y1 && i < y2-1)
                {
                    pixel = pixels[(i-1) * image->w + x2 - 1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(i+1) * image->w + x2 - 1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    x2++;
                    cont = 1;
                }
                i++;
            }
        }
        if(y1 > 0)
        {
            int cont = 0;
            int i = x1;
            while (i < x2 && cont == 0)
            {
                Uint32 pixel = pixels[y1 * image->w + i];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(y1+1) * image->w + i];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > x1 && i < x2-1)
                {
                    pixel = pixels[(y1+1) * image->w + i-1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(y1+1) * image->w + i+1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    y1--;
                    cont = 1;
                }
                i++;
            }
        }
        if(y2+1 < image->h)
        {
            int cont = 0;
            int i = x1;
            while (i < x2 && cont == 0)
            {
                Uint32 pixel = pixels[y2 * image->w + i];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(y2-1) * image->w + i];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > x1 && i < x2-1)
                {
                    pixel = pixels[(y2-1) * image->w + i-1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(y2-1) * image->w + i+1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    y2++;
                    cont = 1;
                }
                i++;
            }
        }
    }
    *listSize = (*listSize) +1;
}

//Step 4: Getting the position of each character and putting it in the list
void getCharPos(SDL_Surface* image,int x, int y, int** list, int* listSize)
{
    Uint32 * pixels = (Uint32 *)image->pixels;
    int x1 = x;
    int y1 = y;
    int x2 = x+1;
    int y2 = y+1;
    int lastx1 = -1;
    int lasty1 = -1;
    int lastx2 = -1;
    int lasty2 = -1;
    while (x1 != lastx1 || y1 != lasty1 || x2 != lastx2 || y2 != lasty2)
    {
        lastx1 = x1;
        lasty1 = y1;
        lastx2 = x2;
        lasty2 = y2;
        if(x1 > 0)
        {
            int cont = 0;
            int i = y1;
            while (i < y2 && cont == 0)
            {
		Uint32 pixel = pixels[i * image->w + x1];
            	Uint8 r = pixel >> 16 & 0xFF;
            	pixel = pixels[(i) * image->w + x1 + 1];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > y1 && i < y2-1)
                {
                    pixel = pixels[(i-1) * image->w + x1 + 1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(i+1) * image->w + x1 + 1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    x1--;
                    cont = 1;
                }
                i++;
            }
        }
        if(x2+1 < image->w)
        {
            int cont = 0;
            int i = y1;
            while (i < y2 && cont == 0)
            {
                Uint32 pixel = pixels[i * image->w + x2];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(i) * image->w + x2 - 1];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > y1 && i < y2-1)
                {
                    pixel = pixels[(i-1) * image->w + x2 - 1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(i+1) * image->w + x2 - 1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    x2++;
                    cont = 1;
                }
                i++;
            }
        }
        if(y1 > 0)
        {
            int cont = 0;
            int i = x1;
            while (i < x2 && cont == 0)
            {
                Uint32 pixel = pixels[y1 * image->w + i];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(y1+1) * image->w + i];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > x1 && i < x2-1)
                {
                    pixel = pixels[(y1+1) * image->w + i-1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(y1+1) * image->w + i+1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    y1--;
                    cont = 1;
                }
                i++;
            }
        }
        if(y2+1 < image->h)
        {
            int cont = 0;
            int i = x1;
            while (i < x2 && cont == 0)
            {
                Uint32 pixel = pixels[y2 * image->w + i];
                Uint8 r = pixel >> 16 & 0xFF;
                pixel = pixels[(y2-1) * image->w + i];
                Uint8 r2 = pixel >> 16 & 0xFF;
                Uint8 r3 = 0;
                Uint8 r4 = 0;
                if (i > x1 && i < x2-1)
                {
                    pixel = pixels[(y2-1) * image->w + i-1];
                    r3 = pixel >> 16 & 0xFF;
                    pixel = pixels[(y2-1) * image->w + i+1];
                    r4 = pixel >> 16 & 0xFF;
                }
                if(r < 127.5 && (r2 < 127.5 || r3 < 127.5 || r4 < 127.5))
                {
                    y2++;
                    cont = 1;
                }
                i++;
            }
        }
    }
    addToList(list,listSize,x1,y1,x2,y2,(x2-x1)*(y2-1));
}

//Step 5: Cutting the found letters into individual images
void cut(SDL_Surface* image, int** list,int listSize)
{
    system("mkdir -p letters_extracted");
    for (int i = 0; i < listSize; i++)
    {
        int x_pos = list[i][0];
        int y_pos = list[i][1];
        int wwidth = list[i][2] - list[i][0];
        int wheight = list[i][3] - list[i][1];
        SDL_Rect cell_rect = { x_pos, y_pos, wwidth, wheight };
        SDL_Surface *cell_surface = SDL_CreateRGBSurface(0, wwidth, wheight, image->format->BitsPerPixel,
                                                                       image->format->Rmask, image->format->Gmask,
                                                                       image->format->Bmask, image->format->Amask);
        if(!cell_surface)
        {
            return;
        }
        SDL_BlitSurface(image, &cell_rect, cell_surface, NULL);
        char filename[listSize];
        snprintf(filename, sizeof(filename), "letters_extracted/letter_%d_%d.png", x_pos, y_pos);
        IMG_SavePNG(cell_surface, filename);
        //printf("Cell [%d, %d] saved in %s\n", x, y, filename);
        SDL_FreeSurface(cell_surface);
    }
    //freeList(list,listSize);
}

//Step 6: main loop
void detectionLoop(SDL_Surface* image)
{
    Uint32 * pixels = (Uint32 *)image->pixels;
    int listSize = 0;
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w ; x++)
        {
            Uint32 pixel = pixels[y * image->w + x];
            Uint8 r = pixel >> 16 & 0xFF;
            if(r < 127.5)
            {
                getCharNb(image,x,y,&listSize);
            }
        }
    }
    int **list = initList(listSize);
    int size2 = 0;
    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w ; x++)
        {
            Uint32 pixel = pixels[y * image->w + x];
            Uint8 r = pixel >> 16 & 0xFF;
            if(r < 127.5)
            {
                getCharPos(image,x,y,list,&size2);
            }
        }
    }
    /*int averageSize = 0;
    for (int i = 0; i < size2; i++)
        if (list[i][0] != 0)
        {
            averageSize += list[i][4];
        }
    averageSize = averageSize / size2;*/
    cut(image,list,size2);
    for (int i = 0; i < size2; i++)
        if (list[i][0] != 0 /*&& averageSize/2 <= list[i][4] && 2*averageSize >= list[i][4]*/)
	{
            draw_rectangle(image,list[i][0],list[i][1],list[i][2],list[i][3]);
        }
    freeList(list,listSize);
}

int main(int argc, char ** argv)
{
    int quit = 0;
    SDL_Event event;
    double frotate=0;

    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./detection <file name>");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    IMG_Init(IMG_INIT_JPG);

    SDL_Window * window = SDL_CreateWindow("SDL2 Detection",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);

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
                case SDLK_d:
                    detectionLoop(image);
                    break;
                case SDLK_s:
                    if(IMG_SavePNG(image, "Pictures/out.png")==1)
                    {
                        errx(EXIT_FAILURE, "%s", SDL_GetError());
                    }
                }
                break;
        }
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, frotate, NULL,SDL_FLIP_NONE );
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
