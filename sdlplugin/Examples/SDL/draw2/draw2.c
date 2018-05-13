/*
  CCTools SDL plugin example

  Derived from testdraw2:

  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

/* Simple program:  draw as many random objects on the screen as possible */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>

#define NUM_OBJECTS 100

static int num_objects;
static SDL_bool cycle_color;
static SDL_bool cycle_alpha;
static int cycle_direction = 1;
static int current_alpha = 255;
static int current_color = 255;
static SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;

void
DrawPoints(SDL_Renderer * renderer)
{
    int i;
    int x, y;
    SDL_Rect viewport;

    /* Query the sizes */
    SDL_RenderGetViewport(renderer, &viewport);

    for (i = 0; i < num_objects * 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        x = rand() % viewport.w;
        y = rand() % viewport.h;
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void
DrawLines(SDL_Renderer * renderer)
{
    int i;
    int x1, y1, x2, y2;
    SDL_Rect viewport;

    /* Query the sizes */
    SDL_RenderGetViewport(renderer, &viewport);

    for (i = 0; i < num_objects; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        if (i == 0) {
            SDL_RenderDrawLine(renderer, 0, 0, viewport.w - 1, viewport.h - 1);
            SDL_RenderDrawLine(renderer, 0, viewport.h - 1, viewport.w - 1, 0);
            SDL_RenderDrawLine(renderer, 0, viewport.h / 2, viewport.w - 1, viewport.h / 2);
            SDL_RenderDrawLine(renderer, viewport.w / 2, 0, viewport.w / 2, viewport.h - 1);
        } else {
            x1 = (rand() % (viewport.w*2)) - viewport.w;
            x2 = (rand() % (viewport.w*2)) - viewport.w;
            y1 = (rand() % (viewport.h*2)) - viewport.h;
            y2 = (rand() % (viewport.h*2)) - viewport.h;
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

void
DrawRects(SDL_Renderer * renderer)
{
    int i;
    SDL_Rect rect;
    SDL_Rect viewport;

    /* Query the sizes */
    SDL_RenderGetViewport(renderer, &viewport);

    for (i = 0; i < num_objects / 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        rect.w = rand() % (viewport.h / 2);
        rect.h = rand() % (viewport.h / 2);
        rect.x = (rand() % (viewport.w*2) - viewport.w) - (rect.w / 2);
        rect.y = (rand() % (viewport.h*2) - viewport.h) - (rect.h / 2);
        SDL_RenderFillRect(renderer, &rect);
    }
}

int
main(int argc, char *argv[])
{
    int done;
    SDL_Event event;
    Uint32 then, now, frames;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_DisplayMode mode;

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Initialize parameters */
    num_objects = NUM_OBJECTS;

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
	return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);

    /* Create window and renderer for given surface */
    window = SDL_CreateWindow("Draw2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w, mode.h, SDL_WINDOW_SHOWN);
    if(!window)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n",SDL_GetError());
	return 1;
    }	
    surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(surface);
    if(!renderer)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n",SDL_GetError());
	return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, blendMode);
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
    SDL_RenderClear(renderer);

    srand((unsigned int)time(NULL));

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    while (!done) {
        /* Check for events */
        ++frames;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
                done = 1;
            }

            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_Log("update screen size");
                SDL_DestroyRenderer(renderer);
                surface = SDL_GetWindowSurface(window);
                renderer = SDL_CreateSoftwareRenderer(surface);
                if(!renderer) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n",SDL_GetError());
                    done = 1;
                    break;
                }
                SDL_SetRenderDrawBlendMode(renderer, blendMode);
                SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
                SDL_RenderClear(renderer);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderClear(renderer);

        DrawRects(renderer);
        DrawLines(renderer);
        DrawPoints(renderer);

        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        double fps = ((double) frames * 1000) / (now - then);
        SDL_Log("%2.2f frames per second\n", fps);
    }

    SDL_Quit();

    return 0;
}
