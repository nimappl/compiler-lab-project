CC=gcc
SDL_CONFIG=/mingw64/bin/
SDL_INCLUDE=/mingw64/include/SDL2
SDL_LIB=/mingw64/lib

[ ! -d bin/ ] && mkdir bin

all :
	# ${CC} *.c -mwindows -I${SDL_INCLUDE} -L{SDL_LIB} `${SDL_CONFIG} --cflags --libs` -lSDL2 -o main.exe
	${CC} *.c -lm -o bin/main.exe


