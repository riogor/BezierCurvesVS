#pragma once
#include "pch.h"

namespace localization
{
	extern const char* const language_list[];
	extern vector<string> labels;
	extern vector<map<string, string>> locales;

	void initLocale();

	const char* getLocStr(string name);
} //namespace localization
