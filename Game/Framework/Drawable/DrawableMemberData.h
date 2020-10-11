#pragma once

#include "DrawableBase.h"
#include "../ImGui/imgui.h"
#include <sstream>

template<class T>
class DrawableMemberData : public DrawableBase<T>
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
	virtual bool SpawnControlWindow() noexcept override
    {
        bool open = true;
        if ( ImGui::Begin( std::to_string( index ).c_str(),&open ) )
        {
            ImGui::Text( "Position" );
            ImGui::SliderFloat( "X",&x,-10.0f,10.0f );
            ImGui::SliderFloat( "Y",&y,-10.0f,10.0f );
            ImGui::SliderFloat( "Z",&z,-10.0f,10.0f );
            if ( ImGui::Button( "ResetPosition" ) )
            {
                x = 0.0f;
                y = 0.0f;
                z = 0.0f;
            }
            ImGui::Text( "Rotation" );
            ImGui::SliderAngle( "Roll",&roll,-180.0f,180.0f );
            ImGui::SliderAngle( "Pitch",&pitch,-180.0f,180.0f );
            ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );
            if ( ImGui::Button( "ResetRotaion" ) )
            {
                roll = 0.0f;
                pitch = 0.0f;
                yaw = 0.0f;
            }
            ImGui::Text( "Scaleing" );
            ImGui::SliderFloat( "Width",&scale_width,0.0f,10.0f );
            ImGui::SliderFloat( "Height",&scale_height,0.0f,10.0f );
            ImGui::SliderFloat( "Depth",&scale_depth,0.0f,10.0f );
            if ( ImGui::Button( "ResetScaling" ) )
            {
                scale_width = 1.0f;
                scale_height = 1.0f;
                scale_depth = 1.0f;
            }
            ImGui::Text( "Coloring" );
            ImGui::ColorPicker3( "Color",color );
            ImGui::Text( "Specular" );
            ImGui::SliderFloat( "Intesity",&specularIntesity,0.0f,1.0f );
            ImGui::SliderFloat( "Power",&specularPower,0.0f,1.0f );
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
    float specularIntesity = 1.0f;
    float specularPower = 0.01f;

    int index;
};