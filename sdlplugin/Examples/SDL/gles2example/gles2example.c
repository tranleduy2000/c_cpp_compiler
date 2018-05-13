/*
 * SDL gles2 example for CCTools SDL plugin
 *
 * http://cctools.info
 *
 * Ported from:
 * Created by exoticorn ( http://talk.maemo.org/showthread.php?t=37356 )
 * edited and commented by André Bergner [endboss]
 */

#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include <SDL.h>
#include <SDL_opengles2.h>

const char vertex_src [] =
"                                        \
   attribute vec4        position;       \
   varying mediump vec2  pos;            \
   uniform vec4          offset;         \
                                         \
   void main()                           \
   {                                     \
      gl_Position = position + offset;   \
      pos = position.xy;                 \
   }                                     \
";
 
 
const char fragment_src [] =
"                                                      \
   varying mediump vec2    pos;                        \
   uniform mediump float   phase;                      \
                                                       \
   void  main()                                        \
   {                                                   \
      gl_FragColor  =  vec4( 1., 0.9, 0.7, 1.0 ) *     \
        cos( 30.*sqrt(pos.x*pos.x + 1.5*pos.y*pos.y)   \
             + atan(pos.y,pos.x) - phase );            \
   }                                                   \
";
//  some more formulas to play with...
//      cos( 20.*(pos.x*pos.x + pos.y*pos.y) - phase );
//      cos( 20.*sqrt(pos.x*pos.x + pos.y*pos.y) + atan(pos.y,pos.x) - phase );
//      cos( 30.*sqrt(pos.x*pos.x + 1.5*pos.y*pos.y - 1.8*pos.x*pos.y*pos.y)
//            + atan(pos.y,pos.x) - phase );


void print_shader_info_log(GLuint  shader)
{
    GLint  length;

    glGetShaderiv ( shader , GL_INFO_LOG_LENGTH , &length );

    if ( length ) {
	char* buffer  =  alloca(length);
	glGetShaderInfoLog ( shader , length , NULL , buffer );
	SDL_Log("shader info: %s", buffer);

	GLint success;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( success != GL_TRUE ) {
	    exit ( 1 );
	}
    }
}


GLuint load_shader(const char  *shader_source, GLenum type)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    print_shader_info_log(shader);

    return shader;
}

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

GLint
    phase_loc,
    offset_loc,
    position_loc;


int  update_pos = 0;

const float vertexArray[] = {
    0.0,  0.5,  0.0,
   -0.5,  0.0,  0.0,
    0.0, -0.5,  0.0,
    0.5,  0.0,  0.0,
    0.0,  0.5,  0.0 
};


void  render()
{
    static float  phase = 0;
    static int    donesetup = 0;

    //// draw

    if ( !donesetup ) {
	glViewport ( 0 , 0 , mode.w , mode.h );
	glClearColor ( 0.08 , 0.06 , 0.07 , 1.);    // background color
	donesetup = 1;
    }
    glClear ( GL_COLOR_BUFFER_BIT );

   glUniform1f ( phase_loc , phase );  // write the value of phase to the shaders phase
   phase  =  fmodf ( phase + 0.5f , 2.f * 3.141f );    // and update the local variable

    if ( update_pos ) {  // if the position of the texture has changed due to user action
	GLfloat old_offset_x  =  offset_x;
	GLfloat old_offset_y  =  offset_y;

	offset_x  =  norm_x - p1_pos_x;
	offset_y  =  norm_y - p1_pos_y;

	p1_pos_x  =  norm_x;
	p1_pos_y  =  norm_y;

	offset_x  +=  old_offset_x;
	offset_y  +=  old_offset_y;

	update_pos = 0;
    }

    glUniform4f ( offset_loc  ,  offset_x , offset_y , 0.0 , 0.0 );

    glVertexAttribPointer ( position_loc, 3, GL_FLOAT, 0, 0, vertexArray );
    glEnableVertexAttribArray ( position_loc );
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 5 );

    SDL_GL_SwapWindow(window);
}

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

    GLuint vertexShader   = load_shader ( vertex_src , GL_VERTEX_SHADER  );     // load vertex shader
    GLuint fragmentShader = load_shader ( fragment_src , GL_FRAGMENT_SHADER );  // load fragment shader

    GLuint shaderProgram  = glCreateProgram ();                 // create program object
    glAttachShader ( shaderProgram, vertexShader );             // and attach both...
    glAttachShader ( shaderProgram, fragmentShader );           // ... shaders to it

    glLinkProgram ( shaderProgram );    // link the program
    glUseProgram  ( shaderProgram );    // and select it for usage

   //// now get the locations (kind of handle) of the shaders variables
    position_loc  = glGetAttribLocation  ( shaderProgram , "position" );
    phase_loc     = glGetUniformLocation ( shaderProgram , "phase"    );
    offset_loc    = glGetUniformLocation ( shaderProgram , "offset"   );
    if ( position_loc < 0  ||  phase_loc < 0  ||  offset_loc < 0 ) {
	SDL_Log("Unable to get uniform location");
	return 1;
    }

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
            case SDL_MOUSEBUTTONDOWN: {
		GLfloat window_y  =  (mode.h - event.button.y) - mode.h / 2.0;
		norm_y            =  window_y / (mode.h / 2.0);
		GLfloat window_x  =  event.button.x - mode.w / 2.0;
		norm_x            =  window_x / (mode.w / 2.0);
		update_pos = 1;
		}
		break;
	    case SDL_KEYDOWN:
		if (event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
		    quit = 1;
		}
		break;
	    }
	}

	render();   // now we finally put something on the screen

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
