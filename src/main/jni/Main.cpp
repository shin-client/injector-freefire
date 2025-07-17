#include <dlfcn.h>
#include <jni.h>
#include <sys/system_properties.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "And64InlineHook/And64InlineHook.hpp"
#include "Chams.h"
#include "ImGui/Fonts/Roboto-Regular.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/imgui.h"
#include "Includes/ByNameModding.h"
#include "Includes/Hooks.h"
#include "Includes/Macros.h"
#include "Includes/Utils.h"
#include "Includes/config.h"
#include "KittyMemory/KittyInclude.h"
#include "Struct/Class.h"
#include "Struct/Gui.h"
#include "Struct/Logger.h"
#include "Struct/obfuscate.h"
#include "Unity/Il2Cpp.h"

extern int  g_GlWidth, g_GlHeight;
ElfScanner  g_il2cppELF;
uintptr_t   g_Il2cppBase;
std::string processName;

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);

EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

  if (!g_IsSetup) {
    prevWidth  = g_GlWidth;
    prevHeight = g_GlHeight;
    SetupImgui();
    LOGI("ImGui setup completed - Screen: %dx%d", g_GlWidth, g_GlHeight);
    g_IsSetup = true;
  }

  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
  ImGui::NewFrame();

  int touchCount = (((int (*)())(Class_Input__get_touchCount))());
  if (touchCount > 0) {
    UnityEngine_Touch_Fields touch    = ((UnityEngine_Touch_Fields (*)(int))(Class_Input__GetTouch))(0);
    float                    reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
    switch (touch.m_Phase) {
      case TouchPhase::Began:
      case TouchPhase::Stationary:
        io.MousePos     = ImVec2(touch.m_Position.fields.x, reverseY);
        io.MouseDown[0] = true;
        break;
      case TouchPhase::Ended:
      case TouchPhase::Canceled:
        io.MouseDown[0] = false;
        break;
      case TouchPhase::Moved:
        io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
        break;
      default:
        break;
    }
  }

  if (svaston) {
    SwastonCrosshairv1();
  }

  DrawESP(g_GlWidth, g_GlHeight);
  ImGui::SetNextWindowSize(ImVec2((float)g_GlWidth * 0.35f, (float)g_GlHeight * 0.50f), ImGuiCond_Once);
  if (ImGui::Begin(OBFUSCATE("NgocDev FF Việt Nam"), 0, ImGuiWindowFlags_NoBringToFrontOnFocus)) {
    SetShadowSettings();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("%s", processName.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnOffset(1, 145);
    static int tab = 1;
    {
      if (ImGui::Button("PLAYER", ImVec2(100, 80))) {
        tab = 1;
      }
      if (ImGui::Button("CHAMS", ImVec2(100, 80))) {
        tab = 2;
      }
      if (ImGui::Button("VISUAL", ImVec2(100, 80))) {
        tab = 3;
      }
      ImGui::NextColumn();

      if (tab == 1) {
        static bool prev_ResetGuest = false;
        ImGui::Checkbox(OBFUSCATE("Reset Guest"), &ResetGuest);
        if (ResetGuest != prev_ResetGuest) {
          if (ResetGuest && ResetGuest_Offset != 0) {
            // Patch_ResetGuest(ResetGuest);
            // LOGD("Patch_ResetGuest [ON]");
            LOGD("ResetGuest_OffsetVar: %d", ResetGuest_Offset);
            DWORD absAddr = getAbsoluteAddress("libil2cpp.so", (DWORD)ResetGuest_Offset);
            LOGD("absAddr: %d", absAddr);
            DobbyHook((void *)absAddr, (void *)_ResetGuest, (void **)&old_ResetGuest);
            LOGD("DobbyHook [ON]");
            // A64HookFunction((void *)absAddr, (void *)_ResetGuest, (void **)&old_ResetGuest);
            // LOGD("A64HookFunction [ON]");
          } else {
            LOGD("Reset Guest [OFF]");
            DobbyDestroy((void *)ResetGuest_Offset);
          }
          prev_ResetGuest = ResetGuest;
        }
      }

      if (tab == 2) {
        ImGui::Text("%s", processName.c_str());
      }

      if (tab == 3) {
        ImGui::Checkbox(OBFUSCATE("Draw Crosshair"), &svaston);
        if (svaston) {
          ImGui::SameLine();
          ImGui::PushItemWidth(300);
          ImGui::ColorEdit3("Color##Svaston", (float *)&svastonColor, ImGuiColorEditFlags_NoInputs);
          ImGui::SliderFloat("Crosshair Size##Aim", &cs1, 0, 100);
          ImGui::PopItemWidth();
        }
      }
    }
  }

  ImGui::End();
  ImGui::Render();

  glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return old_eglSwapBuffers(dpy, surface);
}

