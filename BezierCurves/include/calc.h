#pragma once

#include "pch.h"

decltype(basepoints.end()) findPointOnClickPos(int x, int y, int radius);

void calculateBezierPoint(const double t, const int point, const int group);
void calculateBezierCurve();