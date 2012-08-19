
// Thanx to http://www.libsdl.org/intro.en/usingsound.html

#include "audio.h"

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

void mixaudio(void *unused, Uint8 *stream, int len);

int InitAudio()
{
    extern void mixaudio(void *unused, Uint8 *stream, int len);
    SDL_AudioSpec fmt;

    /* Set 8-bit stereo audio at 11Khz */
    fmt.freq = 11025;
	fmt.format = AUDIO_U8;
    fmt.channels = 1;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = mixaudio;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        return -1;
    }
    SDL_PauseAudio(0);

	return 0;
}

int CloseAudio()
{
    SDL_CloseAudio();
	return 0;
}

#define NUM_SOUNDS 1
struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

void mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    int amount;

    for ( i=0; i<NUM_SOUNDS; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += amount;
    }
}

void SKPlaySound(unsigned char *data, unsigned int len)
{  
	SDL_LockAudio();
    sounds[0].data = data;
    sounds[0].dlen = len;
    sounds[0].dpos = 0;
    SDL_UnlockAudio();
}