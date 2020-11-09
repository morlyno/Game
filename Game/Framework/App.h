#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Drawable.h"
#include "Surface.h"
#include "Managers/ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Drawable/Model.h"
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
	void SpawnSimulationWindow() noexcept;
	void SpawnDrawableControlWindowMangerWindow() noexcept;
	void SpawnDrawableControlWindows() noexcept;
	void SpawnDrawableSpawnWindow() noexcept;
private:
	float c[3] = { 0.0f,0.0f,0.0f };
	ImGuiManager imgui;
	Camera cam;
	Window wnd;
	MorTimer timer;
	PointLight pl;
	std::vector<std::unique_ptr<Drawable>> drawables;
	Model mesh;
	float SimulationSpeed = 1.0f;
	bool paused = false;
	std::set<int> DrawableId;
	std::optional<int> index;
	std::optional<int> TypeIndex;
	int x = 0,y = 0;
};