#include "Camera.h"
#include "ImGui/imgui.h"
#include <string>
#include "Utility/MorMath.h"
#include <math.h>

Camera::Camera() noexcept
{
    Reset();
}

void Camera::ShowControlWindow() noexcept
{
    constexpr float offset = 100.0f;
    const float item_width = (ImGui::GetWindowSize().x - offset) / 3.0f - 20.0f - 8.0f;
    if ( ImGui::Begin( "Camera" ) )
    {
        ImGui::PushItemWidth( item_width );

        ImGui::Text( "Position" );
        ImGui::SameLine( offset );
        ImGui::DragFloat( "##x",&pos.x,1.0f,0.0f,0.0f,"%.2f" );
        ImGui::SameLine( 0.0f,0.0f );
        ImGui::DragFloat( "##y",&pos.y,1.0f,0.0f,0.0f,"%.2f" );
        ImGui::SameLine( 0.0f,0.0f );
        ImGui::DragFloat( "##z",&pos.z,1.0f,0.0f,0.0f,"%.2f" );

        ImGui::Text( "Direction" );
        ImGui::SameLine( offset );
        ImGui::DragFloat( "##pitch",&d_pitch,1.0f,0.0f,360.0f,"%.2f" );
        ImGui::SameLine( 0.0f,0.0f );
        ImGui::DragFloat( "##yaw",&d_yaw,1.0f,0.0f,360.0f,"%.2f" );

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Camera::Inputs( const Mouse::Event& mouse,const Keyboard& kbd ) noexcept
{
    // ----- Mouse ----- //

    d_yaw += mouse.GetDeltaX();
    d_pitch += mouse.GetDeltaY();
    d_pitch = std::clamp( d_pitch,-89.0f,89.0f );

    // ----- Keyboard ----- //

    constexpr float y_speed = 1.0f;
    constexpr float PIOver2 = MorMath::PI / 2.0f;
    namespace dx = DirectX;

    dx::XMVECTOR vpos = dx::XMLoadFloat3( &pos );

    if ( kbd.KeyIsPresst( 'W') )
    {
        vpos = dx::XMVectorAdd( vpos,eyeDir );
    }
    if ( kbd.KeyIsPresst( 'A' ) )
    {
        vpos = dx::XMVectorAdd( vpos,dx::XMVector3Rotate( eyeDir,dx::XMQuaternionRotationAxis( { 0.0f,1.0f,0.0f,0.0f },-PIOver2 ) ) );
    }
    if ( kbd.KeyIsPresst( 'S' ) )
    {
        vpos = dx::XMVectorAdd( vpos,dx::XMVectorNegate( eyeDir ) );
    }
    if ( kbd.KeyIsPresst( 'D' ) )
    {
        vpos = dx::XMVectorAdd( vpos,dx::XMVector3Rotate( eyeDir,dx::XMQuaternionRotationAxis( { 0.0f,1.0f,0.0f,0.0f },PIOver2 ) ) );
    }
    if ( kbd.KeyIsPresst( VK_SPACE ) )
    {
        vpos = dx::XMVectorSetY( vpos,dx::XMVectorGetY( vpos ) + y_speed );
    }
    if ( kbd.KeyIsPresst( VK_SHIFT ) )
    {
        vpos = dx::XMVectorSetY( vpos,dx::XMVectorGetY( vpos ) - y_speed );
    }
    dx::XMStoreFloat3( &pos,vpos );
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    namespace dx = DirectX;
    constexpr dx::XMVECTOR eyedir_s = { 0.0f,0.0f,1.0f,0.0f };
    constexpr dx::XMVECTOR up = { 0.0f,1.0f,0.0f,0.0f };
    const float r_pitch = MorMath::ToRadians( d_pitch );
    const float r_yaw = MorMath::ToRadians( d_yaw );

    const dx::XMVECTOR cPos = dx::XMLoadFloat3( &pos );
    const dx::XMMATRIX rotation = dx::XMMatrixRotationRollPitchYaw( r_pitch,r_yaw,0.0f );
    const dx::XMVECTOR eyedir = dx::XMVector4Transform( eyedir_s,rotation );
    
    const_cast<Camera*>(this)->eyeDir = eyedir;
    return dx::XMMatrixLookToLH( cPos,eyedir,up );
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
    return pos;
}

void Camera::Reset() noexcept
{
    pos = { 0.0f,0.0f,0.0f };
    eyeDir = { 0.0f,0.0f,1.0f };
    d_pitch = 0.0f;
    d_yaw = 0.0f;
}

void Camera::WrapAngles() noexcept
{
    d_pitch = MorMath::wrap_angle( d_pitch );
    d_yaw = MorMath::wrap_angle( d_yaw );
}