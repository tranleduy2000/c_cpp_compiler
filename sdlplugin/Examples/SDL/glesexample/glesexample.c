/*
  CCTools SDL plugin GLES example

  Derived from SDL testgles sources:

  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_opengles.h>

static SDL_Window *window;
static SDL_GLContext context;

static void
AspectAdjust(int w, int h)
{
    float aspectAdjust;

    aspectAdjust = (4.0f / 3.0f) / ((float)w / h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(-2.0, 2.0, -2.0 * aspectAdjust, 2.0 * aspectAdjust, -20.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
}

static void
Render()
{
    static GLubyte color[8][4] = { {255, 0, 0, 0},
    {255, 0, 0, 255},
    {0, 255, 0, 255},
    {0, 255, 0, 255},
    {0, 255, 0, 255},
    {255, 255, 255, 255},
    {255, 0, 255, 255},
    {0, 0, 255, 255}
    };
    static GLfloat cube[8][3] = { {0.5, 0.5, -0.5},
    {0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {-0.5f, -0.5f, 0.5f}
    };
    static GLubyte indices[36] = { 0, 3, 4,
        4, 5, 0,
        0, 5, 6,
        6, 1, 0,
        6, 7, 2,
        2, 1, 6,
        7, 4, 3,
        3, 2, 7,
        5, 4, 7,
        7, 6, 5,
        2, 3, 1,
        3, 0, 1
    };


    /* Do our drawing, too. */
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Draw the cube */
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

    glMatrixMode(GL_MODELVIEW);
    glRotatef(5.0, 1.0, 1.0, 1.0);
}

int
main(int argc, char *argv[])
{
    int done;
    SDL_DisplayMode mode;
    SDL_Event event;
    Uint32 then, now, frames;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { /* Initialize SDL's Video subsystem */
        SDL_Log("Unable to initialize SDL");
        return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);

    // Create our window centered
    window = SDL_CreateWindow("GLES example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if (!window) {
	// Die if creation failed
	SDL_Log("Unable to create window");
	return 1;
    }

    // Create our opengl context and attach it to our window
    context = SDL_GL_CreateContext(window);
    if (!context) {
	SDL_Log("Unable to create GL context");
	return 1;
    }

    SDL_GL_MakeCurrent(window, context);

    SDL_GL_SetSwapInterval(1);

    SDL_Log("Screen bpp: %d\n", SDL_BITSPERPIXEL(mode.format));
    SDL_Log("\n");
    SDL_Log("Vendor     : %s\n", glGetString(GL_VENDOR));
    SDL_Log("Renderer   : %s\n", glGetString(GL_RENDERER));
    SDL_Log("Version    : %s\n", glGetString(GL_VERSION));
    SDL_Log("Extensions : %s\n", glGetString(GL_EXTENSIONS));
    SDL_Log("\n");

    AspectAdjust(mode.w, mode.h);

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    while (!done) {
        /* Check for events */
        ++frames;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
                    done = 1;
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        /* Change view port to the new window dimensions */
                        AspectAdjust(event.window.data1, event.window.data2);
                        /* Update window content */
                        Render();
                        SDL_GL_SwapWindow(window);
                        break;
                }
            }
        }

        Render();
        SDL_GL_SwapWindow(window);
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        SDL_Log("%2.2f frames per second\n",
               ((double) frames * 1000) / (now - then));
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
