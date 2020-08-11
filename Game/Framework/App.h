#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"

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
};