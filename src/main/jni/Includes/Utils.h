#ifndef UTILS
#define UTILS

#include <jni.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "log.h"

static uintptr_t libBase;
const char      *libName   = "libil2cpp.so";
bool             libLoaded = false;

DWORD findLibrary(const char *library) {
  char  filename[0xFF] = {0}, buffer[1024] = {0};
  FILE *fp      = NULL;
  DWORD address = 0;

  sprintf(filename, OBFUSCATE("/proc/self/maps"));

  fp = fopen(filename, OBFUSCATE("rt"));
  if (fp == NULL) {
    perror(OBFUSCATE("fopen"));
    goto done;
  }

  while (fgets(buffer, sizeof(buffer), fp)) {
    if (strstr(buffer, library)) {
      address = (DWORD)strtoul(buffer, NULL, 16);
      goto done;
    }
  }

done:

  if (fp) {
    fclose(fp);
  }

  return address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr) {
  libBase = findLibrary(libraryName);
  if (libBase == 0) return 0;
  return static_cast<DWORD>(static_cast<uintptr_t>(libBase) + static_cast<uintptr_t>(relativeAddr));
}

bool isLibraryLoaded(const char *libraryName) {
  char  line[512] = {0};
  FILE *fp        = fopen("/proc/self/maps", "rt");
  if (fp != NULL) {
    while (fgets(line, sizeof(line), fp)) {
      std::string a = line;
      if (strstr(line, libraryName)) {
        libLoaded = true;
        return true;
      }
    }
    fclose(fp);
  }
  return false;
}

#endif