#include <Struct/Logger.h>

#include <cstddef>
#include <cstdint>

#define targetLibName OBFUSCATE("libil2cpp.so")

extern int g_GlWidth, g_GlHeight;

bool ResetGuest = false;
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

/*=================[ Fov ]====================*/
bool fovFeature = false;
int  fovValue   = 55;

float (*Org_FovHack2)(void *instance);

float FovHack2(void *instance) {
  if (instance != NULL) {
    if (fovFeature) {
      return fovValue;
    }
  }
  return Org_FovHack2(instance);
}

/*=================[ Csr ]====================*/
static bool  svaston;
static float cs1             = 6.8f;
static float svastonColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static bool  isFov2;
static float aimRadius = 55.0f, aimThickness = 1.0f;
static float aimColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

void SwastonCrosshairv1() {
  ImGui::GetBackgroundDrawList()->AddLine(ImVec2(g_GlWidth / 2, (g_GlHeight / 2) - cs1),
                                          ImVec2(g_GlWidth / 2, (g_GlHeight / 2) + cs1),
                                          ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)svastonColor));
  ImGui::GetBackgroundDrawList()->AddLine(ImVec2((g_GlWidth / 2) - cs1, g_GlHeight / 2),
                                          ImVec2((g_GlWidth / 2) + cs1, g_GlHeight / 2),
                                          ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)svastonColor));
}

/*=================[ System ]====================*/
/*
void (*old_input)(void *event, void *exAb, void *exAc);

void hook_input(void *event, void *exAb, void *exAc) {
  old_input(event, exAb, exAc);
  ImGui_ImplAndroid_HandleTouchEvent(
      (AInputEvent *)event, {(float)g_GlWidth / (float)g_GlWidth, (float)g_GlHeight / (float)g_GlHeight});
  return;
}

int (*old_getWidth)(ANativeWindow *window);

int hook_getWidth(ANativeWindow *window) {
  g_GlWidth = old_getWidth(window);
  return old_getWidth(window);
}

int (*old_getHeight)(ANativeWindow *window);

int hook_getHeight(ANativeWindow *window) {
  g_GlHeight = old_getHeight(window);
  return old_getHeight(window);
}

// Use in hack_thread()
void *sym_input =
    DobbySymbolResolver(("/system/lib/libinput.so"),
                        ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
if (NULL != sym_input) {
  DobbyHook((void *)sym_input, (void *)hook_input, (void **)&old_input);
}

void *libEGL = dlopen(OBFUSCATE("libEGL.so"), RTLD_NOW | RTLD_GLOBAL);
if (!libEGL) {
  LOGE(OBFUSCATE("Failed to load libEGL.so: %s"), dlerror());
}

DobbyHook((void *)dlsym(dlopen(OBFUSCATE("libandroid.so"), 4), OBFUSCATE("ANativeWindow_getWidth")),
          (void *)hook_getWidth, (void **)&old_getWidth);
DobbyHook((void *)dlsym(dlopen(OBFUSCATE("libandroid.so"), 4), OBFUSCATE("ANativeWindow_getHeight")),
          (void *)hook_getHeight, (void **)&old_getHeight);
*/