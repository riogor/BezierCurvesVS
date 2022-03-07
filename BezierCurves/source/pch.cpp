#include "pch.h"

const int base_radius = 10;
double    precision = 0.0005;
int       precision_points = 1.0 / precision;

vector<pointI> basepoints   = {};
vector<pointD> tmppoints    = {};
vector<pointI> bezierpoints = {};

int width = 1000, height = 800;
int viewx = 0,    viewy  = 0;

decltype(basepoints.end()) movingpoint = basepoints.end();

bool   isCalculateBezier    = true;
bool   isRenderSubbezier    = false;
bool   isCalculateSubbezier = false;
double subbezierT           = 0.5;