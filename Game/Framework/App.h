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
private:
	float c[3] = { 0.0f,0.0f,0.0f };
	ImGuiManager imgui;
	Camera cam;
	Window wnd;
	MorTimer timer;
	PointLight pl;
	Model mesh;
	float SimulationSpeed = 1.0f;
	bool paused = false;
};