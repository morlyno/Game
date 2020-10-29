#include "Sphere.h"
#include "../Bindable/BindableHeaders.h"
#include "GeometryFactory.h"
#include "../VertexLayout.h"
#include "../BindableCodex.h"

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

    AddBind( std::make_shared<Bind::VertexBuffer>( gfx,vd,typeid(*this).name() ) );

    AddBind( std::make_shared<Bind::IndexBuffer>( gfx,indices,typeid(*this).name() ) );

    auto pvs = std::make_shared<Bind::VertexShader>( gfx,"VertexShader.cso" );
    auto pvsbc = pvs->GetBytecode();
    AddBind( std::move( pvs ) );

    AddBind( std::make_shared<Bind::PixelShader>( gfx,"6ColorPS.cso" ) );

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

    AddBind( std::make_shared<Bind::InputLayout>( gfx,vd.GetLayout(),pvsbc ) );

    AddBind( std::make_shared<Bind::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

    AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

void Sphere::Update( float dt ) noexcept
{
    roll = MorMath::wrap_angle( roll );
    pitch = MorMath::wrap_angle( pitch );
    yaw = MorMath::wrap_angle( yaw );
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
    return { color[0],color[1],color[2],1.0f };
}

std::string Sphere::GetType() const noexcept
{
    return "[Sphere]";
}
