#include "Sheet.h"
#include "../VertexLayout.h"
#include "../Bindable/BindableHeaders.h"
#include "GeometryFactory.h"

Sheet::Sheet( Graphics& gfx,float x,float y,float z,float width,float height,float depth,int index )
    :
    DrawableMemberData( x,y,z,0.0f,0.0f,0.0f,index,width,height,depth )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
        .Add( VertexLayout::Normal )
        .Add( VertexLayout::Tangent )
        .Add( VertexLayout::Bitangent )
        .Add( VertexLayout::Texture2D )
    ) );
    std::vector<unsigned short> indices;
    auto func = [&vd,&indices]()
    {
        constexpr float side = 0.5f;
        vd.Resize( 4u );
        vd[0].Get<VertexLayout::Position3D>() = { -side,side,0.0f };
        vd[1].Get<VertexLayout::Position3D>() = { side,side,0.0f };
        vd[2].Get<VertexLayout::Position3D>() = { side,-side,0.0f };
        vd[3].Get<VertexLayout::Position3D>() = { -side,-side,0.0f };

        vd[0].Get<VertexLayout::Texture2D>() = { 0.0f,0.0f };
        vd[1].Get<VertexLayout::Texture2D>() = { 1.0f,0.0f };
        vd[2].Get<VertexLayout::Texture2D>() = { 1.0f,1.0f };
        vd[3].Get<VertexLayout::Texture2D>() = { 0.0f,1.0f };

        indices.resize( 6u );
        indices[0] = 0u;
        indices[1] = 1u;
        indices[2] = 2u;

        indices[3] = 2u;
        indices[4] = 3u;
        indices[5] = 0u;

        Geometry::MakeNormals( vd,indices );

        Geometry::MakeTanBitan( vd,indices );

    };
    AddBind( Bind::VertexBuffer::Resolve( gfx,vd,func,"Sheet" ) );
    AddBind( Bind::IndexBuffer::Resolve( gfx,indices,"Sheet" ) );
    auto pVs = Bind::VertexShader::Resolve( gfx,"TextureNL_VS.cso" );
    auto pVsBc = pVs->GetBytecode();
    AddBind( std::move( pVs ) );
    AddBind( Bind::PixelShader::Resolve( gfx,"TextureNL_PS.cso" ) );
    AddBind( Bind::Texture::Resolve( gfx,Surface::FromFile( L"brickwall.jpg" ),0u ) );
    AddBind( Bind::Texture::Resolve( gfx,Surface::FromFile( L"brickwall_normal.jpg" ),1u ) );
    AddBind( Bind::Sampler::Resolve( gfx ) );
    AddBind( Bind::InputLayout::Resolve( gfx,vd.GetLayout(),pVsBc ) );
    AddBind( Bind::Topology::Resolve( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
    AddBind( std::make_shared<Bind::ConstBuffDoubleBoy>( gfx,*this ) );
}

void Sheet::Update( float dt ) noexcept
{
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return
        dx::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        dx::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Sheet::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],1.0f };
}

std::string Sheet::GetType() const noexcept
{
    return "[Sheet]";
}
