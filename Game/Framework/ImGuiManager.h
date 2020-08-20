#pragma once

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();
private:
	struct ImGuiContext* pImGuiContext = nullptr;
};