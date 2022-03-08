#include "pch.h"

bool  isRenderGrid      = true;
bool  isRenderBase      = true;

const int max_field        = 5;
const int base_radius      = 10;
double    precision        = 0.0005;
int       precision_points = 1.0 / precision;

vector<pointI>         basepoints   = {};
vector<pointD>         tmppoints    = {};
vector<vector<pointI>> bezierpoints = {};

int width = 1000,     height = 800;
int viewx = -width/2, viewy  = -height/2;

decltype(basepoints.end()) movingpoint = basepoints.end();

int   bezierType        = 1; // 1 - normal, 2 - quadratic (spline), 3 - cubic (spline)
bool  isCalculateBezier = true;
bool  isRenderSubbezier = false;
float subbezierT        = 0.5f;