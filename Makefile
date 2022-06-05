CC=gcc
SDL_CONFIG=/mingw64/bin/
SDL_INCLUDE=/mingw64/include/SDL2
SDL_LIB=/mingw64/lib

all :
	[ ! -d bin/ ] && mkdir bin
	# ${CC} *.c -I${SDL_INCLUDE} -L{SDL_LIB} `${SDL_CONFIG} --cflags --libs` -lSDL2 -o bin/nmparser
	${CC} *.c -lm -o bin/nmparser

win:
	[ ! -d bin/ ] && mkdir bin
	# ${CC} *.c -mwindows -I${SDL_INCLUDE} -L{SDL_LIB} `${SDL_CONFIG} --cflags --libs` -lSDL2 -o bin/nmparser.exe
	${CC} *.c -mwindows -lm -o bin/nmparser.exe