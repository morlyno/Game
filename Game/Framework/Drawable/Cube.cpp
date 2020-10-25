#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include <DirectXMath.h>
#include "GeometryFactory.h"
#include "../VertexLayout.h"
#include <string>
#include "../BindableCodex.h"

#define NAME(x) std::string( typeid(x).name() )

Cube::Cube( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index,bool test )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index ),
    test( test ),
    Rotate_Pitch( test ),
    Rotate_Yaw( test ),
    Rotate_Roll( test )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
        .Add( VertexLayout::Normal )
    ) );

    std::vector<unsigned short> indices;

    using namespace std::string_literals;

    const auto this_name = std::string{ typeid(*this).name() };

    if ( const auto key = NAME( Bind::IndexBuffer ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        indices = Geometry::Cube::MakeIndicesIndipendent();
        auto ptr = std::make_shared<Bind::IndexBuffer>( gfx,indices );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::VertexBuffer ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        Geometry::Cube::MakeIndipendent( vd );
        Geometry::MakeNormals( vd,indices );
        auto ptr = std::make_shared<Bind::VertexBuffer>( gfx,vd );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::VertexShader ) + "LightVS"; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightVS.cso" );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::PixelShader ) + "LightPS"; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightPS.cso" );
        ptr->SetKEY( key );
        Bind::Codex::Store( ptr );
        AddBind( ptr );
    }
    if ( const auto key = NAME( Bind::InputLayout ) + this_name; !AddBind( Bind::Codex::Resolve( key ) ) )
    {
        auto ptr = std::make_shared<Bind::InputLayout>(
            gfx,vd.GetDesc(),
            dynamic_cast<Bind::VertexShader*>(Bind::Codex::Resolve( NAME( Bind::VertexShader ) + "LightVS" ).get())->GetBytecode()
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
    AddBind( std::make_shared<Bind::ConstBuffDoubleBoy>( gfx,*this ) );
}

void Cube::Update( float dt ) noexcept
{
    roll = wrap_angle( Rotate_Roll ? roll + dt : roll );
    pitch = wrap_angle( Rotate_Pitch ? pitch + dt : pitch );
    yaw = wrap_angle( Rotate_Yaw ? yaw + dt : yaw );
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    if ( test )
    {
        return
            dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
            dx::XMMatrixTranslation( x,0.0f,0.0f ) *
            dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll );
    }
    return
        dx::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        dx::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Cube::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],color[3] };
}

std::string Cube::GetType() const noexcept
{
    return "[Cube]";
}

bool Cube::SpawnControlWindow() noexcept
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
        ImGui::Checkbox( "Rotate Roll",&Rotate_Roll );
        ImGui::Checkbox( "Rotate Pitch",&Rotate_Pitch );
        ImGui::Checkbox( "Rotate Yaw",&Rotate_Yaw );
    }
    ImGui::End();
    return open;
}
