#include "main.h"
#include <SDL.h>

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

void plot()
{
    SDL_Event e;
    bool done = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) err(SDL, (char *)SDL_GetError());
    else {
        win = SDL_CreateWindow("nmplotter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
        renderer = renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

        while (!done)
        {
            // Clear and draw the screen (gives us a clean, black canvas)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            // Draw the grid lines
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, SDL_ALPHA_OPAQUE);
            for (int i = 10; i < 1000; i+=10) SDL_RenderDrawLine(renderer, i, 0, i, 600);
            for (int i = 10; i < 600; i+=10) SDL_RenderDrawLine(renderer, 0, i, 1000, i);

            SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
            for (int i = 50; i < 1000; i+=50) SDL_RenderDrawLine(renderer, i, 0, i, 600);
            for (int i = 50; i < 600; i+=50) SDL_RenderDrawLine(renderer, 0, i, 1000, i);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 500, 0, 500, 600);
            SDL_RenderDrawLine(renderer, 0, 300, 1000, 300);

            SDL_RenderPresent(renderer);
            SDL_Delay(5); // Comment this if you feel like cooking your meal on your graphics card

            while (SDL_PollEvent(&e) != 0)
                if (e.type == SDL_QUIT) done = true;
        }
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    win = NULL;
    renderer = NULL;
}