#ifndef NORKA_CHAMS
#define NORKA_CHAMS
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <dlfcn.h>

#include "RGB.h"
#include "main.h"

static void       *handle;
static const char *shaderName;
static bool        enableWallhack;
static bool        enableWallhackW;
static bool        enableWallhackG;
static bool        enableWallhackO;
static bool        enableRainbow;
static bool        isRGB;
static float       r  = 0.0f;
static float       g  = 0.0f;
static float       b  = 0.0f;
static int         w1 = 1;
static float       w2 = 1.0f;
static float       w3 = 1.0f;
static int         a  = 255;

float red  = 0.0f;
float gren = 0.0f;
float blue = 0.0f;
float mi   = 0.0f;

void setShader(const char *s) {
  handle     = dlopen("libGLESv2.so", RTLD_LAZY);
  shaderName = s;
}

const char *getShader() { return shaderName; }

void SetWallhack(bool enable) { enableWallhack = enable; }

void SetWireframe(bool enable) { enableWallhackW = enable; }

void SetGlow(bool enable) { enableWallhackG = enable; }

void SetOutline(bool enable) { enableWallhackO = enable; }

void SetRainbow(bool enable) { enableRainbow = enable; }

void SetRGB(bool enable) { isRGB = enable; }

void SetR(int set) { r = set; }

void SetG(int set) { g = set; }

void SetB(int set) { b = set; }

void SetWireframeWidth(int set) { w1 = set; }

void SetGlowWidth(int set) { w2 = set; }

void SetOutlineWidth(int set) { w3 = set; }

bool getWallhackEnabled() { return enableWallhack; }

bool getWireframeEnabled() { return enableWallhackW; }

bool getGlowEnabled() { return enableWallhackG; }

bool getOutlineEnabled() { return enableWallhackO; }

bool getRainbowEnabled() { return enableRainbow; }

bool getRGB() { return isRGB; }

int (*old_glGetUniformLocation)(GLuint, const GLchar *);

GLint new_glGetUniformLocation(GLuint program, const GLchar *name) { return old_glGetUniformLocation(program, name); }

bool isCurrentShader(const char *shader) {
  GLint currProgram;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
  return old_glGetUniformLocation(currProgram, shader) != -1;
}

void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);

void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
  old_glDrawElements(mode, count, type, indices);
  if (mode != GL_TRIANGLES || count < 1000) return;
  {
    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    GLint id = old_glGetUniformLocation(currProgram, getShader());
    if (id == -1) return;
    if (getWireframeEnabled()) {
      if (enableWallhackW) {
        glDepthRangef(1, 0.5);
      } else {
        glDepthRangef(0.5, 1);
      }
      glBlendColor(GLfloat(r / 255), GLfloat(g / 255), GLfloat(b / 255), 1);
      glColorMask(1, 1, 1, 1);
      glEnable(GL_BLEND);
      glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
      glLineWidth(w1);
      performRGBChange();
      old_glDrawElements(GL_LINE_LOOP, count, type, indices);
      if (isRGB) {
        glBlendColor(redd * 3.92156863 * 0.001, greenn * 3.92156863 * 0.001, bluee * 3.92156863 * 0.001, 1.000);
      }
    }

    if (getWallhackEnabled()) {
      glDepthRangef(1, 0.5);
      performRGBChange();
      glBlendColor(GLfloat(r / 255), GLfloat(g / 255), GLfloat(b / 255), 1);
      glColorMask(1, 1, 1, 1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glBlendEquation(GL_FUNC_ADD);
      if (isRGB) {
        glBlendColor(redd * 3.92156863 * 0.001, greenn * 3.92156863 * 0.001, bluee * 3.92156863 * 0.001, 1.000);
      }
    }

    if (getGlowEnabled()) {
      glEnable(GL_BLEND);
      performRGBChange();
      glBlendColor(GLfloat(r / 255), GLfloat(g / 255), GLfloat(b / 255), 1);
      glColorMask(1, 1, 1, 1);
      glEnable(GL_BLEND);
      glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
      glLineWidth(w2);
      glDepthRangef(0.5, 1);
      performRGBChange();
      old_glDrawElements(GL_LINES, count, type, indices);
      glBlendColor(1, 1, 1, 1);
      glDepthRangef(1, 0.5);
      old_glDrawElements(GL_TRIANGLES, count, type, indices);
      if (isRGB) {
        glBlendColor(redd * 3.92156863 * 0.001, greenn * 3.92156863 * 0.001, bluee * 3.92156863 * 0.001, 1.000);
      }
    }

    if (getOutlineEnabled()) {
      glDepthRangef(1, 0);
      performRGBChange();
      glLineWidth(w3);
      glEnable(GL_BLEND);
      glColorMask(1, 1, 1, 1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
      glBlendColor(0, 0, 0, 1);
      old_glDrawElements(GL_TRIANGLES, count, type, indices);
      glBlendColor(GLfloat(r / 255), GLfloat(g / 255), GLfloat(b / 255), 1);
      glDepthRangef(1, 0.5);
      glBlendColor(GLfloat(float(r) / 255), GLfloat(float(g) / 255), GLfloat(float(b) / 255), 1);
      old_glDrawElements(GL_LINES, count, type, indices);
      if (isRGB) {
        glBlendColor(redd * 3.92156863 * 0.001, greenn * 3.92156863 * 0.001, bluee * 3.92156863 * 0.001, 1.000);
      }
    }

    if (getRainbowEnabled()) {
      if (red == 255) {
        if (blue == 0) {
          if (gren == 255) {
          } else {
            gren = gren + 1;
          }
        }
      }
      if (gren == 255) {
        if (red == 0) {
        } else {
          red = red - 1;
        }
      }
      if (gren == 255) {
        if (red == 0) {
          if (blue == 255) {
          } else {
            blue = blue + 1;
          }
        }
      }
      if (blue == 255) {
        if (gren == 0) {
          mi  = 1;
          red = red + 1;
        } else {
          gren = gren - 1;
        }
      }
      if (mi == 1) {
        if (red == 255) {
          if (blue == 0) {
          } else {
            blue = blue - 1;
          }
        }
      }
      SetR(red);
      SetG(gren);
      SetB(blue);
    }

    old_glDrawElements(mode, count, type, indices);
    glDepthRangef(0.5, 1);
    glColorMask(1, 1, 1, 1);
    glDisable(GL_BLEND);
  }
}

bool mlovinit() {
  handle = NULL;
  handle = dlopen("libGLESv2.so", RTLD_LAZY);
  if (!handle) {
    return false;
  }
  return true;
}

void LogShaders() {
  handle                             = dlopen("libGLESv2.so", RTLD_LAZY);
  auto        p_glGetUniformLocation = (const void *(*)(...))dlsym(handle, "glGetUniformLocation");
  const char *dlsym_error            = dlerror();
  if (dlsym_error) {
    return;
  } else {
    DobbyHook(reinterpret_cast<void *>(p_glGetUniformLocation), reinterpret_cast<void *>(new_glGetUniformLocation),
              reinterpret_cast<void **>(&old_glGetUniformLocation));
  }
}

void Wallhack() {
  handle                       = dlopen("libGLESv2.so", RTLD_LAZY);
  auto        p_glDrawElements = (const void *(*)(...))dlsym(handle, "glDrawElements");
  const char *dlsym_error      = dlerror();
  if (dlsym_error) {
    return;
  } else {
    DobbyHook(reinterpret_cast<void *>(p_glDrawElements), reinterpret_cast<void *>(new_glDrawElements),
              reinterpret_cast<void **>(&old_glDrawElements));
  }
}

#endif
