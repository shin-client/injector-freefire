#pragma once

#ifndef DWORD
#define DWORD unsigned int
#endif

#include <jni.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Struct/Logger.h"
#include "Struct/Obfuscate.h"

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

uintptr_t string2Offset(const char *c) {
  int base = 16;
  // See if this function catches all possibilities.
  // If it doesn't, the function would have to be amended
  // whenever you add a combination of architecture and
  // compiler that is not yet addressed.
  static_assert(sizeof(uintptr_t) == sizeof(unsigned long) || sizeof(uintptr_t) == sizeof(unsigned long long),
                "Please add string to handle conversion for this architecture.");

  // Now choose the correct function ...
  if (sizeof(uintptr_t) == sizeof(unsigned long)) {
    return strtoul(c, nullptr, base);
  }

  // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
  return strtoull(c, nullptr, base);
}

namespace ToastLength {
inline const int LENGTH_LONG  = 1;
inline const int LENGTH_SHORT = 0;
}