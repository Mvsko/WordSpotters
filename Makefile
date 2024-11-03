refine : main.c
	gcc -o refine main.c -lSDL2 -lSDL2_image -Wall -Wextra -g `sdl2-config --cflags --libs`

nn_nxor : NeuralNetwork/NeuralNetwork.c
	gcc -o nn_nxor NeuralNetwork/NeuralNetwork.c -Wextra -Wall -g -lSDL2 -lm

detection : Detection/Detection.c
	gcc -o detection Detection/Detection.c -lSDL2 -lSDL2_image -Wall -Wextra -g `sdl2-config --cflags --libs`

solver : Solver/solver.c
	gcc -o solver Solver/solver.c -Wall -Wextra -g

all: refine nn_nxor detection solver

cleanletters:
	find -type f -name 'letter*' -delete

.PHONY: clean

clean:
	rm -rf *.o
	rm refine
	rm nn_nxor
	rm detection
	rm solver
	find -type f -name 'out*' -delete
	find -type f -name 'letter*' -delete
	find -type d -name '*letter*' -delete
