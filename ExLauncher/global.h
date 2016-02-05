#ifndef _GLOBAL_H
#define _GLOBAL_H

#if defined(WIN32) || defined(_WIN32)
#define WINDOWS
#endif

#if defined(__unix) || defined(__unix__)
#define UNIX
#endif

#define FPS 60

extern bool debugViewBounds;
extern bool isLauncher;

#endif
