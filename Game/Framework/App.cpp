#include "App.h"
#include "ImGui/imgui.h"
#include "Drawable/DrawableHeader.h"
#include "Utility/MorUtility.h"
#include "Utility/MorMath.h"

App::App()
    :
    wnd( 1200,800,L"SexyWindow" ),
	pl( wnd.Gfx(),0.0f,0.0f,0.0f )
{
	drawables.push_back( std::make_unique<Sheet>( wnd.Gfx(),0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,(int)drawables.size() ) );

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
		DoFrame();
		imgui.Render();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	wnd.Gfx().SetCamera( cam );
	const auto dt = timer.Mark() * SimulationSpeed;
	pl.Bind( wnd.Gfx() );
	for ( auto& d : drawables )
	{
		d->Update( paused ? 0.0f : dt );
		d->Draw( wnd.Gfx() );
	}
	pl.Draw( wnd.Gfx() );

	SpawnDrawableControlWindowMangerWindow();
	SpawnDrawableControlWindows();
	SpawnDrawableSpawnWindow();
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
		if ( ImGui::Button( "Spawn Window" ) )
		{
			if ( index )
			{
				DrawableId.insert( *index );
				index.reset();
			}
		}
		if ( ImGui::Button( "Erase Drawable" ) )
		{
			if ( index )
			{
				erase_by_index( drawables,*index );
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

void App::SpawnDrawableSpawnWindow() noexcept
{
	const char* Types[] = { "Square","Sheet","Cube" };
	if ( ImGui::Begin( "Drawable Spawner" ) )
	{
		if ( ImGui::BeginCombo( "Types",TypeIndex ? Types[*TypeIndex] : "Chose Type..." ) )
		{
			for ( int i = 0; i < IM_ARRAYSIZE( Types ); ++i )
			{
				const bool selected = i == TypeIndex;
				if ( ImGui::Selectable( Types[i],selected ) )
				{
					TypeIndex = i;
				}
			}
			ImGui::EndCombo();
		}
		if ( TypeIndex )
		{
			float xyz[3] = { 0.0f };
			ImGui::InputFloat3( "XYZ",xyz );
			if ( ImGui::Button( "Spawn Drawable" ) )
			{
				switch ( *TypeIndex )
				{
				case 0:
					drawables.push_back( std::make_unique<Plane>( wnd.Gfx(),xyz[0],xyz[1],xyz[2],0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,(int)drawables.size() ) );
					DrawableId.insert( (int)drawables.size() - 1 );
					TypeIndex.reset();
					break;
				case 1:
					drawables.push_back( std::make_unique<Sheet>( wnd.Gfx(),xyz[0],xyz[1],xyz[2],0.0f,0.0f,0.0f,(int)drawables.size() ) );
					DrawableId.insert( (int)drawables.size() - 1 );
					TypeIndex.reset();
					break;
				case 2:
					drawables.push_back( std::make_unique<Cube>( wnd.Gfx(),xyz[0],xyz[1],xyz[2],0.0f,0.0f,0.0f,(int)drawables.size() ) );
					DrawableId.insert( (int)drawables.size() - 1 );
					TypeIndex.reset();
					break;
				}
			}
		}

	}
	ImGui::End();
}
