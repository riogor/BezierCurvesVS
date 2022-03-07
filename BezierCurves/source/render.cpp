#include "pch.h"
#include "render.h"

void renderCircle(int x, int y, int radius)
{
	glPointSize(radius);
	glBegin(GL_POINTS);

	glVertex2i(x, y);

	glEnd();
}

void renderBezierBase()
{
	glLineWidth(2);
	glColor3ub(255, 255, 255);
	glBegin(GL_LINE_STRIP);

	for (auto& p : basepoints)
		glVertex2i(p.first, p.second);

	glEnd();

	for (auto& p : basepoints)
		renderCircle(p.first, p.second, base_radius);
}

void renderBezierCurve()
{
	glLineWidth(2);
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);

	for (auto& p : bezierpoints)
		glVertex2i(p.first, p.second);

	glEnd();

	if (!basepoints.empty())
	{
		glColor3ub(255, 255, 255);
		renderCircle(basepoints.begin()->first, basepoints.begin()->second, base_radius);
		renderCircle(basepoints.end()[-1].first, basepoints.end()[-1].second, base_radius);
	}
}

void renderSubbezier()
{
	srand(0x12e15e35b500f16e);

	int stacked = basepoints.size();
	for (auto layer = basepoints.size() - 1; layer > 1; --layer)
	{
		glColor3ub(rand() % 255, rand() % 255, rand() % 255);
		glBegin(GL_LINE_STRIP);

		for (int point = stacked; point < (stacked + layer); ++point)
			glVertex2i(tmppoints[point].first, tmppoints[point].second);

		glEnd();

		for (int point = stacked; point < (stacked + layer); ++point)
			renderCircle(tmppoints[point].first, tmppoints[point].second, base_radius - 1);

		stacked += layer;
	}

	if (basepoints.size() > 1)
	{
		glColor3ub(255, 0, 0);
		renderCircle(tmppoints[stacked].first, tmppoints[stacked].second, base_radius + 1);
	}
}