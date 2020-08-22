#include "ImGuiManager.h"
#include "../ImGui/imgui.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	pImGuiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext( pImGuiContext );
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
