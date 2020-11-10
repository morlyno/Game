#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include <DirectXMath.h>
#include "GeometryFactory.h"
#include "../VertexLayout.h"
#include "../BindableCodex.h"

Cube::Cube( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
        .Add( VertexLayout::Normal )
    ) );

    std::vector<unsigned short> indices;
    auto func = [&indices,&vd]()
    {
        Geometry::MakeNormals( vd,indices );
        Geometry::Cube::MakeIndipendent( vd );
        indices = Geometry::Cube::MakeIndicesIndipendent();
        Geometry::MakeNormals( vd,indices );
    };

    AddBind( Bind::VertexBuffer::Resolve( gfx,vd,func,typeid(*this).name() ) );

    AddBind( Bind::IndexBuffer::Resolve( gfx,indices,typeid(*this).name() ) );

    auto spvs = Bind::VertexShader::Resolve( gfx,"LightVS.cso" );
    auto pvsbc = spvs->GetBytecode();
    AddBind( std::move( spvs ) );

    AddBind( Bind::PixelShader::Resolve( gfx,"LightPS.cso" ) );

    AddBind( Bind::InputLayout::Resolve( gfx,vd.GetLayout(),pvsbc ) );

    AddBind( Bind::Topology::Resolve( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

    AddBind( std::make_shared<Bind::ConstBuffDoubleBoy>( gfx,*this ) );
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
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
