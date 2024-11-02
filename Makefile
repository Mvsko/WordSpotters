detection :
	gcc -o detection Detection.c -lSDL2 -lSDL2_image -Wall -Wextra -g `sdl2-config --cflags --libs`
