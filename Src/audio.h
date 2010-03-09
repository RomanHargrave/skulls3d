
#ifndef SK_AUDIO
#define SK_AUDIO

int InitAudio();
int CloseAudio();
void PlaySound(unsigned char *data, unsigned int len);

#endif // SK_AUDIO