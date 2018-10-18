#include "DockingSpace.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "imgui_docking.h"
#include "imgui.h"

DockingSpace::DockingSpace(float2 _margins_left_up, float2 _margins_right_down)
{
	margins_left_up = _margins_left_up;
	margins_right_down = _margins_right_down;
}

DockingSpace::~DockingSpace()
{
}

void DockingSpace::Start()
{
	font = App->editor->GetLoadedFont("RobotoBold_17");
}

void DockingSpace::CleanUp()
{
}

void DockingSpace::DrawEditor()
{

	//if (ImGui::Begin("DockingSpace", &opened, flags))
	//{
	//	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	//	ImGui::DockSpace(dockspace_id, ImVec2(docking_size.x - 15, docking_size.y - 15));

	//	docking_id = ImGui::GetWindowDockId();
	//}
	//ImGui::End();

	//ImGui::PopFont();
}

void DockingSpace::BeginDockSpace()
{
	float2 window_size = App->window->GetWindowSize();

	float2 docking_pos = float2(margins_left_up.x - 5, margins_left_up.y - 5);
	float2 docking_size = float2(window_size.x - margins_right_down.x + 20 - margins_left_up.x,
		window_size.y - margins_right_down.y + 20 - margins_left_up.y);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(docking_pos.x, docking_pos.y));
	ImGui::SetNextWindowSize(ImVec2(docking_size.x, docking_size.y));

	ImGui::PushFont(font);

	//bool opened = true;
	//ImGui::Begin("DockingSpace", &opened, flags);

	ImGui::RootDock(ImVec2(docking_pos.x, docking_pos.y), ImVec2(docking_size.x, docking_size.y));
}

void DockingSpace::EndDockSpace()
{
	//ImGui::End();

	ImGui::PopFont();
}
