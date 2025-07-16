#include <dlfcn.h>
#include <jni.h>
#include <sys/system_properties.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "Chams.h"
#include "ImGui/Fonts/Comic_Sans.h"
#include "ImGui/Fonts/Roboto-Regular.h"
#include "ImGui/Toggle.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/box_shadow.h"
#include "ImGui/imgui.h"
#include "Includes/ByNameModding.h"
#include "Includes/Utils.h"
#include "Includes/config.h"
#include "Includes/log.h"
#include "Struct/obfuscate.h"
#include "Struct/Class.h"
#include "Struct/Gui.h"

extern int g_GlWidth, g_GlHeight;
ElfScanner g_il2cppELF;
uintptr_t  g_Il2cppBase = g_il2cppELF.base();
char       buf[128];

/*=================[ Hooks ]====================*/

void SetDarkGrayTheme() {
  ImGuiStyle *style  = &ImGui::GetStyle();
  ImVec4     *colors = style->Colors;

  colors[ImGuiCol_Text]                  = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
  colors[ImGuiCol_TextDisabled]          = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
  colors[ImGuiCol_WindowBg]              = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
  colors[ImGuiCol_ChildBg]               = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
  colors[ImGuiCol_PopupBg]               = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
  colors[ImGuiCol_Border]                = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
  colors[ImGuiCol_BorderShadow]          = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
  colors[ImGuiCol_FrameBg]               = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
  colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
  colors[ImGuiCol_FrameBgActive]         = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
  colors[ImGuiCol_TitleBg]               = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_TitleBgActive]         = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
  colors[ImGuiCol_MenuBarBg]             = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
  colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
  colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
  colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_CheckMark]             = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
  colors[ImGuiCol_SliderGrab]            = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
  colors[ImGuiCol_SliderGrabActive]      = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_Button]                = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
  colors[ImGuiCol_ButtonHovered]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
  colors[ImGuiCol_ButtonActive]          = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
  colors[ImGuiCol_Header]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
  colors[ImGuiCol_HeaderHovered]         = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_HeaderActive]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_Separator]             = colors[ImGuiCol_Border];
  colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
  colors[ImGuiCol_SeparatorActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_ResizeGrip]            = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
  colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
  colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_Tab]                   = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
  colors[ImGuiCol_TabHovered]            = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
  colors[ImGuiCol_TabActive]             = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_TabUnfocused]          = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
  colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
  colors[ImGuiCol_PlotLines]             = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
  colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_PlotHistogram]         = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
  colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_TextSelectedBg]        = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
  colors[ImGuiCol_DragDropTarget]        = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavHighlight]          = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
  colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
  colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

  style->ChildRounding     = 4.0f;
  style->FrameBorderSize   = 1.0f;
  style->FrameRounding     = 2.0f;
  style->GrabMinSize       = 7.0f;
  style->PopupRounding     = 2.0f;
  style->ScrollbarRounding = 12.0f;
  style->ScrollbarSize     = 13.0f;
  style->TabBorderSize     = 1.0f;
  style->TabRounding       = 0.0f;
  style->WindowRounding    = 4.0f;
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

  if (!g_IsSetup) {
    LOGD("Setting up ImGui...");
    prevWidth  = g_GlWidth;
    prevHeight = g_GlHeight;
    SetupImgui();
    LOGI("ImGui setup completed - Screen: %dx%d", g_GlWidth, g_GlHeight);
    g_IsSetup = true;
  }

  SetDarkGrayTheme();
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

  ImGui::SetNextWindowSize(ImVec2((float)g_GlWidth * 0.35f, (float)g_GlHeight * 0.50f), ImGuiCond_Once);
  sprintf(buf, OBFUSCATE("NgocDev FF Viet Nam [ %0.1f FPS ]"), (io.Framerate), ImGui::GetFrameCount());
  if (ImGui::Begin(buf, 0, ImGuiWindowFlags_NoBringToFrontOnFocus)) {
    RectangleShadowSettings shadowSettings;
    shadowSettings.rectSize              = ImVec2(ImGui::GetWindowSize().x - 5, ImGui::GetWindowSize().y - 5);
    shadowSettings.sigma                 = 10;
    shadowSettings.padding               = ImVec2(3.f, 3.f);
    shadowSettings.rectPos               = shadowSettings.padding;
    shadowSettings.rings                 = 4;
    shadowSettings.spacingBetweenRings   = 3;
    shadowSettings.samplesPerCornerSide  = 3;
    shadowSettings.spacingBetweenSamples = 15;
    shadowSettings.shadowColor           = ImVec4(0, 188, 212, 1);
    shadowSettings.shadowSize            = ImVec2(0, 0);
    drawRectangleShadowVerticesAdaptive(shadowSettings);

    static int   currentMenu = 0;
    static float item_width = 100.0f, item_height = 80.0f;

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
            Patch_ResetGuest(g_OtherConfig->ResetGuest);
          }

          ImGui::Spacing();
          ImGui::Text(OBFUSCATE("Version: 1.0.0 | Build: x32/x64"));
          ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Status: Injected"));
          ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Game: Free Fire"));

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

          ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), OBFUSCATE("Zygisk Module by Ngoc"));
          ImGui::Text(OBFUSCATE("ESP & Aimbot for Free Fire"));
          ImGui::Separator();

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

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return old_eglSwapBuffers(dpy, surface);
}

