#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

extern const int base_radius;
extern double    precision;
extern int       precision_points;

typedef pair<int,    int>    pointI;
typedef pair<double, double> pointD;

extern vector<pointI> basepoints;
extern vector<pointD> tmppoints;
extern vector<pointI> bezierpoints;

extern int    width, height;
extern int    viewx, viewy;

extern decltype(basepoints.end()) movingpoint;

extern bool   isRenderSubbezier;
extern bool   isCalculateBezier;
extern bool   isCalculateSubbezier;
extern double subbezierT;