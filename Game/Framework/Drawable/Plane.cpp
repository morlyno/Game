#include "Plane.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"
#include "GeometryFactory.h"

Plane::Plane( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float sx,float sy,float sz,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index )
{
    scale_width = sx;
    scale_height = sy;
    scale_depth = 1.0f;
    if ( !IsInitialized() )
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT3 n;
        };

        auto mesh = Geometry::Plane::Make<Vertex>();
        Geometry::MakeNormals( mesh );

        AddStaticBind( std::make_unique<VertexBuffer>( gfx,mesh.vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,mesh.indices ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightPS.cso" ) );

        struct ColorConstBuff
        {
            DirectX::XMFLOAT4 c = { 1.0f,0.0f,1.0f,1.0f };
        } color;
        AddStaticBind( std::make_unique<PixelConstantBuffer<ColorConstBuff>>( gfx,color,1u ) );

        std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

void Plane::Update( float dt ) noexcept
{
    roll = wrap_angle( roll );
    pitch = wrap_angle( pitch );
    yaw = wrap_angle( yaw );
}

DirectX::XMMATRIX Plane::GetTransformXM() const noexcept
{
    return
        DirectX::XMMatrixScaling( scale_width,scale_height,1.0f ) *
        DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        DirectX::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Plane::GetColorXM() const noexcept
{
    return { color[0],color[1],color[2],color[3] };
}

std::string Plane::GetType() const noexcept
{
    return "[Plane]";
}
