#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <dlfcn.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <vector>

#include "Dobby/dobby.h"

#define libTarget "libil2cpp.so"

void hack();

void DoHOOK(void *offseet, void *ptr, void **orig) { DobbyHook(offseet, ptr, orig); }

#define DHOOK(offseet, ptr, orig) DoHOOK((void *)getAbsoluteAddress(libTarget, offseet), (void *)ptr, (void **)&orig)