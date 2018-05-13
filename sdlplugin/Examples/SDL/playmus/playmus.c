#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "SDL.h"
#include "SDL_mixer.h"

#define FONT_FILE "/system/fonts/DroidSans.ttf"

static int audio_open = 0;
static Mix_Music *music = NULL;

static SDL_DisplayMode mode;
static SDL_Window *window;

void CleanUp(int exitcode)
{
    if (Mix_PlayingMusic()) {
        Mix_FadeOutMusic(1500);
        SDL_Delay(1500);
    }
    if (music) {
        Mix_FreeMusic(music);
        music = NULL;
    }
    if (audio_open) {
        Mix_CloseAudio();
        audio_open = 0;
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    exit(exitcode);
}

int main(int argc, char *argv[])
{
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;
    int audio_volume = MIX_MAX_VOLUME;
    int looping = 0;

    /* Initialize variables */
    audio_rate = 22050;
    audio_format = AUDIO_S16;
    audio_channels = 2;
    audio_buffers = 4096;

    /* Initialize the SDL library */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Unable to initialize SDL");
        return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);

    window = SDL_CreateWindow("Simple texture moving", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

    if (!window) {
	SDL_Log("Unable to create window");
	return 1;
    }

    /* Open the audio device */
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
        SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
        return(2);
    } else {
        Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
        SDL_Log("Opened audio at %d Hz %d bit %s (%s), %d bytes audio buffer\n", audio_rate,
            (audio_format&0xFF),
            (audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono",
            (audio_format&0x1000) ? "BE" : "LE",
            audio_buffers );
    }
    audio_open = 1;

    /* Set the music volume */
    Mix_VolumeMusic(audio_volume);

    /* Set the external music player, if any */
    Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));

    /* Load the requested music file */
//    if ( rwops ) {
//        music = Mix_LoadMUS_RW(SDL_RWFromFile(argv[i], "rb"), SDL_TRUE);
//    } else {
    music = Mix_LoadMUS("CrazyDream.ogg");
//    }

    if ( music == NULL ) {
        SDL_Log("Couldn't load: %s\n", SDL_GetError());
        CleanUp(2);
    }

    /* Play and then exit */
    SDL_Log("Playing\n");
    Mix_FadeInMusic(music,looping,2000);
    int done = 0;

    while ( !done && (Mix_PlayingMusic() || Mix_PausedMusic()) ) {
            SDL_Event event;
            while ( SDL_PollEvent(&event) ) {
                if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
                    done = 1;
                }
            }
        SDL_Delay(100);
    }
    Mix_FreeMusic(music);

    CleanUp(0);

    /* Not reached, but fixes compiler warnings */
    return 0;
}
