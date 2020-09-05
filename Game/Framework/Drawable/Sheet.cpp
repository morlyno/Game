#include "Sheet.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"

Sheet::Sheet( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float scale_width,float scale_height,float scale_depth,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,scale_width,scale_height,scale_depth,index )
{
    if ( !IsInitialized() )
    {
        struct Vectex
        {
            struct
            {
                float x;
                float y;
                float z;
            } pos;
            struct
            {
                float u;
                float v;
            } tex;
        };
        std::vector<Vectex> vertices =
        {
            { -1.0f, 1.0f },
            {  1.0f, 1.0f },
            {  1.0f,-1.0f },
            { -1.0f,-1.0f },
        };
        vertices[0].tex = { 0.0f,0.0f };
        vertices[1].tex = { 1.0f,0.0f };
        vertices[2].tex = { 1.0f,1.0f };
        vertices[3].tex = { 0.0f,1.0f };
        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );
        const std::vector<unsigned short> indices =
        {
            0,1,2,
            2,3,0,
        };
        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

        AddStaticBind( std::make_unique<Texture>( gfx,Surface::FromFile( L"Test.png" ) ) );

        AddStaticBind( std::make_unique<Sampler>( gfx ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/TextureVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/TexturePS.cso" ) );

        std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

void Sheet::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
    return
        DirectX::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        DirectX::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Sheet::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],color[3] };
}
