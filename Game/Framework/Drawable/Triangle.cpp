#include "Triangle.h"
#include "../Bindable/BindableBase.h"

Triangle::Triangle( Graphics& gfx,float x,float y,float dx,float dy,float angle,float dangle )
    :
    x( x ),
    y( y ),
    dx( dx ),
    dy( dy ),
    angle( angle ),
    dangle( dangle )
{
    if ( !IsInitialized() )
    {
        struct Vertex
        {
            float x;
            float y;
            float z;
        };
        const std::vector<Vertex> vertecis =
        {
            { 0.0f,1.0f,0.0f },
            { 1.0f,-1.0f,0.0f },
            { -1.0f,-1.0f,0.0f },
        };
        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertecis ) );

        const std::vector<unsigned short> indecies =
        {
            0,1,2,
        };
        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indecies ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/VertexShader.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/PixelShader.cso" ) );

        struct ConstBuffer
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } color[1];
        };
        const ConstBuffer cb =
        {
            {
                { 1.0f,0.0f,0.0f },
            }
        };
        AddStaticBind( std::make_unique<PixelConstantBuffer<ConstBuffer>>( gfx,cb ) );

        std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        };
        AddStaticBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

        AddStaticBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

    }
    else
    {
        SetIndexBufferFromStatic();
    }
    AddBind( std::make_unique<TransformCBuf>( gfx,*this ) );
}

void Triangle::Update( float dt ) noexcept
{
    tx += dx * dt;
    ty += dy * dt;
    angle += dangle * dt;
}

DirectX::XMMATRIX Triangle::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixRotationRollPitchYaw( 0.0f,0.0f,angle ) *
        DirectX::XMMatrixTranslation( cosf( tx ) * 2,sinf( ty ) * 2,0.0f ) *
        DirectX::XMMatrixTranslation( x,y,10.0f );
}
