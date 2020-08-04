#include "App.h"
#include <sstream>

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
}

App::~App()
{
}

int App::Go()
{
	while ( true )
	{
		if ( const auto ecode = Window::ProcessingMessage() )
		{
			return *ecode;
		}
		wnd.Gfx().ClearBuffer( 0.0f,0.0f,0.0f );
		DoFrame();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().Drawsdjsgldfg();
}
