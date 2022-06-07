CC=gcc

#linux
SDL_INCLUDE=/usr/include/SDL2
SDL_LIB=/usr/lib

# mac
SDL_INCLUDE_MAC=/usr/local/Cellar/sdl2/2.0.22/include/SDL2
SDL_LIB_MAC=/usr/local/Cellar/sdl2/2.0.22/lib

# windows
SDL_CONFIG=/mingw64/bin/
SDL_INCLUDE_WIN=/mingw64/include/SDL2
SDL_LIB_WIN=/mingw64/lib

all :
	[ ! -d bin/ ] && mkdir bin & ${CC} *.c -I${SDL_INCLUDE} -L${SDL_LIB} -lSDL2 -lm -o bin/nmplotter

win :
	[ ! -d bin/ ] && mkdir bin & ${CC} *.c -I${SDL_INCLUDE_WIN} -L${SDL_LIB_WIN} -lSDL2 -lm -o bin/nmplotter

mac :
	[ ! -d bin/ ] && mkdir bin & ${CC} *.c -I${SDL_INCLUDE_MAC} -L${SDL_LIB_MAC} -lSDL2 -lm -o bin/nmplotter