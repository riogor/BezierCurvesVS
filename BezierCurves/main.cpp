#include <pch.h>

#include <handler.h>
#include <files.h>

int main()
{
	glfwSetErrorCallback(error);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(width, height, u8"Кривые Безье", NULL, NULL);
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
	ImFont* rus = io.Fonts->AddFontFromFileTTF("fonts/consolas.ttf", 16.f, NULL,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();

	int ui_precision_points = precision_points;
	bool show_save     = false, show_load     = false;
	bool show_controls = false, show_settings = false;
	bool show_error_save = false, show_error_load = false;
	char buf[1000] = {};

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(rus);

		ImGui::BeginMainMenuBar();
		{
			if (ImGui::Button(u8"Сохранить"))
				show_save = !show_save;
			if (ImGui::Button(u8"Загрузить"))
				show_load = !show_load;
			if (ImGui::Button(u8"Управление"))
				show_controls = !show_controls;
			if (ImGui::Button(u8"Настройки"))
				show_settings = !show_settings;
		}
		ImGui::EndMainMenuBar();

		if (show_save)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(u8"Сохранить в файл", (bool*)0, ImGuiWindowFlags_NoResize);
			{
				if (ImGui::InputText(u8"Имя файла", buf, 1000))
					show_error_save = false;

				if (ImGui::Button(u8"Сохранить"))
				{
					if (saveInFile(buf))
					{
						show_save = false;
						memset(buf, 0, 1000 * sizeof(char));
					}
					else
						show_error_save = true;
				}

				if (show_error_save)
					ImGui::TextColored(ImVec4(255, 25, 25, 1), u8"Ошибка при сохранении!");
			}
			ImGui::End();
		}

		if (show_load)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(u8"Загрузить из файла", (bool*)0, ImGuiWindowFlags_NoResize);
			{
				if (ImGui::InputText(u8"Имя файла", buf, 1000))
					show_error_load = false;

				if (ImGui::Button(u8"Загрузить"))
				{
					if (readFromFile(buf))
					{
						show_load = false;
						memset(buf, 0, 1000 * sizeof(char));
					}
					else
						show_error_load = true;
				}

				if (show_error_load)
					ImGui::TextColored(ImVec4(255, 25, 25, 1), u8"Ошибка при загрузке из файла!\nПроверьте название\nи правильность формата");
			}
			ImGui::End();
		}

		if (show_controls)
		{
			ImGui::SetNextWindowSize(ImVec2());
			ImGui::Begin(u8"Панель управления", (bool*)0, ImGuiWindowFlags_NoResize);
			{
				ImGui::Text(u8"Тип кривой:");
				if (ImGui::RadioButton(u8"Обычная", &bezierType, 1))
					isCalculateBezier = true;
				ImGui::SameLine();
				if (ImGui::RadioButton(u8"Квадратичная", &bezierType, 2))
					isCalculateBezier = true;
				ImGui::SameLine();
				if (ImGui::RadioButton(u8"Кубическая", &bezierType, 3))
					isCalculateBezier = true;
				ImGui::Separator();

				ImGui::Checkbox(u8"Показать подкривые", &isRenderSubbezier);
				ImGui::SliderFloat("T", &subbezierT, 0.0f, 1.0f);
				ImGui::Separator();

				ImGui::Text(u8"Навигация");
				if (ImGui::Button(u8"Домой", ImVec2()))
				{
					viewx = -width / 2;
					viewy = -height / 2;
				}
				ImGui::SameLine();
				if (ImGui::Button(u8"К кривой", ImVec2()) && !basepoints.empty())
				{
					viewx = basepoints[0].first - width / 2;
					viewy = basepoints[0].second - height / 2;
				}
				ImGui::Separator();

				ImGui::Checkbox(u8"Показать сетку", &isRenderGrid);
				ImGui::Checkbox(u8"Показать опорные точки", &isRenderBase);
				if (ImGui::Button(u8"Стереть всё", ImVec2()))
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
			ImGui::Begin(u8"Настройки", (bool*)0, ImGuiWindowFlags_NoResize);
			{
				ImGui::SliderInt(u8"Цвет сетки", &grid_col, 0, 255);
				ImGui::Separator();

				ImGui::SliderInt(u8"Точность", &ui_precision_points, 100, 5000);
				if (ImGui::Button(u8"Применить", ImVec2()))
				{
					precision_points = ui_precision_points;
					precision = 1.0 / (double)precision_points;
					isCalculateBezier = true;
				}
				ImGui::SameLine();
				if (ImGui::Button(u8"Сброс", ImVec2()))
				{
					precision_points = 2000;
					ui_precision_points = 2000;
					precision = 1.0 / (double)precision_points;
					isCalculateBezier = true;
				}
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