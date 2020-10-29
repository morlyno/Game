#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include <DirectXMath.h>
#include "GeometryFactory.h"
#include "../VertexLayout.h"
#include "../BindableCodex.h"

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

    Geometry::Cube::MakeIndipendent( vd );
    
    auto indices = Geometry::Cube::MakeIndicesIndipendent();
    Geometry::MakeNormals( vd,indices );

    AddBind( std::make_shared<Bind::VertexBuffer>( gfx,vd,typeid(*this).name() ) );

    AddBind( std::make_shared<Bind::IndexBuffer>( gfx,indices,typeid(*this).name() ) );

    auto pvs = std::make_shared<Bind::VertexShader>( gfx,"LightVS.cso" );
    auto pvsbc = pvs->GetBytecode();
    AddBind( std::move( pvs ) );

    AddBind( std::make_shared<Bind::PixelShader>( gfx,"LightPS.cso" ) );

    AddBind( std::make_shared<Bind::InputLayout>( gfx,vd.GetLayout(),pvsbc ) );

    AddBind( std::make_shared<Bind::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

    AddBind( std::make_shared<Bind::ConstBuffDoubleBoy>( gfx,*this ) );
}

void Cube::Update( float dt ) noexcept
{
    roll = MorMath::wrap_angle( Rotate_Roll ? roll + dt : roll );
    pitch = MorMath::wrap_angle( Rotate_Pitch ? pitch + dt : pitch );
    yaw = MorMath::wrap_angle( Rotate_Yaw ? yaw + dt : yaw );
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
    return { color[0],color[1],color[2],1.0f };
}

std::string Cube::GetType() const noexcept
{
    return "[Cube]";
}
