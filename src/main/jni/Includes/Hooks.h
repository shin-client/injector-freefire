#include <Struct/Logger.h>

#include <cstddef>
#include <cstdint>
#include <android/input.h>

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

int (*original_AInputQueue_getEvent)(AInputQueue *, AInputEvent **);
int hooked_AInputQueue_getEvent(AInputQueue *queue, AInputEvent **event)
{
  int result = original_AInputQueue_getEvent(queue, event);
  if (result >= 0 && *event)
  {
    int32_t type = AInputEvent_getType(*event);
    if (type == AINPUT_EVENT_TYPE_MOTION)
    {
      int32_t action = AMotionEvent_getAction(*event) & AMOTION_EVENT_ACTION_MASK;
      int32_t pointerIndex = (AMotionEvent_getAction(*event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
      float rawX = AMotionEvent_getX(*event, pointerIndex);
      float rawY = AMotionEvent_getY(*event, pointerIndex);
      float scaledX = (rawX * g_GlWidth) / egl.screenWidth;
      float scaledY = (rawY * g_GlHeight) / egl.screenHeight;
      ImGuiIO &io = ImGui::GetIO();
      switch (action)
      {
      case AMOTION_EVENT_ACTION_DOWN:
      case AMOTION_EVENT_ACTION_POINTER_DOWN:
        io.MousePos = ImVec2(scaledX, scaledY);
        io.MouseDown[0] = true;
        break;
      case AMOTION_EVENT_ACTION_UP:
      case AMOTION_EVENT_ACTION_POINTER_UP:
        io.MousePos = ImVec2(scaledX, scaledY);
        io.MouseDown[0] = false;
        break;
      case AMOTION_EVENT_ACTION_MOVE:
        io.MousePos = ImVec2(scaledX, scaledY);
        break;
      default:
        break;
      }
    }
  }
  return result;
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