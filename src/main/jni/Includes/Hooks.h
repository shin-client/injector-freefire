#include <Struct/Logger.h>

#include <cstddef>
#include <cstdint>

#define targetLibName OBFUSCATE("libil2cpp.so")

bool      ResetGuest           = false;
bool (*old_ResetGuest)(void *instance);

bool _ResetGuest(void *instance) {
  if (instance != NULL) {
    if (ResetGuest) {
      LOGD("Hook _ResetGuest done");
      return true;
    }
  }
  return old_ResetGuest(instance);
}