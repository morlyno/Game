#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Drawable.h"
#include "Surface.h"
#include "Managers/ImGuiManager.h"
#include "Camera.h"

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
private:
	float c[3] = { 0.5f,0.5f,0.5f };
	ImGuiManager imgui;
	Camera cam;
	Window wnd;
	MorTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawable;
};