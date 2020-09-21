#include "Plane.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"

Plane::Plane( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float sx,float sy,float sz,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index )
{
    scale_width = sx;
    scale_height = sy;
    scale_depth = sz;
    if ( !IsInitialized() )
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT3 n;
        };
        auto[vertices,indices] = Squeare::Make<Vertex>();
        for ( int i = 0; i < indices.size(); i += 3 )
        {
            auto& v0 = vertices[indices[i]];
            auto& v1 = vertices[indices[i + 1]];
            auto& v2 = vertices[indices[i + 2]];

            const auto p0 = DirectX::XMLoadFloat3( &v0.pos );
            const auto p1 = DirectX::XMLoadFloat3( &v1.pos );
            const auto p2 = DirectX::XMLoadFloat3( &v2.pos );

            const auto n = DirectX::XMVector3Normalize( DirectX::XMVector3Cross( DirectX::XMVectorSubtract( p1,p0 ),DirectX::XMVectorSubtract( p2,p0 ) ) );

            DirectX::XMStoreFloat3( &v0.n,n );
            DirectX::XMStoreFloat3( &v1.n,n );
            DirectX::XMStoreFloat3( &v2.n,n );
        }
        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightPS.cso" ) );

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
        DirectX::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
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
