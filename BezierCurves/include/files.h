#pragma once

#include "pch.h"

bool readFromFile(string path);

void saveInFile(string path);

/*
File format:
Bezier type\n
Points amount\n
<Base points> (one per line)
*/