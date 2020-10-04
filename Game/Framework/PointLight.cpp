#include "PointLight.h"

PointLight::PointLight( Graphics& gfx,float x,float y,float z )
	:
	mesh( gfx,x,y,z,0.0f,0.0f,0.0f,-1 ),
	pcb( gfx )
{
	Reset();
}

void PointLight::Draw( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	mesh.SetPos( PLConst.pos );
	mesh.Draw( gfx );
}

void PointLight::Bind( Graphics& gfx ) noexcept
{
	pcb.Update( gfx,PLConst );
	pcb.Bind( gfx );
}

void PointLight::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Point Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&PLConst.pos.x,-40.0f,40.0f );
		ImGui::SliderFloat( "Y",&PLConst.pos.y,-40.0f,40.0f );
		ImGui::SliderFloat( "Z",&PLConst.pos.z,-40.0f,40.0f );
		ImGui::SliderFloat( "Falloff",&PLConst.falloffmutiplier,0.0f,1.0f );
		ImGui::SliderFloat( "Ambinat",&PLConst.ambinat,0.0f,1.0f );
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	PLConst.pos = { 20.0f,0.0f,0.0f };
	PLConst.falloffmutiplier = 0.15f;
	PLConst.ambinat = 0.1f;
}
