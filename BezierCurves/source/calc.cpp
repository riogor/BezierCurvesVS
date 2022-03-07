#include "pch.h"
#include "calc.h"

decltype(basepoints.end()) findPointOnClickPos(int x, int y, int radius)
{
	for (auto point = basepoints.begin(); point < basepoints.end(); ++point)
		if (((point->first + radius >= x) && (point->first - radius <= x))
			&& ((point->second + radius >= y) && (point->second - radius <= y)))
			return point;

	return basepoints.end();
}

void calculateBezierPoint(double t, int point)
{
	if (basepoints.empty())
		return;

	tmppoints.clear();
	tmppoints.resize((basepoints.size() * (basepoints.size() + 1)) / 2);

	for (int p = 0; p < basepoints.size(); p++)
		tmppoints[p] = basepoints[p];

	int stacked = basepoints.size();
	for (auto layer = basepoints.size() - 1; layer > 0; --layer)
	{
		for (size_t point = stacked; point < stacked + layer; ++point)
		{
			tmppoints[point].first = tmppoints[point - layer - 1].first +
				(tmppoints[point - layer].first - tmppoints[point - layer - 1].first) * t;

			tmppoints[point].second = tmppoints[point - layer - 1].second +
				(tmppoints[point - layer].second - tmppoints[point - layer - 1].second) * t;
		}

		stacked += layer;
	}

	if (point != -1) //calculating subbezier
		bezierpoints[point] = tmppoints.end()[-1];
}

void calculateBezierCurve()
{
	bezierpoints.clear();
	bezierpoints.resize(precision_points + 1);

	double t = 0.0;
	for (int point = 0; point <= precision_points; ++point, t += precision)
		calculateBezierPoint(t, point);

	isCalculateBezier = false;
}