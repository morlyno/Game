#pragma once

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();
	void StartFrame() noexcept;
	void Render() noexcept;
private:
	struct ImGuiContext* pImGuiContext = nullptr;
};