#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Triangle.h"

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
	Window wnd;
	MorTimer timer;
	//Square square;
	std::vector<std::unique_ptr<Square>> square;
	float scale = 1.0f;
};