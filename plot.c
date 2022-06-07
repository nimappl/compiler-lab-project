#include "main.h"
#include <SDL.h>

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;

// Utilieses SDL to render recieved function's plot for -6 to 6 domain.
void plot()
{
    SDL_Event e;
    bool done = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) err(SDL, (char *)SDL_GetError());
    else {
        win = SDL_CreateWindow("Function Graph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_SHOWN);
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

            // Draw X and Y axis
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 500, 0, 500, 600);
            SDL_RenderDrawLine(renderer, 0, 300, 1000, 300);

            draw_numbers();
            SDL_SetRenderDrawColor(renderer, 50, 255, 255, SDL_ALPHA_OPAQUE);

            /*
                Go through -6 to +6 values, iterating 0.02 units each time, which would equivate to 1 pixel
                on the rendering surface. every 50 pixels are considered 1 arithmetic unit.
                Calculate the f value every iteration, save the current point, draw a line from previous point
                to the current, and save the current point for next iteration. for any value if the function
                is undefined, contiuous is set to false, so that at the next defined point we don't draw a line
                from previously valid point, since the function is undefined in between those points.
            */
            point previous, current;
            bool continuous = false;
            int j = 0; // for pixels across x-axis of the window
            for (double i = -10; i <= 10; i+=0.02)
            {
                token result = calculate_for(i, false, RADIAN);
                if (result.type != TT_NULL) {
                    if (result.value < 6 || result.value > -6) {
                        current.x = j;

                        if (result.value > 0) current.y = 300 - fabs(result.value) / 0.02;
                        else current.y = 300 + fabs(result.value) / 0.02;

                        if (continuous) SDL_RenderDrawLine(renderer, previous.x, previous.y, current.x, current.y);
                        previous = current;
                        continuous = true;
                    } else {
                        continuous = false;
                    }
                } else {
                    continuous = false;
                }

                j++;
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(10); // Comment this if you feel like cooking your meal on your graphics card

            while (SDL_PollEvent(&e) != 0)
                if (e.type == SDL_QUIT) done = true;
        }
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void draw_numbers()
{

    SDL_Rect src = { 0, 0, 10, 16 }, dest = { 0, 0, 10, 16 }, dest_on_canvas = { 0, 302, 33, 16 };
    int pos_difference = 100;

    ////////////// Draw numbers for x-axis //////////////
    for (int i = -10; i <= 10; i+=2)
    {
        SDL_Surface *image_sprite = SDL_LoadBMP("bmp/numbers.bmp");    // change to SDL_LoadBMP("..\\bmp\\numbers.bmp") for windows
        SDL_Surface *number = SDL_LoadBMP("bmp/empty.bmp");            // change to SDL_LoadBMP("..\\bmp\\empty.bmp") for windows
        if (!image_sprite || !number) err(SDL, (char*)SDL_GetError());

        int space = 0;
        if (i < 0) {
            src = (SDL_Rect){ 8, 0, 7, 16 };
            dest = (SDL_Rect){ 0, 0, 10, 16 };
            SDL_BlitSurface(image_sprite, &src, number, &dest);
            space = 8;
        }

        src.w = 10;

        switch (abs(i))
        {
            case 0: src.x = 32; break;
            case 2: src.x = 54; break;
            case 4: src.x = 75; break;
            case 6: src.x = 97; break;
            case 8: src.x = 118; break;
            case 10:
                src.x = 43; src.w = 7; dest.x = space;
                SDL_BlitSurface(image_sprite, &src, number, &dest);
                src.x = 32; space += 8;
                break;
        }

        src.w = 10; dest.x = space;
        dest_on_canvas.x = i == -10 ? 0 : pos_difference - (12 + space);
        pos_difference = i == -10 ? 100 : pos_difference + 100;

        SDL_BlitSurface(image_sprite, &src, number, &dest);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, number);
        SDL_RenderCopy(renderer, texture, NULL, &dest_on_canvas);
        
        SDL_FreeSurface(image_sprite);
        SDL_FreeSurface(number);
    }

    ////////////// Draw numbers for y-axis //////////////
    dest_on_canvas = (SDL_Rect){ 500, 0, 33, 16 };
    for (int i = 6; i >= -6; i-=2)
    {
        if (i == 0) {dest_on_canvas.y += 100; continue; }
        int dist = 500;
        SDL_Surface *image_sprite = SDL_LoadBMP("bmp/numbers.bmp");    // change to SDL_LoadBMP("..\\bmp\\numbers.bmp") for windows
        SDL_Surface *number = SDL_LoadBMP("bmp/empty.bmp");            // change to SDL_LoadBMP("..\\bmp\\empty.bmp") for windows
        if (!image_sprite || !number) err(SDL, (char*)SDL_GetError());

        int space = 0;
        if (i < 0) {
            src = (SDL_Rect){ 8, 0, 7, 16 };
            dest = (SDL_Rect){ 0, 0, 10, 16 };
            SDL_BlitSurface(image_sprite, &src, number, &dest);
            space = 8;
        }

        src.w = 10;

        switch (abs(i))
        {
            case 2: src.x = 54; break;
            case 4: src.x = 75; break;
            case 6: src.x = 97; break;
        }

        src.w = 10; dest.x = space;
        dest_on_canvas.x = dist - (12 + space);

        SDL_BlitSurface(image_sprite, &src, number, &dest);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, number);
        SDL_RenderCopy(renderer, texture, NULL, &dest_on_canvas);
        dest_on_canvas.y = i == -4 ? dest_on_canvas.y + 83 : dest_on_canvas.y + 100;
        
        SDL_FreeSurface(image_sprite);
        SDL_FreeSurface(number);
    }
}