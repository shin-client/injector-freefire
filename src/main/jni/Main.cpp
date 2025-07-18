#include <android/native_window.h>
#include <dlfcn.h>
#include <jni.h>
#include <sys/system_properties.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "And64InlineHook/And64InlineHook.hpp"
#include "Chams.h"
#include "ImGui/Fonts/Roboto-Regular.h"
#include "ImGui/Toggle.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/imgui.h"
#include "Includes/Hooks.h"
#include "Includes/Macros.h"
#include "Includes/Utils.h"
#include "Includes/config.h"
#include "KittyMemory/KittyInclude.h"
#include "Struct/Class.h"
#include "Struct/Gui.h"
#include "Struct/Logger.h"
#include "Struct/Patches.h"
#include "Struct/obfuscate.h"
#include "Unity/Il2Cpp.h"

extern int    g_GlWidth, g_GlHeight;
extern ImVec2 imageSize;
ElfScanner    g_il2cppELF;
uintptr_t     g_Il2cppBase = 0;
std::string   processName;

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);

inline static ImVec2 SetCenterMenuPos(float width, float height) {
  return ImVec2(g_GlWidth / 2 - (g_GlWidth * width) / 2, g_GlHeight / 2 - (g_GlHeight * height) / 2);
}

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

  DrawESP(g_GlWidth, g_GlHeight);
  RenderLogsWindow();
  static ImVec2 menuPos = ImVec2(50, 50);
  ImGui::SetNextWindowPos(menuPos, ImGuiCond_Always);
  // ImGui::SetNextWindowSize(ImVec2((float)g_GlWidth * 0.1f, (float)g_GlHeight * 0.1f), ImGuiCond_Once);
  if (ImGui::Begin(OBFUSCATE("NgocDev"), 0,
                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 100.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::ImageButton((void *)(intptr_t)texture, imageSize)) {
      IsMenuOpen = !IsMenuOpen;
    }
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      ImVec2 delta = ImGui::GetIO().MouseDelta;
      menuPos.x += delta.x;
      menuPos.y += delta.y;
    }

    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(3);
    ImGui::End();
  }

  ImGui::SetNextWindowPos(SetCenterMenuPos(0.35f, 0.50f), ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2((float)g_GlWidth * 0.35f, (float)g_GlHeight * 0.50f), ImGuiCond_Once);
  if (IsMenuOpen && ImGui::Begin(OBFUSCATE("NgocDev FF Việt Nam"), &IsMenuOpen,
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus)) {
    ImGui::Text(OBFUSCATE("FPS: %.1f"), ImGui::GetIO().Framerate);

    static int   currentMenu = 0;
    static float item_width = 0.0f, item_height = 60.0f;

    // Sidebar
    ImGui::BeginChild(OBFUSCATE("Sidebar"), ImVec2(150, 0), true);
    {
      if (ImGui::Selectable(OBFUSCATE("ESP"), currentMenu == 0, 0, ImVec2(item_width, item_height))) currentMenu = 0;
      if (ImGui::Selectable(OBFUSCATE("Aimbot"), currentMenu == 1, 0, ImVec2(item_width, item_height))) currentMenu = 1;
      if (ImGui::Selectable(OBFUSCATE("Settings"), currentMenu == 2, 0, ImVec2(item_width, item_height)))
        currentMenu = 2;
      if (ImGui::Selectable(OBFUSCATE("Info"), currentMenu == 3, 0, ImVec2(item_width, item_height))) currentMenu = 3;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Content
    ImGui::BeginChild(OBFUSCATE("Content"), ImVec2(0, 0), true);
    {
      switch (currentMenu) {
        case 0: {
          ImGui::Spacing();
          ImGui::Text(OBFUSCATE("ESP Features"));
          ImGui::Separator();

          Toggle(OBFUSCATE("Enable ESP"), &g_ESPConfig->ESP_Enable);
          Toggle(OBFUSCATE("Box ESP"), &g_ESPConfig->ESP_Box);
          Toggle(OBFUSCATE("Line ESP"), &g_ESPConfig->ESP_Line);
          Toggle(OBFUSCATE("Health ESP"), &g_ESPConfig->ESP_Health);
          Toggle(OBFUSCATE("Name ESP"), &g_ESPConfig->ESP_Name);
          Toggle(OBFUSCATE("Distance ESP"), &g_ESPConfig->ESP_Distance);

          ImGui::Spacing();
          ImGui::Separator();

          ImGui::Spacing();
          if (ImGui::Button(OBFUSCATE("Enable All ESP"), ImVec2(-1, 0))) {
            g_ESPConfig->enableAll();
          }
          if (ImGui::Button(OBFUSCATE("Disable All ESP"), ImVec2(-1, 0))) {
            g_ESPConfig->disableAll();
          }
          break;
        }
        case 1: {
          ImGui::Spacing();
          Toggle(OBFUSCATE("Enable AimBot Head"), &g_AimbotConfig->Aimbot);
          ImGui::Separator();

          if (g_AimbotConfig->Aimbot) {
            ImGui::Text(OBFUSCATE("Aim Settings:"));
            ImGui::Combo(OBFUSCATE("##AimDir"), &g_AimbotConfig->AimWhen, dir, DIR_COUNT);

            if (g_AimbotConfig->AimWhen == 0) {
              ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Mode: Auto Aim"));
            } else if (g_AimbotConfig->AimWhen == 1) {
              ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Mode: Hold to Fire"));
            } else if (g_AimbotConfig->AimWhen == 2) {
              ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), OBFUSCATE("Mode: Hold to Scope"));
            }

            ImGui::SliderFloat(OBFUSCATE("FOV Range"), &g_AimbotConfig->Fov_Aim, 0.0f, 500.0f, "%.0f");
            ImGui::SliderFloat(OBFUSCATE("Max Distance"), &g_AimbotConfig->Aimdis, 0.0f, 1000.0f, "%.0f");
          } else {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), OBFUSCATE("Enable Aimbot to configure settings"));
          }
          break;
        }
        case 2: {
          ImGui::Text(OBFUSCATE("General Settings"));
          ImGui::Separator();

          if (ImGui::Checkbox(OBFUSCATE("Anti-Report"), &g_OtherConfig->AntiReport)) {
            if (g_OtherConfig->AntiReport) {
              SetupAntiReport();
            }
          }

          if (ImGui::Checkbox(OBFUSCATE("Reset Guest"), &g_OtherConfig->ResetGuest)) {
            Patch_ResetGuest_Hook();
          }

          ImGui::Spacing();
          if (ImGui::Button(OBFUSCATE("Reset All Settings"), ImVec2(-1, 0))) {
            g_OtherConfig->resetAll();
          }
          break;
        }
        case 3: {
          ImGui::Spacing();
          ImGui::Text(OBFUSCATE("About"));
          ImGui::Separator();

          ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), OBFUSCATE("Injector by Ngoc"));
          ImGui::Text(OBFUSCATE("ESP & Aimbot for Free Fire"));
          ImGui::Spacing();
          ImGui::Separator();
          ImGui::Text(OBFUSCATE("Version: 1.0.0 | Build: x32/x64"));
          ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Status: Injected"));
          ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Game: %s"), processName.c_str());

          ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), OBFUSCATE("Use responsibly!"));

          ImGui::Spacing();
          ImGui::Separator();

          ImGui::Text(OBFUSCATE("Debug Options"));
          ImGui::Separator();

          if (ImGui::Button(OBFUSCATE("Show Debug Logs"), ImVec2(-1, 0))) {
            g_LogsVisible = true;
          }

          ImGui::Spacing();
          if (ImGui::Button(OBFUSCATE("Export All Logs"), ImVec2(-1, 0))) {
            SaveLogsToFile();
            LOGI("Log export completed. Check file: %s", g_LogFilePath.c_str());
          }
          break;
        }
      }
    }
    ImGui::EndChild();
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
  InitHooks();

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
  sleep(10);

  Il2CppAttach();
  LOGI("IL2CPP attached successfully");

  initPatch();
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
__attribute__((constructor)) void lib_main() {
  std::thread(hack_thread, getpid()).detach();
  LOGI("lib_main success");
}
#endif
