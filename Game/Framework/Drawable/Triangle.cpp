#include "Triangle.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"

Triangle::Triangle( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float scale_width,float scale_height,float scale_depth,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,scale_width,scale_height,scale_depth,index )
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

    AddBind( std::make_unique<ColorCBuf>( gfx,*this ) );
}

void Triangle::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX Triangle::GetTransformXM() const noexcept
{
    return
        DirectX::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        DirectX::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Triangle::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],color[3] };
}

std::string Triangle::GetType() const noexcept
{
    return "[Triangle]";
}
