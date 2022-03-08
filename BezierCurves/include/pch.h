#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

extern bool  isRenderBase;

extern const int max_field;
extern const int base_radius;
extern double    precision;
extern int       precision_points;

typedef pair<int,    int>    pointI;
typedef pair<double, double> pointD;

extern vector<pointI>         basepoints;
extern vector<pointD>         tmppoints;
extern vector<vector<pointI>> bezierpoints;

extern int width, height;
extern int viewx, viewy;

extern decltype(basepoints.end()) movingpoint;

extern int   bezierType;
extern bool  isRenderSubbezier;
extern bool  isCalculateBezier;
extern float subbezierT;