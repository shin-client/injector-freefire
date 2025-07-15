#include <list>
#include <vector>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <pthread.h>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "Dobby/dobby.h"

#define libTarget "libil2cpp.so"

uintptr_t G_IL2CPP;



void DoHOOK(void *offseet, void* ptr, void **orig) {
    DobbyHook(offseet, ptr, orig);
}
#define DHOOK(offseet, ptr, orig) DoHOOK((void *)getAbsoluteAddress(libTarget, offseet), (void *)ptr, (void **)&orig)

struct {
    bool setup;
    
    int screenWidth;
    int screenHeight;
} egl;

