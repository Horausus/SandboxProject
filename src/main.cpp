#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "SandboxProject");

#pragma region imgui
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2;
#pragma endregion

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

#pragma region imgui
		rlImGuiBegin();

		//docking stuff...
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

		DrawRectangle(75, 75, 100, 100, {0, 255, 0, 127});
		DrawRectangle(50, 50, 100, 100, { 255, 0, 0, 127 });

#pragma region imgui
#pragma region imgui window
		ImGui::Begin("test");

		ImGui::Text("hello");
		
		if (ImGui::Button("Button"))
		{
			std::cout << "Button Pressed!" << std::endl;
		}
		ImGui::SameLine();
		if (ImGui::Button("Button2"))
		{
			std::cout << "Second Button" << std::endl;
		}

		ImGui::End();

		ImGui::Begin("second window");
		ImGui::Text("Hello");
		ImGui::Separator();
		ImGui::NewLine();
		
		ImGui::Text("Speed");
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Controls how fast the player moves.");
			ImGui::EndTooltip();
		}
		static float a = 0;
		ImGui::SliderFloat("slider", &a, 0, 1);

		static char buffer[128] = "";

		ImGuiInputTextFlags flags =
			ImGuiInputTextFlags_CharsNoBlank |        // no spaces
			ImGuiInputTextFlags_EnterReturnsTrue |    // return true on Enter
			ImGuiInputTextFlags_AutoSelectAll;        // select all text when focused

		if (ImGui::InputText("Player Name", buffer, sizeof(buffer), flags))
		{
			std::cout << "Confirmed name: " << buffer << "\n";
		}
		

		ImGui::End();
#pragma endregion

		//ImGui::ShowDemoWindow();

		rlImGuiEnd();
#pragma endregion

		EndDrawing();
	}

#pragma	region imgui
	rlImGuiShutdown();
#pragma endregion

	CloseWindow();
	return 0;
}