void StartGUI() {
  void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
  if (ptr_eglSwapBuffer != nullptr) {
    LOGI("eglSwapBuffers found at address: %p", ptr_eglSwapBuffer);
    DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
    LOGI("GUI started successfully");
  } else {
    LOGE("Failed to find eglSwapBuffers symbol");
  }
}

void hack_thread(pid_t pid) {
  LOGD("Thread PID: %i", pid);

  processName = KittyMemory::getProcessName();

  do {
    sleep(1);
    LOGD("Searching for libil2cpp.so...");
    g_il2cppELF = ElfScanner::createWithPath("libil2cpp.so");
  } while (!g_il2cppELF.isValid());
  g_Il2cppBase = g_il2cppELF.base();

  if (g_Il2cppBase == 0) {
    LOGE("CRITICAL: libil2cpp.so not found!");
    LOGE("Thread %d terminating", pid);
    std::terminate();
  }

  LOGI("il2cppBase: 0x%" PRIxPTR, g_Il2cppBase);
  LOGD("Waiting 10 seconds before attachment...");
  sleep(5);

  Il2CppAttach();
  LOGI("IL2CPP attached successfully");

  LOGI("ResetGuest_OffsetVar: %d", ResetGuest_Offset);
  StartGUI();
}

// INFO: DO NOT CHANGE
void *pLibRealUnity = 0;
typedef jint(JNICALL *CallJNI_OnLoad_t)(JavaVM *vm, void *reserved);
typedef void(JNICALL *CallJNI_OnUnload_t)(JavaVM *vm, void *reserved);
CallJNI_OnLoad_t   RealJNIOnLoad   = 0;
CallJNI_OnUnload_t RealJNIOnUnload = 0;

#ifdef UseFakeLib
JNIEXPORT jint JNICALL CallJNIOL(JavaVM *vm, void *reserved) {
  LOGI("OnLoad called");
  std::thread(hack_thread, getpid()).detach();
  if (!pLibRealUnity) pLibRealUnity = dlopen("librealmain.so", RTLD_NOW);
  if (!pLibRealUnity) pLibRealUnity = dlopen("librealunity.so", RTLD_NOW);
  if (!RealJNIOnLoad) RealJNIOnLoad = reinterpret_cast<CallJNI_OnLoad_t>(dlsym(pLibRealUnity, "JNI_OnLoad"));
  return RealJNIOnLoad(vm, reserved);
}

JNIEXPORT void JNICALL CallJNIUL(JavaVM *vm, void *reserved) {
  LOGI("OnUnload called");
  if (!pLibRealUnity) pLibRealUnity = dlopen("librealmain.so", RTLD_NOW);
  if (!pLibRealUnity) pLibRealUnity = dlopen("librealunity.so", RTLD_NOW);
  if (!RealJNIOnUnload) RealJNIOnUnload = reinterpret_cast<CallJNI_OnUnload_t>(dlsym(pLibRealUnity, "JNI_OnUnload"));
  RealJNIOnUnload(vm, reserved);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  LOGI("Initialize JNI");
  return CallJNIOL(vm, reserved);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
  LOGI("Unload JNI");
  CallJNIUL(vm, reserved);
}

#else
__attribute__((constructor)) void lib_main() { std::thread(hack_thread, getpid()).detach(); }
#endif
