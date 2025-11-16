// Sound stub for Renesas RA6M5
// Sound is disabled due to memory constraints

#include "config.h"
#include "doomtype.h"
#include "i_sound.h"

// Sound disabled
void I_InitSound(void) {
}

void I_ShutdownSound(void) {
}

void I_SetChannels(void) {
}

int I_GetSfxLumpNum(sfxinfo_t *sfxinfo) {
    return 0;
}

int I_StartSound(int id, int channel, int vol, int sep, int pitch, int priority) {
    return 0;
}

void I_StopSound(int handle) {
}

int I_SoundIsPlaying(int handle) {
    return 0;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch) {
}

void I_UpdateSound(void) {
}

void I_SubmitSound(void) {
}

void I_ShutdownMusic(void) {
}

void I_InitMusic(void) {
}

void I_PlaySong(int handle, int looping) {
}

void I_PauseSong(int handle) {
}

void I_ResumeSong(int handle) {
}

void I_StopSong(int handle) {
}

void I_UnRegisterSong(int handle) {
}

int I_RegisterSong(const void *data, size_t len) {
    return 0;
}

int I_RegisterMusic(const char *filename, musicinfo_t *music) {
    return 0;
}

void I_SetMusicVolume(int volume) {
}
