#include "pch.h"

bool  isRenderGrid      = true;
bool  isRenderBase      = true;

const int max_field        = 5;
const int base_radius      = 10;
const int step_size        = 25;
int       grid_col         = 55;
double    precision        = 0.0005;
int       precision_points = 1.0 / precision;

vector<pointI>         basepoints   = {};
vector<pointD>         tmppoints    = {};
vector<vector<pointI>> bezierpoints = {};

int width = 1000,     height = 800;
int viewx = -width/2, viewy  = -height/2;

decltype(basepoints.end()) movingpoint = basepoints.end();

int   bezierType        = 1;
bool  isCalculateBezier = true;
bool  isRenderSubbezier = false;
float subbezierT        = 0.5f;