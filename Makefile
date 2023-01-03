CC=gcc

#linux
SDL_INCLUDE=/usr/include/SDL2
SDL_LIB=/usr/lib

# windows
SDL_CONFIG=/mingw64/bin/
SDL_INCLUDE_WIN=/mingw64/include/SDL2
SDL_LIB_WIN=/mingw64/lib

all :
	[ ! -d bin/ ] && mkdir bin & ${CC} *.c -I${SDL_INCLUDE} -L${SDL_LIB} -lSDL2 -lm -o bin/nmplot

win :
	[ ! -d bin/ ] && mkdir bin & ${CC} *.c -I${SDL_INCLUDE_WIN} -L${SDL_LIB_WIN} -lSDL2 -lm -o bin/math_parser
