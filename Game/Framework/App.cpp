#include "App.h"
#include <sstream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
	drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,0.0f ) );
	drawable.push_back( std::make_unique<Triangle>( wnd.Gfx(),1.0f,0.0f,0.0f,0.0f,0.0f,1.0f ) );

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) );
	wnd.Gfx().SetCamera( DirectX::XMMatrixTranslation( 0.0f,0.0f,20.0f ) );
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

	const auto dt = timer.Mark();
	for ( auto& d : drawable )
	{
		d->Update( dt );
		d->Draw( wnd.Gfx() );
	}

	ImGui::Begin( "Test Window" );
	ImGui::Text( "Some Text stuf" );
	ImGui::SliderFloat( "X: ",&camx,-40.0f,40.0f );
	ImGui::SliderFloat( "Y: ",&camy,-40.0f,40.0f );
	ImGui::SliderFloat( "Z: ",&camz,0.0f,40.0f );
	if ( ImGui::Button( "Close" ) )
	{
		wnd.Kill();
	}
	ImGui::End();

	auto z = dynamic_cast<Triangle*>( drawable[1].get() );
	ImGui::Begin( "Triangle" );
	ImGui::SliderFloat( "Z: ",z->GetZ(),-20.0f,40.0f );
	if ( ImGui::Button( "Reset" ) )
	{
		*z->GetZ() = 0.0f;
	}
	ImGui::End();

	wnd.Gfx().SetCamera( DirectX::XMMatrixTranslation( camx,camy,camz ) );

}
