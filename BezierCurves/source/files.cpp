#include "pch.h"

#include "files.h"

#include <fstream>

template<typename T, typename V>
istream& operator>>(istream& in, pair<T, V>& p)
{
	in >> p.first >> p.second;

	return in;
}

template<typename T, typename V>
ostream& operator<<(ostream& out, pair<T, V>& p)
{
	out << p.first << " " << p.second;

	return out;
}

bool readFromFile(char* path)
{
	ifstream in;
	in.open(path);

	if (!in.is_open())
		return false;

	int buftype;
	in >> buftype;
	if (buftype != 1 && buftype != 2 && buftype != 3)
		return false;

	int size;
	in >> size;

	vector<pointI> bufpoints(size, {0,0});

	int count;
	for (count = 0; count < size && in.peek() != EOF; ++count)
		in >> bufpoints[count];

	if (count != size)
		return false;

	basepoints = bufpoints;
	bezierType = buftype;
	isCalculateBezier = true;

	in.close();

	return true;
}

bool saveInFile(char* path)
{
	ofstream out;
	out.open(path);

	if (!out.is_open())
		return false;

	out << bezierType << endl << basepoints.size() << endl;
	
	for (auto& p : basepoints)
		out << p << endl;

	out.close();

	return true;
}
