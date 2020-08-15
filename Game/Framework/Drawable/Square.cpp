#include "Square.h"
#include "../Bindable/BindableBase.h"

Square::Square( Graphics& gfx,float x,float y,float dx,float dy,float angle,float dangle )
    :
    x( x ),
    y( y ),
    dx( dx ),
    dy( dy ),
    angle( angle ),
    dangle( dangle )
{
    struct Vertex
    {
        float x;
        float y;
        float z;
    };
    const std::vector<Vertex> vertecis =
    {
        { 1.0f,1.0f,0.0f },
        { 1.0f,-1.0f,0.0f },
        { -1.0f,-1.0f,0.0f },
        { -1.0f,1.0f,0.0f },
    };
    AddBind( std::make_unique<VertexBuffer>( gfx,vertecis ) );

    const std::vector<unsigned short> indecies =
    {
        0,1,2,
        2,3,0,
    };
    AddIndexBuffer( std::make_unique<IndexBuffer>( gfx,indecies ) );

    auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/VertexShader.cso" );
    auto pvsbc = pvs->GetBytecode();
    AddBind( std::move( pvs ) );

    AddBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/PixelShader.cso" ) );

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
            //{ 0.0f,1.0f,0.0f },
        }
    };
    AddBind( std::make_unique<PixelConstantBuffer<ConstBuffer>>( gfx,cb ) );

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        { "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    AddBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

    AddBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

    AddBind( std::make_unique<TransformCBuf>( gfx,*this ) );
}

void Square::Update( float dt ) noexcept
{
    //if ( x > 10.0f && MoveRight )
    //{
    //    MoveRight = false;
    //    dx = -dx;
    //}
    //if ( x < -10.0f && !MoveRight )
    //{
    //    MoveRight = true;
    //    dx = -dx;
    //}


    tx += dx * dt;
    ty += dy * dt;
    angle += dangle * dt;
}

DirectX::XMMATRIX Square::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixRotationRollPitchYaw( 0.0f,0.0f,angle ) *
        DirectX::XMMatrixTranslation( cosf( tx ) * 2,sinf( ty ) * 2,0.0f ) *
        //DirectX::XMMatrixTranslation( x,y,0.0f ) *
        DirectX::XMMatrixTranslation( x,y,10.0f );
}
