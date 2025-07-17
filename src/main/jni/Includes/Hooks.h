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

/*=================[ Csr ]====================*/
static bool  svaston;
static float cs1             = 6.8f;
static float svastonColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
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

void (*old_input)(void *event, void *exAb, void *exAc);

void hook_input(void *event, void *exAb, void *exAc) {
  old_input(event, exAb, exAc);
  ImGui_ImplAndroid_HandleTouchEvent(
      (AInputEvent *)event, {(float)egl.screenWidth / (float)g_GlWidth, (float)egl.screenHeight / (float)g_GlHeight});
  return;
}

int (*old_getWidth)(ANativeWindow *window);

int hook_getWidth(ANativeWindow *window) {
  egl.screenWidth = old_getWidth(window);
  return old_getWidth(window);
}

int (*old_getHeight)(ANativeWindow *window);

int hook_getHeight(ANativeWindow *window) {
  egl.screenHeight = old_getHeight(window);
  return old_getHeight(window);
}

void InitHooks() {
  void *libAndroid = dlopen(OBFUSCATE("libandroid.so"), RTLD_NOW | RTLD_GLOBAL);
  if (!libAndroid) {
    LOGE(OBFUSCATE("Failed to load libandroid.so: %s"), dlerror());
  }

  void *sym_input = DobbySymbolResolver("/system/lib/libinput.so",
                                        "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
  if (sym_input != NULL) {
    DobbyHook((void *)sym_input, (void *)hook_input, (void **)&old_input);
  }

  DobbyHook((void *)dlsym(dlopen(OBFUSCATE("libandroid.so"), RTLD_NOW | RTLD_GLOBAL), OBFUSCATE("ANativeWindow_getWidth")),
            (void *)hook_getWidth, (void **)&old_getWidth);
  DobbyHook((void *)dlsym(dlopen(OBFUSCATE("libandroid.so"), RTLD_NOW | RTLD_GLOBAL), OBFUSCATE("ANativeWindow_getHeight")),
            (void *)hook_getHeight, (void **)&old_getHeight);
}