#include "App.h"
#include <sstream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

App::App()
    :
    wnd( 800,600,L"SexyWindow" )
{
	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,0.0f,5.0f,0.0f,0.0f,1.0f ) );
	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),0.0f,5.0f,0.0f,0.0f,0.0f,0.0f ) );

	//drawable.push_back( std::make_unique<Square>( wnd.Gfx(),2.0f,2.0f,0.0f,0.0f,0.0f,2.0f ) );

	//drawable.push_back( std::make_unique<Triangle>( wnd.Gfx(),0.0f,-5.0f,2.0f,2.0f,0.0f,0.0f ) );

	drawable.push_back( std::make_unique<Sheet>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,0.0f ) );
	drawable.push_back( std::make_unique<Sheet>( wnd.Gfx(),2.1f,0.0f,0.0f,0.0f,0.0f,0.0f ) );

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
			scale > 0.1f ? scale -= 0.1 : scale = 0.1f;
			wnd.Gfx().SetProjection(
				DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) *
				DirectX::XMMatrixScaling( scale,scale,1.0f )
			);
		}
		else if ( e.GetKeyCode() == VK_UP )
		{
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

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool show_demo_window = true;
	if ( show_demo_window )
	{
		ImGui::ShowDemoWindow( &show_demo_window );
	}

	ImGui::Begin( "Test Window" );
	ImGui::Text( "Some Text stuf" );
	ImGui::Checkbox( "Demo Window",&show_demo_window );
	ImGui::SliderFloat( "float slider: ",&scale,0.0f,10.0f );
	if ( ImGui::Button( "Close" ) )
	{
		wnd.Kill();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

	wnd.Gfx().SetProjection(
		DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) *
		DirectX::XMMatrixScaling( scale,scale,1.0f )
	);
}
