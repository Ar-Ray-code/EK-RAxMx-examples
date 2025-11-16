// Configuration file for DOOM on Renesas RA6M5
// Adapted from ESP32-DOOM

#ifndef __CONFIG_H__
#define __CONFIG_H__

// Platform definitions
#define RENESAS_RA6M5 1

// Screen dimensions
#define SCREENWIDTH  320
#define SCREENHEIGHT 240
#define SCREENPITCH  SCREENWIDTH

// Memory constraints
// RA6M5 has 512KB SRAM (much less than ESP32's PSRAM)
#define ZONSIZE (128*1024)  // Reduced memory zone

// Disable features to save memory
#define NOMUSIC 1
#define NOSOUND 1
#define HAVE_LIBZ 0
#define HAVE_LIBPNG 0

// FreeRTOS is available
#define HAVE_FREERTOS 1

// No network
#define HAVE_NET 0

// Compiler features
#define HAVE_STPCPY 1
#define HAVE_MMAP 0

// Version info
#define VERSION_ID "2.5.1.5-ra6m5"
#define PACKAGE_NAME "prboom-ra6m5"
#define PACKAGE_VERSION "2.5.1.5"

#endif // __CONFIG_H__
