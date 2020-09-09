#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Drawable.h"
#include "Surface.h"
#include "Managers/ImGuiManager.h"
#include "Camera.h"
#include <set>

class App
{
public:
	App();
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;
	~App();
	int Go();
private:
	void DoFrame();
	void SpawnSimulationWindow();
	void SpawnDrawableControlWindowMangerWindow() noexcept;
	void SpawnDrawableControlWindows() noexcept;
private:
	float c[3] = { 0.5f,0.5f,0.5f };
	ImGuiManager imgui;
	Camera cam;
	Window wnd;
	MorTimer timer;
	float SimulationSpeed = 1.0f;
	bool paused = false;
	std::vector<std::unique_ptr<Drawable>> drawables;
	std::set<int> DrawableId;
	std::optional<int> index;
};