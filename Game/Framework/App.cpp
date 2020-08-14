#include "App.h"
#include <sstream>

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,0.0f,5.0f,0.0f,0.0f,1.0f ) );
	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,5.0f,2.5f,0.0f,0.0f,2.0f ) );

	drawable.push_back( std::make_unique<Square>( wnd.Gfx(),2.0f,2.0f,0.0f,0.0f,0.0f,2.0f ) );

	//drawable.push_back( std::make_unique<Triangle>( wnd.Gfx(),0.0f,0.0f,2.0f,2.0f,0.0f,0.0f ) );

	wnd.Gfx().SetProjection(
		DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) *
		DirectX::XMMatrixScaling( 1.0f,1.0f,1.0f )
	);
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
		wnd.Gfx().ClearBuffer( 0.0f,0.0f,0.0f );
		DoFrame();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	const auto e = wnd.kbd.ReadKey();
	if ( e.GetType() == Keyboard::Event::Type::Press )
	{
		if ( e.GetKeyCode() == VK_DOWN )
		{
			OutputDebugString( L"Down" );
			scale > 0.1f ? scale -= 0.1 : scale = 0.1f;
			wnd.Gfx().SetProjection(
				DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) *
				DirectX::XMMatrixScaling( scale,scale,1.0f )
			);
		}
		else if ( e.GetKeyCode() == VK_UP )
		{
			OutputDebugString( L"Up" );
			scale += 0.1f;
			wnd.Gfx().SetProjection(
				DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) *
				DirectX::XMMatrixScaling( scale,scale,1.0f )
			);
		}
	}

	const auto dt = timer.Mark();
	for ( auto& d : drawable )
	{
		d->Update( dt );
		d->Draw( wnd.Gfx() );
	}
}
