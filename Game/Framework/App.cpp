#include "App.h"
#include "ImGui/imgui.h"
#include "Drawable/DrawableHeader.h"
#include "Utility/MorUtility.h"

App::App()
    :
    wnd( 1200,800,L"SexyWindow" ),
	pl( wnd.Gfx(),0.0f,0.0f,0.0f ),
	mesh( wnd.Gfx(),"Models/nanosuit.obj" )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,(float)wnd.GetHeight() / (float)wnd.GetWidth(),0.5f,400.0f ) );
}

App::~App()
{
}

int App::Go()
{
	while ( true )
	{
		if ( wnd.kbd.KeyIsPresst( 'Q' ) )
			wnd.Kill();
		if ( const auto ecode = Window::ProcessingMessage() )
		{
			return *ecode;
		}
		wnd.Gfx().ClearBuffer( c[0],c[1],c[2] );
		imgui.StartFrame();
		wnd.Gfx().SetCamera( cam );
		timer.Mark();
		DoFrame();
		imgui.Render();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	const auto e = wnd.mouse.Read();
	cam.Inputs( e,wnd.kbd );

	pl.Bind( wnd.Gfx() );

	mesh.Draw( wnd.Gfx() );

	pl.Draw( wnd.Gfx() );

	const auto c = wnd.kbd.ReadChar();
	if ( c == 'e' )
	{
		wnd.HideMouse();
		wnd.ClipMouse();
		wnd.ReadRawInputs();
	}
	else if ( c == 'r' )
	{
		wnd.ShowMouse();
		wnd.ReleasMouse();
		wnd.ReadNormalInputs();
	}

	SpawnSimulationWindow();
	cam.ShowControlWindow();
	pl.SpawnControlWindow();
}

void App::SpawnSimulationWindow() noexcept
{
	if( ImGui::Begin( "Simulation Control" ) )
	{
		ImGui::ColorEdit3( "Background",c );
		ImGui::SliderFloat( "Speed Factor",&SimulationSpeed,0.0f,10.0f );
		if ( ImGui::Button( "Reset" ) )
			SimulationSpeed = 1.0f;
		ImGui::SameLine();
		if ( ImGui::Button( "x2" ) )
			SimulationSpeed = 2.0f;
		ImGui::SameLine();
		if ( ImGui::Button( "x0.5" ) )
			SimulationSpeed = 0.5f;
		ImGui::Checkbox( "Pause",&paused );
		const float lastdurr = timer.LastDuration();
		ImGui::Text( paused ? "(Paused)" : ( "ms/Frame:" + std::to_string( lastdurr * 1000.0f ) + "  FPS: " + std::to_string( 1.0f / lastdurr )).c_str() );
	}
	ImGui::End();
}
