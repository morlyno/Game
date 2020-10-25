#include "SolidSphere.h"
#include "../Utility/MorMath.h"
#include "../Bindable/BindableHeaders.h"
#include "Sphere.h"
#include "GeometryFactory.h"
#include "../BindableCodex.h"

#define NAME(x) std::string( typeid(x).name() )

SolidSphere::SolidSphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
    ) );

    std::vector<unsigned short> indices;

    const auto this_name = NAME( *this );

    if ( const auto key = NAME( Bind::VertexBuffer ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        Geometry::Sphere::Make( vd,indices );
        auto ptr = std::make_shared<Bind::VertexBuffer>( gfx,vd );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::IndexBuffer ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::IndexBuffer>( gfx,indices );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::VertexShader ) + "VertexShader"; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/VertexShader.cso" );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::PixelShader ) + "SolidColor"; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/SolidColor.cso" );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = "PCB" + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        struct ConstBuffer
        {
            DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
        } ColorConst;
        auto ptr = std::make_shared<Bind::PixelConstantBuffer<ConstBuffer>>( gfx,ColorConst );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::InputLayout ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::InputLayout>(
            gfx,vd.GetDesc(),
            dynamic_cast<Bind::VertexShader*>(Bind::Codex::Resolve( NAME( Bind::VertexShader ) + "VertexShader" ).get())->GetBytecode()
            );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::Topology ) + std::to_string( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

void SolidSphere::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return
        dx::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        dx::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 SolidSphere::GetColorXM() const noexcept
{
    return { 0.0f,0.0f,0.0f,0.0f };
}

std::string SolidSphere::GetType() const noexcept
{
    return "[SolidSphere]";
}

bool SolidSphere::SpawnControlWindow() noexcept
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
    }
    ImGui::End();
    return open;
}
