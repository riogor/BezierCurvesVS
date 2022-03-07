#pragma once

#include "pch.h"

decltype(basepoints.end()) findPointOnClickPos(int x, int y, int radius);

void calculateBezierPoint(double t, int point);
void calculateBezierCurve();