/*
 * SDL gles2 example for CCTools SDL plugin
 *
 * http://cctools.info
 *
 * Based on ARM GLES2 emulator cube example
 *
 */

#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include <SDL.h>
#include <SDL_opengles2.h>

#include "shader.h"
#include "matrix.h"

typedef struct {
    float x, y, z;
} vec3;

#include "cube.inc"

SDL_DisplayMode	mode;
SDL_Window	*window;
SDL_GLContext	context;

GLfloat
    norm_x    =  0.0,
    norm_y    =  0.0,
    offset_x  =  0.0,
    offset_y  =  0.0,
    p1_pos_x  =  0.0,
    p1_pos_y  =  0.0;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { /* Initialize SDL's Video subsystem */
        SDL_Log("Unable to initialize SDL");
        return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

    // Create our window centered
    window = SDL_CreateWindow("Simple texture moving", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

   ///////  the openGL part  ///////////////////////////////////////////////////////////////

    GLint iLocPosition = 0;

    GLint iLocColour, iLocMVP;

    int iXangle = 0, iYangle = 0, iZangle = 0;

    float aRotate[16], aModelView[16], aPerspective[16], aMVP[16];

    GLuint vertexShader = -1;
    GLuint fragmentShader = -1;

    if (process_shader(&vertexShader, "shaders/shader.vert", GL_VERTEX_SHADER)) {
	SDL_Log("Unable load vertex shader");
	return 1;
    }

    if (process_shader(&fragmentShader, "shaders/shader.frag", GL_FRAGMENT_SHADER)) {
	SDL_Log("Unable load fragment shader");
	return 1;
    }

    GLuint shaderProgram  = glCreateProgram ();                 // create program object
    glAttachShader ( shaderProgram, vertexShader );             // and attach both...
    glAttachShader ( shaderProgram, fragmentShader );           // ... shaders to it

    glLinkProgram ( shaderProgram );    // link the program
    glUseProgram  ( shaderProgram );    // and select it for usage

    /* Get attribute locations of non-fixed attributes like colour and texture coordinates. */
    iLocPosition = glGetAttribLocation(shaderProgram, "av4position");
    iLocColour = glGetAttribLocation(shaderProgram, "av3colour");
    iLocMVP = glGetUniformLocation(shaderProgram, "mvp");

    if ( iLocPosition < 0  ||  iLocColour < 0  ||  iLocMVP < 0 ) {
	SDL_Log("Unable to get uniform location");
	return 1;
    }

    glEnableVertexAttribArray(iLocPosition);
    glEnableVertexAttribArray(iLocColour);

    /* Populate attributes for position, colour and texture coordinates etc. */
    glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, aVertices);
    glVertexAttribPointer(iLocColour, 3, GL_FLOAT, GL_FALSE, 0, aColours);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glViewport ( 0 , 0 , mode.w , mode.h );

    //// this is needed for time measuring  -->  frames per second
    struct  timezone  tz;
    struct timeval  t1, t2;
    gettimeofday ( &t1 , &tz );
    int  num_frames = 0;

    int quit = 0;
    while ( !quit ) {    // the main loop
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
	    switch (event.type) {
	    case SDL_KEYDOWN:
		if (event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
		    quit = 1;
		}
		break;
	    }
	}

        /* 
        * Do some rotation with Euler angles. It is not a fixed axis as
        * quaterions would be, but the effect is cool. 
        */
        rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
        rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

        multiply_matrix(aRotate, aModelView, aModelView);

        rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

        multiply_matrix(aRotate, aModelView, aModelView);

        /* Pull the camera back from the cube */
        aModelView[14] -= 2.5;

        perspective_matrix(45.0, (double)mode.w/(double)mode.h, 0.01, 100.0, aPerspective);
        multiply_matrix(aPerspective, aModelView, aMVP);

        glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, aMVP);

        iXangle += 3;
        iYangle += 2;
        iZangle += 1;

        if(iXangle >= 360) iXangle -= 360;
        if(iXangle < 0) iXangle += 360;
        if(iYangle >= 360) iYangle -= 360;
        if(iYangle < 0) iYangle += 360;
        if(iZangle >= 360) iZangle -= 360;
        if(iZangle < 0) iZangle += 360;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);

	SDL_GL_SwapWindow(window);

	if ( ++num_frames % 100 == 0 ) {
	    gettimeofday( &t2, &tz );
	    float dt  =  t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6;
	    SDL_Log("fps: %f", num_frames / dt);
	    num_frames = 0;
	    t1 = t2;
	}
//      usleep( 1000*10 );
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

   return 0;
}
