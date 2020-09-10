#include "App.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "Drawable/Sheet.h"
#include "Drawable/Square.h"
#include "Drawable/Triangle.h"
#include "Drawable/Cube.h"

App::App()
    :
    wnd( 1200,800,L"SexyWindow" )
{
	//drawables.push_back( std::make_unique<Square>	( wnd.Gfx(),-9.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,(int)drawables.size() ) );
	//drawables.push_back( std::make_unique<Square>	( wnd.Gfx(), 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,(int)drawables.size() ) );
	//drawables.push_back( std::make_unique<Square>	( wnd.Gfx(),-4.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,(int)drawables.size() ) );
	//drawables.push_back( std::make_unique<Triangle>	( wnd.Gfx(), 0.0f,-7.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,(int)drawables.size() ) );
	//drawables.push_back( std::make_unique<Cube>		( wnd.Gfx(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,				  (int)drawables.size() ) );
	//drawables.push_back( std::make_unique<Cube>		( wnd.Gfx(), 5.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f,				  (int)drawables.size() ) );

	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(),-4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f,-4.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f, 0.0f,-4.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );

	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(),10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );
	drawables.push_back( std::make_unique<Cube>( wnd.Gfx(), 0.0f,10.0f, 0.0f, 0.0f, 0.0f, 0.0f, (int)drawables.size() ) );

	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,(float)wnd.GetHeight() / (float)wnd.GetWidth(),0.5f,400.0f ) );
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
	const auto dt = timer.Mark() * SimulationSpeed;
	for ( auto& d : drawables )
	{
		d->Update( paused ? 0.0f : dt );
		d->Draw( wnd.Gfx() );
	}

	SpawnDrawableControlWindowMangerWindow();
	SpawnDrawableControlWindows();
	SpawnSimulationWindow();
	cam.ShowControlWindow();
}

void App::SpawnSimulationWindow()
{
	if( ImGui::Begin( "Simulation Control" ) )
	{
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
		ImGui::Text( paused ? "(Paused)" : ("FPS: " + std::to_string( 1.0f / timer.LastDuration() )).c_str() ); 
	}
	ImGui::End();
}

void App::SpawnDrawableControlWindowMangerWindow() noexcept
{
	if ( ImGui::Begin( "Drawables" ) )
	{
		if ( ImGui::BeginCombo( "Index",index ? std::to_string( index.value() ).c_str() : "Select Drawable..." ) )
		{
			for ( int i = 0; i < drawables.size(); ++i )
			{
				const bool selected = i == index;
				if ( ImGui::Selectable( (std::to_string( i ) + " " + drawables[i]->GetType()).c_str(),selected ) )
				{
					index = i;
				}
			}

			ImGui::EndCombo();
		}
		if ( ImGui::Button( "SpawnWindow" ) )
		{
			if ( index )
			{
				DrawableId.insert( *index );
				index.reset();
			}
		}
	}
	ImGui::End();
}

void App::SpawnDrawableControlWindows() noexcept
{
	for ( auto i = DrawableId.begin(); i != DrawableId.end(); )
	{
		if ( !drawables[*i]->SpawnControlWindow() )
		{
			i = DrawableId.erase( i );
		}
		else
		{
			++i;
		}
	}
}
