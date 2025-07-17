// GUI and input handling for Android native application
// Provides ImGui integration, JNI utilities, and system service access

#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <Struct/ESP.h>
#include <dlfcn.h>
#include <jni.h>

#include <vector>

#include "ImGui/fonts/Roboto-Regular.h"
#include "imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "main.h"

// Global variables
inline static bool g_IsSetup = false;
inline int         prevWidth, prevHeight;
inline float       density = -1;
inline ImFont     *font;

// JNI global references
extern JavaVM  *jvm;
extern JNIEnv  *genv;
extern jclass   UnityPlayer_cls;
extern jfieldID UnityPlayer_CurrentActivity_fid;

// Input hook function - captures input events for ImGui
inline void (*origInput)(void *, void *, void *);

inline void myInput(void *thiz, void *ex_ab, void *ex_ac) {
  origInput(thiz, ex_ab, ex_ac);
  ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
  return;
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
  colors[ImGuiCol_TabActive]             = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
  colors[ImGuiCol_TabUnfocused]          = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
  colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.220f, 0.220f, 0.220f, 1.000f);
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
  style->FrameRounding     = 2.3f;
  style->GrabMinSize       = 10.0f;
  style->PopupRounding     = 2.0f;
  style->ScrollbarRounding = 12.0f;
  style->ScrollbarSize     = 20.0f;
  style->TabBorderSize     = 1.0f;
  style->TabRounding       = 4.0f;
  style->WindowRounding    = 6.0f;
  style->WindowTitleAlign  = ImVec2(0.5, 0.5);
  style->FramePadding      = ImVec2(8, 6);
  style->ScaleAllSizes(1.5f);

  // Enhanced tab styling for fixed appearance
  style->TabMinWidthForCloseButton = 0.0f;
  style->WindowPadding             = ImVec2(8.0f, 8.0f);
  style->ItemSpacing               = ImVec2(8.0f, 4.0f);
}

void SetupImgui() {
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();
  ImGuiIO    &io    = ImGui::GetIO();
  ImGuiStyle *style = &ImGui::GetStyle();
  io.DisplaySize    = ImVec2((float)g_GlWidth, (float)g_GlHeight);

  // ImGui::StyleColorsDark();

  SetDarkGrayTheme();

  ImGui_ImplOpenGL3_Init("#version 300 es");

  ImFontConfig font_cfg;
  font_cfg.SizePixels = 22.0f;

  ImFont *roboto_font = io.Fonts->AddFontFromMemoryTTF((void *)Roboto_Regular, sizeof(Roboto_Regular), 22.0f, &font_cfg,
                                                     io.Fonts->GetGlyphRangesDefault());

  if (!roboto_font) {
    io.Fonts->AddFontDefault(&font_cfg);
  }
}

JavaVM *jvm;
JNIEnv *genv;

std::string getClipboard() {
  std::string result;
  JNIEnv     *env = nullptr;

  if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
    return result;  // Return empty string on failure
  }

  jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
  if (looperClass) {
    jmethodID prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    if (prepareMethod) {
      env->CallStaticVoidMethod(looperClass, prepareMethod);
    }
    env->DeleteLocalRef(looperClass);
  }

  jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
  if (activityThreadClass) {
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(
        activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    if (sCurrentActivityThreadField) {
      jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
      if (sCurrentActivityThread) {
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"),
                                                            OBFUSCATE("Landroid/app/Application;"));
        if (mInitialApplicationField) {
          jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
          if (mInitialApplication) {
            jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
            if (contextClass) {
              jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"),
                                                                  OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
              if (getSystemServiceMethod) {
                jstring str = env->NewStringUTF(OBFUSCATE("clipboard"));
                if (str) {
                  jobject clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
                  env->DeleteLocalRef(str);

                  if (clipboardManager) {
                    jclass ClipboardManagerClass = env->FindClass(OBFUSCATE("android/content/ClipboardManager"));
                    if (ClipboardManagerClass) {
                      jmethodID getText = env->GetMethodID(ClipboardManagerClass, OBFUSCATE("getText"),
                                                           OBFUSCATE("()Ljava/lang/CharSequence;"));
                      if (getText) {
                        jclass CharSequenceClass = env->FindClass(OBFUSCATE("java/lang/CharSequence"));
                        if (CharSequenceClass) {
                          jmethodID toStringMethod = env->GetMethodID(CharSequenceClass, OBFUSCATE("toString"),
                                                                      OBFUSCATE("()Ljava/lang/String;"));
                          if (toStringMethod) {
                            jobject text = env->CallObjectMethod(clipboardManager, getText);
                            if (text) {
                              jstring textStr = (jstring)env->CallObjectMethod(text, toStringMethod);
                              if (textStr) {
                                const char *utf8_str = env->GetStringUTFChars(textStr, 0);
                                if (utf8_str) {
                                  result = std::string(utf8_str);
                                  env->ReleaseStringUTFChars(textStr, utf8_str);
                                }
                                env->DeleteLocalRef(textStr);
                              }
                              env->DeleteLocalRef(text);
                            }
                          }
                          env->DeleteLocalRef(CharSequenceClass);
                        }
                      }
                      env->DeleteLocalRef(ClipboardManagerClass);
                    }
                    env->DeleteLocalRef(clipboardManager);
                  }
                }
              }
              env->DeleteLocalRef(contextClass);
            }
            env->DeleteLocalRef(mInitialApplication);
          }
        }
        env->DeleteLocalRef(sCurrentActivityThread);
      }
    }
    env->DeleteLocalRef(activityThreadClass);
  }

  jvm->DetachCurrentThread();
  return result;
}

jclass   UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;

