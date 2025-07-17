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
#include "ImGui/box_shadow.h"
#include "ImGui/imgui.h"
#include "Includes/ByNameModding.h"
#include "Includes/Hooks.h"
#include "Includes/config.h"
#include "KittyMemory/KittyInclude.h"
#include "Struct/Class.h"
#include "Struct/Logger.h"
#include "Struct/obfuscate.h"
#include "Unity/Il2Cpp.h"

extern int g_GlWidth, g_GlHeight;
ElfScanner g_il2cppELF;
uintptr_t  g_Il2cppBase;

/*=================[ Offsets ]====================*/
uintptr_t ResetGuest_OffsetVar = 0;

/*=================[ End Offsets ]====================*/

/*=================[ Chams ]====================*/
const char *shaderlm     = "[ OFF ]";
static int  shaderrn     = 0;
const char *shadertype[] = {"[ OFF ]",
                            "_MainTex",
                            "_BumpMap",
                            "_MainTex_ST",
                            "_Color",
                            "hlslcc_mtx4x4unity_MatrixVP[0]",
                            "hlslcc_mtx4x4unity_ObjectToWorld[0]",
                            "_Flip",
                            "_RendererColor",
                            "_Displacement",
                            "uvOffsetAndScale",
                            "_AtmosphereThickness",
                            "_SkyTint",
                            "_GroundColor",
                            "_Exposure",
                            "_LightColor0",
                            "_WorldSpaceLightPos0",
                            "_SunSize",
                            "_TextureSampleAdd",
                            "_UIMaskSoftnessY",
                            "_UIMaskSoftnessX",
                            "_ClipRect",
                            "hlslcc_mtx4x4glstate_matrix_projection[0]",
                            "_ScreenParams",
                            "sk_RTAdjust",
                            "uTextureSampler_0_Stage0",
                            "uAtlasSizeInv_Stage0",
                            "uTextureSampler_0_Stage0",
                            "unity_SHC"};

bool chams, wireframe, outline, glow = false;
int  setR  = 0;
int  setG  = 0;
int  setB  = 0;
int  setw1 = 1;
int  setw2 = 1;
int  setw3 = 1;

/*=================[ Fov ]====================*/
uintptr_t P_View;
uintptr_t P_View2;
bool      fovFeature = false;
int       fovValue   = 55;
void (*Org_FovHack1)(void *instance, float fov);

void FovHack1(void *instance, float fov) {
  if (instance != NULL) {
    if (fovFeature) {
      fov = fovValue;
    }
  }
  Org_FovHack1(instance, fov);
}

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

void SwastonCrosshairv1() {
  ImGui::GetBackgroundDrawList()->AddLine(ImVec2(g_GlWidth / 2, (g_GlHeight / 2) - cs1),
                                          ImVec2(g_GlWidth / 2, (g_GlHeight / 2) + cs1),
                                          ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)svastonColor));
  ImGui::GetBackgroundDrawList()->AddLine(ImVec2((g_GlWidth / 2) - cs1, g_GlHeight / 2),
                                          ImVec2((g_GlWidth / 2) + cs1, g_GlHeight / 2),
                                          ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)svastonColor));
}

static bool  isFov2;
static float aimRadius = 55.0f, aimThickness = 1.0f;
static float aimColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

std::string processName;
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);

EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
  eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
  eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

  if (!egl.setup) {
    ImGui::CreateContext();
    ImGuiIO &io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(g_GlWidth, g_GlHeight);
    ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 300 es"));
    ImGui_ImplAndroid_Init(NULL);
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, 22, 22.0f);

    io.ConfigWindowsMoveFromTitleBarOnly = false;
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    egl.setup = true;
  }

  SetDarkGrayTheme();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
  ImGui::NewFrame();

  if (svaston) {
    SwastonCrosshairv1();
  }
  if (isFov2) {
    auto background = ImGui::GetBackgroundDrawList();
    background->AddCircle({g_GlWidth / 2, g_GlHeight / 2}, aimRadius,
                          ImGui::ColorConvertFloat4ToU32(*(ImVec4 *)aimColor), 0, aimThickness / 20);
  }

  ImGui::SetNextWindowSize(ImVec2((float)g_GlWidth * 0.25f, (float)g_GlHeight * 0.35f), ImGuiCond_Once);
  if (ImGui::Begin(OBFUSCATE("NgocDev FF Việt Nam"), 0, ImGuiWindowFlags_NoBringToFrontOnFocus)) {
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("%s", processName.c_str());

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

      if (tab == 1)

      {
        static bool prev_ResetGuest = false;
        ImGui::Checkbox(OBFUSCATE("Reset Guest"), &ResetGuest);
        if (ResetGuest != prev_ResetGuest) {
          if (ResetGuest) {
            LOGD("Reset Guest [ON]");
            DobbyHook((void *)ResetGuest_OffsetVar, (void *)_ResetGuest, (void **)&old_ResetGuest);
          } else {
            LOGD("Reset Guest [OFF]");
            DobbyDestroy((void *)ResetGuest_OffsetVar);
          }
          prev_ResetGuest = ResetGuest;
        }
      }

      if (tab == 2)

      {
        ImGui::PushItemWidth(300);
        ImGui::Combo(OBFUSCATE("Set Shader"), &shaderrn, shadertype, IM_ARRAYSIZE(shadertype));
        ImGui::PopItemWidth();

        switch (shaderrn) {
          case 0:
            shaderlm = "OFF";
            setShader(shaderlm);
            break;
          case 1:
            shaderlm = "_MainTex";
            setShader(shaderlm);
            break;
          case 2:
            shaderlm = "_BumpMap";
            setShader(shaderlm);
            break;
          case 3:
            shaderlm = "_MainTex_ST";
            setShader(shaderlm);
            break;
          case 4:
            shaderlm = "_Color";
            setShader(shaderlm);
            break;
          case 5:
            shaderlm = "hlslcc_mtx4x4unity_MatrixVP[0]";
            setShader(shaderlm);
            break;
          case 6:
            shaderlm = "hlslcc_mtx4x4unity_ObjectToWorld[0]";
            setShader(shaderlm);
            break;
          case 7:
            shaderlm = "_Flip";
            setShader(shaderlm);
            break;
          case 8:
            shaderlm = "_RendererColor";
            setShader(shaderlm);
            break;
          case 9:
            shaderlm = "_Displacement";
            setShader(shaderlm);
            break;
          case 10:
            shaderlm = "uvOffsetAndScale";
            setShader(shaderlm);
            break;
          case 11:
            shaderlm = "_AtmosphereThickness";
            setShader(shaderlm);
            break;
          case 12:
            shaderlm = "_SkyTint";
            setShader(shaderlm);
            break;
          case 13:
            shaderlm = "_GroundColor";
            setShader(shaderlm);
            break;
          case 14:
            shaderlm = "_Exposure";
            setShader(shaderlm);
            break;
          case 15:
            shaderlm = "_LightColor0";
            setShader(shaderlm);
            break;
          case 16:
            shaderlm = "_WorldSpaceLightPos0";
            setShader(shaderlm);
            break;
          case 17:
            shaderlm = "_SunSize";
            setShader(shaderlm);
            break;
          case 18:
            shaderlm = "_TextureSampleAdd";
            setShader(shaderlm);
            break;
          case 19:
            shaderlm = "_UIMaskSoftnessY";
            setShader(shaderlm);
            break;
          case 20:
            shaderlm = "_UIMaskSoftnessX";
            setShader(shaderlm);
            break;
          case 21:
            shaderlm = "_ClipRect";
            setShader(shaderlm);
            break;
          case 22:
            shaderlm = "hlslcc_mtx4x4glstate_matrix_projection[0]";
            setShader(shaderlm);
            break;
          case 23:
            shaderlm = "_ScreenParams";
            setShader(shaderlm);
            break;
          case 24:
            shaderlm = "sk_RTAdjust";
            setShader(shaderlm);
            break;
          case 25:
            shaderlm = "uTextureSampler_0_Stage0";
            setShader(shaderlm);
            break;
          case 26:
            shaderlm = "uAtlasSizeInv_Stage0";
            setShader(shaderlm);
            break;
          case 27:
            shaderlm = "uTextureSampler_0_Stage0";
            setShader(shaderlm);
            break;
          case 28:
            shaderlm = "unity_SHC";
            setShader(shaderlm);
            break;
        }
        ImGui::Checkbox("Default Chams", &chams);
        if (chams) {
          SetWallhack(true);
        } else {
          SetWallhack(false);
        }
        ImGui::Checkbox("Wireframe Chams", &wireframe);
        if (wireframe) {
          SetWireframe(true);
        } else {
          SetWireframe(false);
        }
        ImGui::Checkbox("Glow Chams", &glow);
        if (glow) {
          SetGlow(true);
        } else {
          SetGlow(false);
        }
        ImGui::Checkbox("Outline Chams", &outline);
        if (outline) {
          SetOutline(true);
        } else {
          SetOutline(false);
        }
        if (ImGui::CollapsingHeader("More Customization")) {
          ImGui::PushItemWidth(300);
          ImGui::SliderInt("Red Color", &setR, -0.0f, 255.0f);
          if (setR) {
            SetR(setR);
          }
          ImGui::SliderInt("Green Color", &setG, -0.0f, 255.0f);
          if (setG) {
            SetG(setG);
          }
          ImGui::SliderInt("Blue Color", &setB, -0.0f, 255.0f);
          if (setB) {
            SetB(setB);
          }
          ImGui::SliderInt("Wireframe Width", &setw1, 1.0f, 10.0f);
          if (setw1) {
            SetWireframeWidth(setw1);
          }
          ImGui::SliderInt("Glow Width", &setw2, 1.0f, 10.0f);
          if (setw2) {
            SetGlowWidth(setw2);
          }
          ImGui::SliderInt("Outline Width", &setw3, 1.0f, 10.0f);
          if (setw3) {
            SetOutlineWidth(setw3);
          }
          ImGui::PopItemWidth();
        }
      }

      if (tab == 3) {
        ImGui::Checkbox("Camera Fov", &fovFeature);
        if (fovFeature) {
          ImGui::SliderInt("Fov value", &fovValue, 360, 0);
        }
        ImGui::Checkbox("Draw fov", &isFov2);
        if (isFov2) {
          ImGui::SameLine();
          ImGui::PushItemWidth(300);
          ImGui::ColorEdit3("Color##Aim", (float *)&aimColor, ImGuiColorEditFlags_NoInputs);
          ImGui::SliderFloat("Fov##Aim", &aimRadius, 0, 360, "%f°");
          ImGui::SliderFloat("Thickness##Aim", &aimThickness, 20, 100, "%f", 0);
          ImGui::PopItemWidth();
        }
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

void hack_thread(pid_t pid) {
  LOGD("Thread PID: %i", pid);
  mlovinit();
  setShader(shaderlm);
  LogShaders();
  Wallhack();

  processName = KittyMemory::getProcessName();

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
  DobbyHook((void *)dlsym(dlopen(OBFUSCATE("libEGL.so"), 4), OBFUSCATE("eglSwapBuffers")), (void *)hook_eglSwapBuffers,
            (void **)&old_eglSwapBuffers);

  do {
    sleep(1);
    LOGD("Searching for libil2cpp.so...");
    g_il2cppELF = ElfScanner::createWithPath("libil2cpp.so");
  } while (!g_il2cppELF.isValid());
  g_Il2cppBase = g_il2cppELF.base();
  LOGD("Search libil2cpp.so DONE");

  if (g_Il2cppBase == 0) {
    LOGE("CRITICAL: libil2cpp.so not found!");
    LOGE("Thread %d terminating", pid);
    std::terminate();
  }

  sleep(5);

  auto ResetGuest      = new LoadClass("COW", OBFUSCATE("GameConfig"));
  ResetGuest_OffsetVar = ResetGuest->GetMethodOffsetByName(OBFUSCATE("get_ResetGuest"), 0);
  A64HookFunction((void *)(g_Il2cppBase + ResetGuest_OffsetVar), (void *)ResetGuest, (void **)&old_ResetGuest);
  LOGD("hack_thread() END");
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
