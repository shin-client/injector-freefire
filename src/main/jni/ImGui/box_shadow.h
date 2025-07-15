#pragma once

#include "imgui.h"

struct ImVec2;
struct ImVec4;
class ImDrawList;

struct RectangleShadowSettings {
  bool    linear;
  float   sigma;
  ImVec2  padding;
  ImVec2  rectPos;
  ImVec2  rectSize;
  ImVec2  shadowOffset;
  ImVec2  shadowSize;
  ImColor shadowColor;
  int     rings;
  int     spacingBetweenRings;
  int     samplesPerCornerSide;
  int     spacingBetweenSamples;
  int     totalVertices;
  int     totalIndices;
  bool    enableDebugVisualization;
};

extern RectangleShadowSettings shadowSettings;

ImVec4 boxGaussianIntegral(ImVec4 x);
ImVec4 boxLinearInterpolation(ImVec4 x);
float  boxShadow(ImVec2 lower, ImVec2 upper, ImVec2 point, float sigma, bool linearInterpolation);
void   drawRectangleShadowVerticesAdaptive(RectangleShadowSettings& settings, bool foreground = false);
void   drawShadowTestExampleWindow();
