#pragma once

#include "Window.h"
#include "Utility/MorTimer.h"
#include "Drawable/Square.h"
#include "Drawable/Triangle.h"
#include "Surface.h"

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
	std::vector<std::unique_ptr<Drawable>> drawable;
	Surface surf = Surface::FromFile( L"Framework/kappa.jpg" );
	float scale = 1.0f;
};