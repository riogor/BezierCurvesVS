#include <iostream>
#include <vector>
#include <cmath>

#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

const int    base_radius     = 10;
const double precision       = 0.0005;
const int    precision_points = 1.0 / precision;

typedef pair<int,    int>    pointI;
typedef pair<double, double> pointD;

vector<pointI> basepoints;
vector<pointD> tmppoints;
vector<pointI> bezierpoints;

int    width = 1000, height = 800;
int    viewx = 0, viewy  = 0;

auto   movingpoint          = basepoints.end();
bool   isRenderSubbezier    = false;
bool   isCalculateBezier    = true;
bool   isCalculateSubbezier = false;
double subbezierT           = 0.5;

namespace calculate
{
	auto findPointOnClickPos(int x, int y, int radius)
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

} // namespace calculate

namespace render
{
	void renderCircle(int x, int y, int radius)
	{
		glPointSize(radius);
		glBegin(GL_POINTS);

		glVertex2i(x, y);

		glEnd();
	}

} // namespace render

namespace eventHandler
{
	void error(int error, const char* description)
	{
		cerr << error << " : " << description << "\n";
	}

	void reshape(GLFWwindow* window, int w, int h)
	{
		width = w;
		height = h;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(viewx, width+viewx, height+viewy, viewy, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void display()
	{
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);

		glColor3ub(255, 255, 255);

		for (auto p : basepoints)
			glVertex2i(p.first, p.second);

		glEnd();

		for (auto p : basepoints)
			render::renderCircle(p.first, p.second, base_radius);

		if (isCalculateBezier)
			calculate::calculateBezierCurve();

		if (isRenderSubbezier && !basepoints.empty())
		{
			if (isCalculateSubbezier)
			{
				calculate::calculateBezierPoint(subbezierT, -1);
				isCalculateSubbezier = false;
			}

			srand(0x12e15e35b500f16e);

			int stacked = basepoints.size();
			for (auto layer = basepoints.size() - 1; layer > 1; --layer)
			{
				glBegin(GL_LINE_STRIP);
				glColor3ub(rand() % 255, rand() % 255, rand() % 255);

				for (int point = stacked; point < (stacked + layer); ++point)
					glVertex2i(tmppoints[point].first, tmppoints[point].second);

				glEnd();

				for (int point = stacked; point < (stacked + layer); ++point)
					render::renderCircle(tmppoints[point].first, tmppoints[point].second, base_radius - 1);

				stacked += layer;
			}

			glColor3ub(255, 0, 0);
			render::renderCircle(tmppoints[stacked].first, tmppoints[stacked].second, base_radius+1);
		}

		glLineWidth(2);
		glBegin(GL_LINE_STRIP);

		glColor3ub(255, 0, 0);
		for (auto p : bezierpoints)
			glVertex2i(p.first, p.second);

		glEnd();

		if (!basepoints.empty())
		{
			glColor3ub(255, 255, 255);
			render::renderCircle(basepoints.begin()->first, basepoints.begin()->second, base_radius);
			render::renderCircle(basepoints.end()[-1].first, basepoints.end()[-1].second, base_radius);
		}
	}

	void mouse(GLFWwindow* window, int button, int action, int mode)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		x += viewx;
		y += viewy;

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			auto point_on_click = calculate::findPointOnClickPos(x, y, base_radius);

			if (movingpoint != basepoints.end())
			{
				movingpoint->first = x;
				movingpoint->second = y;

				movingpoint = basepoints.end();

				isCalculateBezier = true;
				isCalculateSubbezier = true;

				return;
			}

			if (point_on_click == basepoints.end())
			{
				cerr << x << " " << y << "\n";
				basepoints.push_back({ x, y });

				movingpoint = basepoints.end();

				isCalculateBezier = true;
				isCalculateSubbezier = true;
			}
			else
				movingpoint = point_on_click;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			auto point_on_click = calculate::findPointOnClickPos(x, y, base_radius);
			if (point_on_click != basepoints.end())
			{
				basepoints.erase(point_on_click);

				movingpoint = basepoints.end();

				isCalculateBezier = true;
				isCalculateSubbezier = true;
			}
		}
	}

	void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		switch (key)
		{
		case GLFW_KEY_Q:
			if (action == GLFW_PRESS)
			{
				isRenderSubbezier = !isRenderSubbezier;
				isCalculateSubbezier = true;
			}

			break;

		case GLFW_KEY_A:
			subbezierT = (subbezierT > precision ? subbezierT - precision : 0);
			isCalculateSubbezier = true;

			break;

		case GLFW_KEY_D:
			subbezierT = (subbezierT < 1.0 ? subbezierT + precision : 1.0);
			isCalculateSubbezier = true;

			break;

		case GLFW_KEY_W:
			subbezierT = (subbezierT < 1.0 ? subbezierT + 0.01 : 1.0);
			isCalculateSubbezier = true;

			break;

		case GLFW_KEY_S:
			subbezierT = (subbezierT > 0.01 ? subbezierT - 0.01 : 0);
			isCalculateSubbezier = true;

			break;

		case GLFW_KEY_UP:
			viewy -= 10;

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(viewx, width + viewx, height + viewy, viewy, -1, 1);

			break;

		case GLFW_KEY_DOWN:
			viewy += 10;

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(viewx, width + viewx, height + viewy, viewy, -1, 1);

			break;

		case GLFW_KEY_RIGHT:
			viewx += 10;

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(viewx, width + viewx, height + viewy, viewy, -1, 1);

			break;

		case GLFW_KEY_LEFT:
			viewx -= 10;

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(viewx, width + viewx, height + viewy, viewy, -1, 1);

			break;
		}
	}
} // namespace eventHandler

int main(int argc, char* argv[])
{
	glfwSetErrorCallback(eventHandler::error);

	if (!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(width, height, "Bezier curves", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, eventHandler::reshape);
	glfwSetKeyCallback(window, eventHandler::keyboard);
	glfwSetMouseButtonCallback(window, eventHandler::mouse);
	glfwSwapInterval(1);
	
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_MULTISAMPLE);		

	eventHandler::reshape(window, width, height);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		eventHandler::display();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}