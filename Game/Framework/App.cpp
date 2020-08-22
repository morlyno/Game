#include "App.h"
#include <sstream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
	drawable.push_back( std::make_unique<Sheet>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,0.0f ) );

	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,0.0f ) );
	//drawable.push_back( std::make_unique<Triangle>( wnd.Gfx(),1.0f,0.0f,0.0f,0.0f,0.0f,1.0f ) );

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
		wnd.Gfx().ClearBuffer( 0.0f,0.0f,0.0f );
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
	}

	cam.ShowControlWindow();
}
