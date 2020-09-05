#include "App.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "Drawable/Sheet.h"
#include "Drawable/Square.h"
#include "Drawable/Triangle.h"

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
	drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,8.0f,1.0f,1.0f,1.0f,(int)drawable.size() ) );
	drawable.push_back( std::make_unique<Square>( wnd.Gfx(),2.0f,0.0f,0.0f,0.0f,5.0f,0.0f,1.0f,1.0f,1.0f,(int)drawable.size() ) );
	drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,1.6f,0.0f,17.0f,0.0f,0.0f,1.0f,1.0f,1.0f,(int)drawable.size() ) );
	drawable.push_back( std::make_unique<Triangle>( wnd.Gfx(),0.0f,1.6f,0.0f,17.0f,0.0f,0.0f,1.0f,1.0f,1.0f,(int)drawable.size() ) );


	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,400.0f ) );
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
		wnd.Gfx().ClearBuffer( c[0],c[1],c[2] );
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		DoFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().SetCamera( cam.GetMatrix() );

	const auto dt = timer.Mark();
	for ( auto& d : drawable )
	{
		d->Update( dt );
		d->Draw( wnd.Gfx() );
		d->SpawnControlWindow();
	}

	cam.ShowControlWindow();
}