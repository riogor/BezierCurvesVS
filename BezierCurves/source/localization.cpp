#include "pch.h"

#include "localization.h"

namespace localization
{
	const char* const language_list[] = {"English", "Russian"};
	vector<string> labels = {};
	vector<map<string, string>> locales = {};

	void initLocale()
	{
		labels = {
			"title",
			"save",
			"load",
			"control",
			"settings",
			"saveIn",
			"fileName",
			"errorSave",
			"loadFrom",
			"errorLoad",
			"controlPanel",
			"curveType",
			"normal",
			"square",
			"cubic",
			"subcurves",
			"showSubcurves",
			"navigation",
			"home",
			"toCurve",
			"showGrid",
			"showBase",
			"erase",
			"gridColor",
			"precision",
			"apply",
			"reset",
			"lang"
		};

		locales.resize(2);

		locales[0] = {
			{labels[0], "Bezier curves"},
			{labels[1], "Save"},
			{labels[2], "Load"},
			{labels[3], "Control"},
			{labels[4], "Settings"},
			{labels[5], "Save in file"},
			{labels[6], "File name"},
			{labels[7], "Error while saving!"},
			{labels[8], "Load from file"},
			{labels[9], "Error while loading from file!\nVerify file name\nand save format"},
			{labels[10], "Control panel"},
			{labels[11], "Curve type:"},
			{labels[12], "Normal"},
			{labels[13], "Square"},
			{labels[14], "Cubic"},
			{labels[15], "Subcurves:"},
			{labels[16], "Show subcurves"},
			{labels[17], "Navigation:"},
			{labels[18], "Home"},
			{labels[19], "To curve"},
			{labels[20], "Show grid"},
			{labels[21], "Show base points"},
			{labels[22], "Erase all"},
			{labels[23], "Grid color"},
			{labels[24], "Precision"},
			{labels[25], "Apply"},
			{labels[26], "Reset"},
			{labels[27], "Language"}
		};

		locales[1] = {
			{labels[0], u8"Кривые Безье"},
			{labels[1], u8"Сохранить"},
			{labels[2], u8"Загрузить"},
			{labels[3], u8"Управление"},
			{labels[4], u8"Настройки"},
			{labels[5], u8"Сохранить в файл"},
			{labels[6], u8"Имя файла"},
			{labels[7], u8"Ошибка при сохранении в файл!"},
			{labels[8], u8"Загрузить из файла"},
			{labels[9], u8"Ошибка при загрузке из файла!\nПроверьте название\nи правильность формата"},
			{labels[10], u8"Панель управления"},
			{labels[11], u8"Тип кривой:"},
			{labels[12], u8"Обычная"},
			{labels[13], u8"Квадратичная"},
			{labels[14], u8"Кубическая"},
			{labels[15], u8"Подкривые:"},
			{labels[16], u8"Показать подкривые"},
			{labels[17], u8"Навигация:"},
			{labels[18], u8"Домой"},
			{labels[19], u8"К кривой"},
			{labels[20], u8"Показать сетку"},
			{labels[21], u8"Показать опорные точки"},
			{labels[22], u8"Стереть всё"},
			{labels[23], u8"Цвет сетки"},
			{labels[24], u8"Точность"},
			{labels[25], u8"Применить"},
			{labels[26], u8"Сброс"},
			{labels[27], u8"Язык"}
		};
	}

	const char* getLocStr(string name)
	{
		auto locale_string = locales[language].find(name);
		return (locale_string != locales[language].end() ? locale_string->second.c_str() : nullptr);
	}
} //namespace localization
