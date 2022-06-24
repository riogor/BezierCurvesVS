#include "pch.h"

#include "calc.h"

decltype(basepoints.end()) findPointOnClickPos(const int x, const int y, const int radius)
{
	if (basepoints.empty())
		return basepoints.end();

	for (auto point = basepoints.begin(); point < basepoints.end(); ++point)
		if (((point->first + radius >= x) && (point->first - radius <= x))
			&& ((point->second + radius >= y) && (point->second - radius <= y)))
			return point;

	return basepoints.end();
}

void calculateBezierPoint(const double t, const int point, const int group)
{
	if (basepoints.empty())
		return;

	int group_size = (bezierType == 1 ? basepoints.size() : bezierType+1);

	tmppoints.clear();
	tmppoints.resize((group_size * (group_size + 1)) / 2);

	int st = group * group_size - (group != 0)*group;
	for (int p = st; p < st + group_size; p++)
		tmppoints[p - st] = basepoints[p];

	int stacked = group_size;
	for (auto layer = group_size - 1; layer > 0; --layer)
	{
		for (int layer_point = stacked; layer_point < stacked + layer; ++layer_point)
		{
			tmppoints[layer_point].first = tmppoints[layer_point - layer - 1].first +
				(tmppoints[layer_point - layer].first - tmppoints[layer_point - layer - 1].first) * t;

			tmppoints[layer_point].second = tmppoints[layer_point - layer - 1].second +
				(tmppoints[layer_point - layer].second - tmppoints[layer_point - layer - 1].second) * t;
		}

		stacked += layer;
	}

	if (point != -1) //calculating subbezier
		bezierpoints[group][point] = static_cast<pointI>(tmppoints.end()[-1]);
}

void calculateBezierCurve()
{
	bezierpoints.clear();
	isCalculateBezier = false;

	if (basepoints.empty())
		return;

	if (bezierType == 1)
	{
		bezierpoints.resize(1);
		bezierpoints[0].resize(precision_points + 1);
	}
	else
	{
		bezierpoints.resize((basepoints.size()-1) / bezierType);
		for (auto& group : bezierpoints)
			group.resize(precision_points + 1);
	}

	for (int group = 0; group < (bezierType == 1 ? 1 : (basepoints.size()-1) / bezierType); ++group)
	{
		double t = 0.0;
		for (int point = 0; point <= precision_points; ++point, t += precision)
			calculateBezierPoint(t, point, group);
	}
}
