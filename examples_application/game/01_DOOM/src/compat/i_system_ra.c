// i_system implementation for Renesas RA6M5
// Adapted from ESP32-DOOM

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "FreeRTOS.h"
#include "task.h"

#include "config.h"
#include "doomtype.h"
#include "doomdef.h"
#include "m_fixed.h"
#include "r_fps.h"
#include "i_system.h"
#include "lprintf.h"

int realtime = 0;

void I_uSleep(unsigned long usecs) {
    vTaskDelay(pdMS_TO_TICKS(usecs / 1000));
}

static unsigned long getMsTicks() {
    return millis();
}

int I_GetTime_RealTime(void) {
    unsigned long now = getMsTicks();
    return (int)((now * TICRATE) / 1000);
}

const int displaytime = 0;

fixed_t I_GetTimeFrac(void) {
    unsigned long now = getMsTicks();
    fixed_t frac;

    if (tic_vars.step == 0)
        return FRACUNIT;
    else {
        frac = (fixed_t)((now - tic_vars.start + displaytime) * FRACUNIT / tic_vars.step);
        if (frac < 0) frac = 0;
        if (frac > FRACUNIT) frac = FRACUNIT;
        return frac;
    }
}

void I_GetTime_SaveMS(void) {
    if (!movement_smooth)
        return;

    tic_vars.start = getMsTicks();
    tic_vars.next = (unsigned int)((tic_vars.start * tic_vars.msec + 1.0f) / tic_vars.msec);
    tic_vars.step = tic_vars.next - tic_vars.start;
}

unsigned long I_GetRandomTimeSeed(void) {
    return getMsTicks();
}

const char* I_GetVersionString(char* buf, size_t sz) {
    snprintf(buf, sz, "PrBoom-RA6M5 v%s", VERSION_ID);
    return buf;
}

const char* I_SigString(char* buf, size_t sz, int signum) {
    buf[0] = '\0';
    return buf;
}

// Simple file descriptor table
typedef struct {
    const uint8_t *data;
    size_t size;
    size_t offset;
    int in_use;
} FileDesc;

#define MAX_FDS 32
static FileDesc fds[MAX_FDS];

// WAD file data (will be defined elsewhere or loaded from SD card)
extern const uint8_t doom1_wad_start[] asm("_binary_doom1_wad_start");
extern const uint8_t doom1_wad_end[] asm("_binary_doom1_wad_end");

int I_Open(const char *wad, int flags) {
    // Find free file descriptor
    int fd = 3;  // Start from 3 (0,1,2 are stdin/stdout/stderr)
    while (fd < MAX_FDS && fds[fd].in_use) fd++;

    if (fd >= MAX_FDS) {
        Serial.println("I_Open: No free file descriptors");
        return -1;
    }

    if (strcmp(wad, "DOOM1.WAD") == 0 || strcmp(wad, "doom1.wad") == 0) {
        // For now, return error - WAD file needs to be provided
        Serial.println("I_Open: WAD file not implemented yet");
        Serial.println("Please add doom1.wad file support");
        return -1;

        // When WAD is available, uncomment:
        // fds[fd].data = doom1_wad_start;
        // fds[fd].size = doom1_wad_end - doom1_wad_start;
        // fds[fd].offset = 0;
        // fds[fd].in_use = 1;
        // return fd;
    }

    Serial.printf("I_Open: Unknown WAD file: %s\n", wad);
    return -1;
}

int I_Lseek(int ifd, off_t offset, int whence) {
    if (ifd < 0 || ifd >= MAX_FDS || !fds[ifd].in_use) return -1;

    if (whence == SEEK_SET) {
        fds[ifd].offset = offset;
    } else if (whence == SEEK_CUR) {
        fds[ifd].offset += offset;
    } else if (whence == SEEK_END) {
        fds[ifd].offset = fds[ifd].size + offset;
    }

    return fds[ifd].offset;
}

int I_Filelength(int ifd) {
    if (ifd < 0 || ifd >= MAX_FDS || !fds[ifd].in_use) return 0;
    return fds[ifd].size;
}

void I_Close(int fd) {
    if (fd >= 0 && fd < MAX_FDS) {
        fds[fd].in_use = 0;
    }
}

void I_Read(int ifd, void* vbuf, size_t sz) {
    if (ifd < 0 || ifd >= MAX_FDS || !fds[ifd].in_use) return;

    if (fds[ifd].offset + sz > fds[ifd].size) {
        sz = fds[ifd].size - fds[ifd].offset;
    }

    memcpy(vbuf, fds[ifd].data + fds[ifd].offset, sz);
    fds[ifd].offset += sz;
}

// Memory mapping stubs (simplified for RA6M5)
void *I_Mmap(void *addr, size_t length, int prot, int flags, int ifd, off_t offset) {
    if (ifd < 0 || ifd >= MAX_FDS || !fds[ifd].in_use) return NULL;

    // Return direct pointer to data (no actual mmap on embedded system)
    return (void*)(fds[ifd].data + offset);
}

int I_Munmap(void *addr, size_t length) {
    // No-op for our simple implementation
    return 0;
}

const char *I_DoomExeDir(void) {
    return "";
}

char* I_FindFile(const char* wfname, const char* ext) {
    return NULL;
}

void I_SetAffinityMask(void) {
    // No-op
}

int access(const char *path, int atype) {
    return -1;
}
