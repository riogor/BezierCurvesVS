#pragma once

#include "pch.h"

void renderCircle(const int    x, const int    y, const int radius);
void renderCircle(const double x, const double y, const int radius);

void renderGrid(const int gap);

void renderBezierBase();
void renderBezierCurve();
void renderSubbezier();