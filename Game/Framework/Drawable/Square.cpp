#include "Square.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"

Square::Square( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float scale_width,float scale_height,float scale_depth,int index )
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
        const std::vector<Vertex> vertices =
        {
            { 1.0f,1.0f,0.0f },
            { 1.0f,-1.0f,0.0f },
            { -1.0f,-1.0f,0.0f },
            { -1.0f,1.0f,0.0f },

            { -1.0f,1.0f,1.0f },
            { 1.0f,1.0f,1.0f },
        };
        const std::vector<unsigned short> indices =
        {
            0,1,2,
            2,3,0,
            3,4,0,
            4,5,0,
        };

        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

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

void Square::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX Square::GetTransformXM() const noexcept
{
    return
        DirectX::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        DirectX::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Square::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],color[3] };
}