void StartGUI() {
  LOGD("Starting GUI initialization...");
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
  LOGD("=== ZYGISK FF HACK THREAD START ===");
  LOGD("Thread PID: %i", pid);

  do {
    sleep(1);
    LOGD("Searching for libil2cpp.so...");
    g_il2cppELF = ElfScanner::createWithPath("libil2cpp.so");
  } while (!g_il2cppELF.isValid());

  if (g_Il2cppBase == 0) {
    LOGE("CRITICAL: libil2cpp.so not found!");
    LOGE("Thread %d terminating", pid);
    std::terminate();
  }

  LOGI("il2cppBase: 0x%" PRIxPTR, g_Il2cppBase);
  LOGD("Waiting 10 seconds before attachment...");
  sleep(10);

  LOGD("Starting IL2CPP attachment...");
  Il2CppAttach();
  LOGI("IL2CPP attached successfully");

  LOGD("Initializing GUI...");
  StartGUI();
  LOGD("=== ZYGISK FF INITIALIZATION COMPLETE ===");

  // while (!G_IL2CPP) {
  //   G_IL2CPP = findLibrary(libTarget);
  //   sleep(5);

  //   //====== [Your Hooks] ======
  //   // parameter Example -> public bool get_IsIAP() { } = 0
  //   // parameter Example -> public bool get_IsIAP() {int gold } = 1
  //   // parameter Example -> public bool get_IsIAP() {int gold, float health } = 2
  //   // parameter Example -> public bool get_IsIAP() {int gold, float health, bool shop } = 3

  //   // Namespace: SYBO.Subway.Meta
  //   // public class Currency
  //   // public bool get_IsIAP() { }
  //   //  if your dump.cs doesn't have Namespace then Do this
  //   //  auto Currency = new LoadClass(/*Namespace*/"", OBFUSCATE(/*Class*/"Currency"));

  //   // Bool Hook
  //   auto Currency = new LoadClass(/*Namespace*/ "SYBO.Subway.Meta", OBFUSCATE(/*Class*/ "Currency"));
  //   FreeShop = Currency->GetMethodOffsetByName(OBFUSCATE(/*Method*/ "get_IsIAP"), /*parameter*/ 0);
  //   DobbyHook((void *)FreeShop, (void *)_BoolExample, (void **)&old_BoolExample);

  //   // Int Hook
  //   auto CharacterMotorAbilities =
  //       new LoadClass(/*Namespace*/ "SYBO.RunnerCore.Character", OBFUSCATE(/*Class*/ "CharacterMotorAbilities"));
  //   JumpLimit = CharacterMotorAbilities->GetMethodOffsetByName(OBFUSCATE(/*Method*/ "get_JumpLimit"), /*parameter*/
  //   0); DobbyHook((void *)JumpLimit, (void *)_IntExample, (void **)&old_IntExample);

  //   // Float Hook
  //   JumpHight = CharacterMotorAbilities->GetMethodOffsetByName(OBFUSCATE(/*Method*/ "get_JumpHeight"), /*parameter*/
  //   0); DobbyHook((void *)JumpHight, (void *)_FloatExample, (void **)&old_FloatExample);
  // }
}

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

__attribute__((constructor)) void lib_main() { std::thread(hack_thread).detach(); }
#endif
