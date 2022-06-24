#include <pch.h>

#include <handler.h>
#include <files.h>
#include <localization.h>

vector<ImFont*> lang_fonts;

using localization::getLocStr;

int main()
{
	glfwSetErrorCallback(error);

	if (!glfwInit())
		return -1;

	localization::initLocale();

	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(width, height, getLocStr("title"), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouse);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SMOOTH);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;

	io.Fonts->Clear();
	lang_fonts = {io.Fonts->AddFontFromFileTTF("fonts/Roboto-Bold.ttf", 16.f),      //Eng
				  io.Fonts->AddFontFromFileTTF("fonts/Roboto-Bold.ttf", 16.f, NULL, //Rus
					  ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()) };
	io.Fonts->Build();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();

	int ui_precision_points = precision_points;

	bool show_save       = false, show_load       = false;
	bool show_controls   = false, show_settings   = false;
	bool show_error_save = false, show_error_load = false;

	char buf_save[1000] = {}, buf_load[1000] = {};

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(lang_fonts[language]);

		ImGui::BeginMainMenuBar();
		{
			if (ImGui::Button(getLocStr("save")))
				show_save = !show_save;
			if (ImGui::Button(getLocStr("load")))
				show_load = !show_load;
			if (ImGui::Button(getLocStr("control")))
				show_controls = !show_controls;
			if (ImGui::Button(getLocStr("settings")))
				show_settings = !show_settings;
		}
		ImGui::EndMainMenuBar();

		if (show_save)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(getLocStr("saveIn"), (bool*)0, ImGuiWindowFlags_NoResize);
			{
				if (ImGui::InputText(getLocStr("fileName"), buf_save, 1000))
					show_error_save = false;

				if (ImGui::Button(getLocStr("save")))
				{
					if (saveInFile(buf_save))
					{
						show_save = false;
						memset(buf_save, 0, 1000 * sizeof(char));
					}
					else
						show_error_save = true;
				}

				if (show_error_save)
					ImGui::TextColored(ImVec4(255, 25, 25, 1), getLocStr("errorLoad"));
			}
			ImGui::End();
		}

		if (show_load)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(getLocStr("loadFrom"), (bool*)0, ImGuiWindowFlags_NoResize);
			{
				if (ImGui::InputText(getLocStr("fileName"), buf_load, 1000))
					show_error_load = false;

				if (ImGui::Button(getLocStr("load")))
				{
					if (readFromFile(buf_load))
					{
						show_load = false;
						memset(buf_load, 0, 1000 * sizeof(char));
					}
					else
						show_error_load = true;
				}

				if (show_error_load)
					ImGui::TextColored(ImVec4(255, 25, 25, 1), getLocStr("errorLoad"));
			}
			ImGui::End();
		}

		if (show_controls)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(getLocStr("controlPanel"), (bool*)0, ImGuiWindowFlags_NoResize);
			{
				ImGui::Text(getLocStr("curveType"));
				if (ImGui::RadioButton(getLocStr("normal"), &bezierType, 1))
					isCalculateBezier = true;
				ImGui::SameLine();
				if (ImGui::RadioButton(getLocStr("square"), &bezierType, 2))
					isCalculateBezier = true;
				ImGui::SameLine();
				if (ImGui::RadioButton(getLocStr("cubic"), &bezierType, 3))
					isCalculateBezier = true;
				ImGui::Separator();

				ImGui::Text(getLocStr("subcurves"));
				ImGui::Checkbox(getLocStr("showSubcurves"), &isRenderSubbezier);
				ImGui::SliderFloat("T", &subbezierT, 0.0f, 1.0f);
				ImGui::Separator();

				ImGui::Text(getLocStr("navigation"));
				if (ImGui::Button(getLocStr("home"), ImVec2()))
				{
					viewx = -width / 2;
					viewy = -height / 2;
				}
				ImGui::SameLine();
				if (ImGui::Button(getLocStr("toCurve"), ImVec2()) && !basepoints.empty())
				{
					viewx = basepoints[0].first - width / 2;
					viewy = basepoints[0].second - height / 2;
				}
				ImGui::Separator();

				ImGui::Checkbox(getLocStr("showGrid"), &isRenderGrid);
				ImGui::Checkbox(getLocStr("showBase"), &isRenderBase);
				if (ImGui::Button(getLocStr("erase"), ImVec2()))
				{
					basepoints.clear();
					movingpoint = basepoints.end();
					isCalculateBezier = true;
				}
			}
			ImGui::End();
		}

		if (show_settings)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(getLocStr("settings"), (bool*)0, ImGuiWindowFlags_NoResize);
			{
				ImGui::SliderInt(getLocStr("gridColor"), &grid_col, 0, 255);
				ImGui::Separator();

				ImGui::SliderInt(getLocStr("precision"), &ui_precision_points, 100, 5000);
				if (ImGui::Button(getLocStr("apply"), ImVec2()))
				{
					precision_points = ui_precision_points;
					precision = 1.0 / (double)precision_points;
					isCalculateBezier = true;
				}
				ImGui::SameLine();
				if (ImGui::Button(getLocStr("reset"), ImVec2()))
				{
					precision_points = 2000;
					ui_precision_points = 2000;
					precision = 1.0 / (double)precision_points;
					isCalculateBezier = true;
				}
				ImGui::Separator();

				ImGui::ListBox(getLocStr("lang"), &language, localization::language_list, 2);
			}
			ImGui::End();
		}

		ImGui::PopFont();
		
		renderGraphics();

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		reshape(window, width, height);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
