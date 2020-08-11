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
		const auto c = sin( timer.Peek() ) / 2.0f + 0.5f;
		wnd.Gfx().ClearBuffer( c,c,1.0f );
		DoFrame();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().Drawsdjsgldfg( 0.0f );
}
