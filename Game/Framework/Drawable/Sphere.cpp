#include "Sphere.h"
#include "../Utility/MorMath.h"
#include "../Bindable/BindableHeaders.h"
#include "GeometryFactory.h"
#include "../VertexLayout.h"

Sphere::Sphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
    ) );

    std::vector<unsigned short> indices;
    Geometry::Sphere::Make( vd,indices );

    AddBind( std::make_shared<Bind::VertexBuffer>( gfx,vd ) );

    AddBind( std::make_shared<Bind::IndexBuffer>( gfx,indices ) );

    auto pvs = std::make_shared<Bind::VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/VertexShader.cso" );
    auto pvsbc = pvs->GetBytecode();
    AddBind( std::move( pvs ) );

    AddBind( std::make_shared<Bind::PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/6ColorPS.cso" ) );

    struct ConstBuffer
    {
        DirectX::XMFLOAT4 color[6];
    };
    ConstBuffer cbuff =
    {
        {
            { 1.0f,0.0f,0.0f,1.0f },
            { 0.0f,1.0f,0.0f,1.0f },
            { 0.0f,0.0f,1.0f,1.0f },
            { 1.0f,1.0f,0.0f,1.0f },
            { 1.0f,0.0f,1.0f,1.0f },
            { 0.0f,1.0f,1.0f,1.0f },
        }
    };
    AddBind( std::make_shared<Bind::PixelConstantBuffer<ConstBuffer>>( gfx,cbuff ) );

    AddBind( std::make_shared<Bind::InputLayout>( gfx,vd.GetDesc(),pvsbc ) );

    AddBind( std::make_shared<Bind::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
    AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

void Sphere::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX Sphere::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return
        dx::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        dx::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Sphere::GetColorXM() const noexcept
{
    return { 0.0f,0.0f,0.0f,0.0f };
}

std::string Sphere::GetType() const noexcept
{
    return "[Sphere]";
}

bool Sphere::SpawnControlWindow() noexcept
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
