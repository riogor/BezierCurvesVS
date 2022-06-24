#pragma once
#include "pch.h"

bool readFromFile(char* path);

bool saveInFile(char* path);

/*
File format:
Bezier type\n
Points amount\n
<Base points> (one per line)
*/
