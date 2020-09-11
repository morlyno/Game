#include "Camera.h"

#include "ImGui/imgui.h"

#include <string>

void Camera::ShowControlWindow() noexcept
{
    ImGui::Begin( "Camera" );
    ImGui::Checkbox( "CameraMode",&rotation_camera );
    if ( rotation_camera )
    {
        ImGui::DragFloat3( "look_at",look_xyz );
        ImGui::DragFloat( "Distance",&r,1.0f,1.0f,1000 );
        ImGui::SliderAngle( "Pitch",&pitch,-89.0f,89.0f );
        ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );
    }
    else
    {
        ImGui::SliderFloat( "Pos X",&x,-40.0f,40.0f );
        ImGui::SliderFloat( "Pos Y",&y,-40.0f,40.0f );
        ImGui::SliderFloat( "Pos Z",&z,0.0f,80.0f );
    }
    if ( ImGui::Button( "Reset" ) )
    {
        Reset();
    }
    ImGui::End();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    namespace dx = DirectX;
    if ( rotation_camera )
    {
        auto rotation = dx::XMMatrixRotationRollPitchYaw( pitch,yaw,0.0f );
        auto distance = dx::XMVectorSet( 0.0f,0.0f,-r,1.0f );
        auto offset = dx::XMVector4Transform( distance,rotation );
        auto look_at = dx::XMVectorSet( look_x,look_y,look_z,1.0f );
        auto pos = dx::XMVectorAdd( look_at,offset );
        auto up_dir = dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f );
        return dx::XMMatrixLookAtLH( pos,look_at,up_dir );
    }
    auto eye_pos = dx::XMVectorSet( x,y,-z,0.0f );
    auto look_dir = dx::XMVectorSet( 0.0f,0.0f,1.0f,0.0f );
    auto up_dir = dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f );
    return dx::XMMatrixLookToLH( eye_pos,look_dir,up_dir );
}

void Camera::Reset() noexcept
{
    x = 0.0f;
    y = 0.0f;
    z = 20.0f;
    look_x = 0.0f;
    look_y = 0.0f;
    look_z = 0.0f;
    r = 20.0f;
    pitch = 0.0;
    yaw = 0.0;
}