jobject getGlobalContext() {
  if (!genv) return nullptr;

  jclass activityThread = genv->FindClass(OBFUSCATE("android/app/ActivityThread"));
  if (!activityThread) return nullptr;

  jmethodID currentActivityThread = genv->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"),
                                                            OBFUSCATE("()Landroid/app/ActivityThread;"));
  if (!currentActivityThread) {
    genv->DeleteLocalRef(activityThread);
    return nullptr;
  }

  jobject at = genv->CallStaticObjectMethod(activityThread, currentActivityThread);
  if (!at) {
    genv->DeleteLocalRef(activityThread);
    return nullptr;
  }

  jmethodID getApplication =
      genv->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
  if (!getApplication) {
    genv->DeleteLocalRef(at);
    genv->DeleteLocalRef(activityThread);
    return nullptr;
  }

  jobject context = genv->CallObjectMethod(at, getApplication);

  // Cleanup local references
  genv->DeleteLocalRef(at);
  genv->DeleteLocalRef(activityThread);

  return context;
}

void displayKeyboard(bool pShow) {
  if (!genv) return;  // Safety check

  jclass ctx = genv->FindClass(OBFUSCATE("android/content/Context"));
  if (!ctx) return;

  jfieldID fid = genv->GetStaticFieldID(ctx, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
  if (!fid) {
    genv->DeleteLocalRef(ctx);
    return;
  }

  jmethodID mid =
      genv->GetMethodID(ctx, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
  if (!mid) {
    genv->DeleteLocalRef(ctx);
    return;
  }

  jobject context = genv->GetStaticObjectField(UnityPlayer_cls, UnityPlayer_CurrentActivity_fid);
  if (!context) {
    genv->DeleteLocalRef(ctx);
    return;
  }

  jobject InputManObj = genv->CallObjectMethod(context, mid, (jstring)genv->GetStaticObjectField(ctx, fid));
  if (!InputManObj) {
    genv->DeleteLocalRef(ctx);
    return;
  }

  jclass ClassInputMethodManager = genv->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
  if (!ClassInputMethodManager) {
    genv->DeleteLocalRef(InputManObj);
    genv->DeleteLocalRef(ctx);
    return;
  }

  jmethodID toggleSoftInputId =
      genv->GetMethodID(ClassInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
  if (!toggleSoftInputId) {
    genv->DeleteLocalRef(ClassInputMethodManager);
    genv->DeleteLocalRef(InputManObj);
    genv->DeleteLocalRef(ctx);
    return;
  }

  if (pShow) {
    genv->CallVoidMethod(InputManObj, toggleSoftInputId, 2, 0);
  } else {
    genv->CallVoidMethod(InputManObj, toggleSoftInputId, 0, 0);
  }

  // Cleanup
  genv->DeleteLocalRef(ClassInputMethodManager);
  genv->DeleteLocalRef(InputManObj);
  genv->DeleteLocalRef(ctx);
}

int ShowSoftKeyboardInput() {
  jint    result = 0;  // Initialize result to avoid undefined behavior
  JNIEnv *env;

  if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
    return -1;  // Failed to attach thread
  }

  jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
  if (looperClass) {
    jmethodID prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
    if (prepareMethod) {
      env->CallStaticVoidMethod(looperClass, prepareMethod);
    }
    env->DeleteLocalRef(looperClass);
  }

  jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
  if (activityThreadClass) {
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(
        activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
    if (sCurrentActivityThreadField) {
      jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
      if (sCurrentActivityThread) {
        jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"),
                                                            OBFUSCATE("Landroid/app/Application;"));
        if (mInitialApplicationField) {
          jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
          if (mInitialApplication) {
            jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
            if (contextClass) {
              jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(
                  contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
              if (fieldINPUT_METHOD_SERVICE) {
                jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
                if (INPUT_METHOD_SERVICE) {
                  jmethodID getSystemServiceMethod = env->GetMethodID(
                      contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
                  if (getSystemServiceMethod) {
                    jobject callObjectMethod =
                        env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
                    if (callObjectMethod) {
                      jclass classInputMethodManager =
                          env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
                      if (classInputMethodManager) {
                        jmethodID toggleSoftInputId =
                            env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
                        if (toggleSoftInputId) {
                          // Show keyboard with proper flags
                          env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, 0);
                          result = 1;  // Success
                        }
                        env->DeleteLocalRef(classInputMethodManager);
                      }
                      env->DeleteLocalRef(callObjectMethod);
                    }
                  }
                  env->DeleteLocalRef(INPUT_METHOD_SERVICE);
                }
              }
              env->DeleteLocalRef(contextClass);
            }
            env->DeleteLocalRef(mInitialApplication);
          }
        }
        env->DeleteLocalRef(sCurrentActivityThread);
      }
    }
    env->DeleteLocalRef(activityThreadClass);
  }

  jvm->DetachCurrentThread();
  return result;
}

struct UnityEngine_Vector2_Fields {
  float x;
  float y;
};

struct UnityEngine_Vector2_o {
  UnityEngine_Vector2_Fields fields;
};

enum TouchPhase { Began = 0, Moved = 1, Stationary = 2, Ended = 3, Canceled = 4 };

struct UnityEngine_Touch_Fields {
  int32_t                      m_FingerId;
  struct UnityEngine_Vector2_o m_Position;
  struct UnityEngine_Vector2_o m_RawPosition;
  struct UnityEngine_Vector2_o m_PositionDelta;
  float                        m_TimeDelta;
  int32_t                      m_TapCount;
  int32_t                      m_Phase;
  int32_t                      m_Type;
  float                        m_Pressure;
  float                        m_maximumPossiblePressure;
  float                        m_Radius;
  float                        m_RadiusVariance;
  float                        m_AltitudeAngle;
  float                        m_AzimuthAngle;
};
