#pragma once

#include "Drawable.h"
#include "../ImGui/imgui.h"
#include "../Utility/MorMath.h"

class DrawableMemberData : public Drawable
{
public:
	DrawableMemberData( float x,float y,float z,float roll,float pitch,float yaw,int index ) noexcept
        :
        x( x ),
        y( y ),
        z( z ),
        roll( roll ),
        pitch( pitch ),
        yaw( yaw ),
        index( index )
    {
    }
    void SetPos( const DirectX::XMFLOAT3& pos ) noexcept
    {
        x = pos.x;
        y = pos.y;
        z = pos.z;
    }
    float GetSpecularPower() const noexcept override
    {
        return specularPower;
    }
    float GetSpecularIntesity() const noexcept override
    {
        return specularIntesity;
    }
    virtual std::string GetType() const noexcept = 0;
	virtual bool SpawnControlWindow() noexcept override
    {
        bool open = true;
        if ( ImGui::Begin( (GetType() + " " + std::to_string( index )).c_str(),&open ) )
        {
            constexpr float offset = 100.0f;
            const float item_width = (ImGui::GetWindowSize().x - offset) / 3.0f - 20.0f - 8.0f;

            // -----Position----- //
            ImGui::Text( "Position" );
            ImGui::PushItemWidth( item_width );

            ImGui::SameLine( offset );
            if ( ImGui::Button( "X",ImVec2( 20,0 ) ) ) { x = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##X",&x,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "Y",ImVec2( 20,0 ) ) ) { y = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f);
            ImGui::DragFloat( "##Y",&y,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "Z",ImVec2( 20,0 ) ) ) { z = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Z",&z,0.1f,0.0f,0.0f,"%.2f" );

            // -----Rotation----- //
            ImGui::Text( "Rotation" );
            ImGui::SameLine( offset );

            roll = MorMath::ToDegreees( roll );
            pitch = MorMath::ToDegreees( pitch );
            yaw = MorMath::ToDegreees( yaw );

            if ( ImGui::Button( "R##Roll",ImVec2( 20,0 ) ) ) { roll = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Roll",&roll,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "P##Pitch",ImVec2( 20,0 ) ) ) { pitch = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Pitch",&pitch,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "Y##Yaw",ImVec2( 20,0 ) ) ) { yaw = 0.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Yaw",&yaw,0.1f,0.0f,0.0f,"%.2f" );
            
            roll = MorMath::ToRadians( roll );
            pitch = MorMath::ToRadians( pitch );
            yaw = MorMath::ToRadians( yaw );

            // -----Scaling----- //
            ImGui::Text( "Scaling" );
            ImGui::SameLine( offset );

            if ( ImGui::Button( "W##Width",ImVec2( 20,0 ) ) ) { scale_width = 1.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Width",&scale_width,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "H##Height",ImVec2( 20,0 ) ) ) { scale_height = 1.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Height",&scale_height,0.1f,0.0f,0.0f,"%.2f" );

            ImGui::SameLine();
            if ( ImGui::Button( "D##Depth",ImVec2( 20,0 ) ) ) { scale_depth = 1.0f; }
            ImGui::SameLine( 0.0f,0.0f );
            ImGui::DragFloat( "##Depth",&scale_depth,0.1f,0.0f,0.0f,"%.2f" );
            
            ImGui::PopItemWidth();

            ImGui::Text( "Color" );
            ImGui::SameLine( offset );
            ImGui::ColorEdit3( "##c",color );

            ImGui::Text( "Intesity" );
            ImGui::SameLine( offset );
            ImGui::DragFloat( "##Intesity",&specularIntesity,0.01f,0.0f,1.0f,"%.3f" );

            ImGui::Text( "Power" );
            ImGui::SameLine( offset );
            ImGui::DragFloat( "##Power",&specularPower,0.01f,0.0f,1.0f,"%.4f" );
        }
        ImGui::End();
        return open;
    }
protected:
	float x;
	float y;
	float z;

	float roll;
	float pitch;
	float yaw;

	float scale_width = 1.0f;
	float scale_height = 1.0f;
	float scale_depth = 1.0f;

	float color[3] = { 1.0f,0.0f,1.0f };
    float specularIntesity = 0.22f;
    float specularPower = 0.003f;

    int index;
};