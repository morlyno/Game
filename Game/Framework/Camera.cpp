#include "Camera.h"

#include "ImGui/imgui.h"


void Camera::ShowControlWindow() noexcept
{
    ImGui::Begin( "Camera" );
    ImGui::SliderFloat( "Pos X",&x,-40.0f,40.0f );
    ImGui::SliderFloat( "Pos Y",&y,-40.0f,40.0f );
    ImGui::SliderFloat( "Pos Z",&z,0.0f,80.0f );
    if ( ImGui::Button( "Reset" ) )
    {
        Reset();
    }
    ImGui::End();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    namespace dx = DirectX;
    auto pos = dx::XMVectorSet( x,y,-z,0.0f );
    auto look = dx::XMVectorSet( 0.0f,0.0f,1.0f,0.0f );
    auto up = dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f );
    return dx::XMMatrixLookToLH( pos,look,up );
}

void Camera::Reset() noexcept
{
    x = 0.0f;
    y = 0.0f;
    z = 20.0f;
}