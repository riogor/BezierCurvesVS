#pragma once

#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

extern bool  isRenderGrid;
extern bool  isRenderBase;

extern const int max_field;   // maximum screen amount (field size: width*maxfield*2, height*maxfield*2)
extern const int base_radius; // radius of point on screen
extern const int step_size;   // amount of pixels when moving camera
extern int       grid_col;
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

extern int   bezierType;        // 1 - normal, 2 - quadratic (spline), 3 - cubic (spline)
extern bool  isRenderSubbezier;
extern bool  isCalculateBezier;
extern float subbezierT;        // T variable for rendering subbezier