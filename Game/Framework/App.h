#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Square.h"
#include "Drawable/Triangle.h"
#include "Drawable/Sheet.h"
#include "Surface.h"
#include "ImGuiManager.h"

class App
{
public:
	App();
	App( const App& ) = delete;
	App operator=( const App& ) = delete;
	~App();
	int Go();
private:
	void DoFrame();
private:
	ImGuiManager imgui;
	Window wnd;
	MorTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawable;
	float scale = 1.0f;
};