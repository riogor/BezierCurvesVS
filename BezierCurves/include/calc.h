#pragma once

#include "pch.h"

decltype(basepoints.end()) findPointOnClickPos(const int x, const int y, const int radius);

void calculateBezierPoint(const double t, const int point, const int group);
void calculateBezierCurve();