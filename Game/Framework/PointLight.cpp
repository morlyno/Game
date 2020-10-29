#include "PointLight.h"

PointLight::PointLight( Graphics& gfx,float x,float y,float z,int index )
	:
	mesh( gfx,x,y,z,0.0f,0.0f,0.0f,index ),
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
		ImGui::DragFloat( "X",&PLConst.pos.x,1.0f,0.0f,0.0f );
		ImGui::DragFloat( "Y",&PLConst.pos.y,1.0f,0.0f,0.0f );
		ImGui::DragFloat( "Z",&PLConst.pos.z,1.0f,0.0f,0.0f );
		ImGui::DragFloat( "Falloff",&PLConst.falloffmutiplier,1.0f,0.0f,1.0f );
		ImGui::DragFloat( "Ambinat",&PLConst.ambinat,1.0f,0.0f,1.0f );
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	PLConst.pos = { 20.0f,0.0f,0.0f };
	PLConst.falloffmutiplier = 0.177f;
	PLConst.ambinat = 0.067f;
}
