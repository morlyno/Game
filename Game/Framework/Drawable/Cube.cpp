#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include <DirectXMath.h>

Cube::Cube( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index,bool test )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,index ),
    test( test ),
    Rotate_Pitch( test ),
    Rotate_Yaw( test ),
    Rotate_Roll( test )
{
    if ( !IsInitialized() )
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
            DirectX::XMFLOAT3 n;
        };
        const float side = 1.0f;
        std::vector<Vertex> vertices( 24 );
        vertices[0].pos = { -side,-side,-side };// 0 near side
        vertices[1].pos = { side,-side,-side };// 1
        vertices[2].pos = { -side,side,-side };// 2
        vertices[3].pos = { side,side,-side };// 3
        vertices[4].pos = { -side,-side,side };// 4 far side
        vertices[5].pos = { side,-side,side };// 5
        vertices[6].pos = { -side,side,side };// 6
        vertices[7].pos = { side,side,side };// 7
        vertices[8].pos = { -side,-side,-side };// 8 left side
        vertices[9].pos = { -side,side,-side };// 9
        vertices[10].pos = { -side,-side,side };// 10
        vertices[11].pos = { -side,side,side };// 11
        vertices[12].pos = { side,-side,-side };// 12 right side
        vertices[13].pos = { side,side,-side };// 13
        vertices[14].pos = { side,-side,side };// 14
        vertices[15].pos = { side,side,side };// 15
        vertices[16].pos = { -side,-side,-side };// 16 bottom side
        vertices[17].pos = { side,-side,-side };// 17
        vertices[18].pos = { -side,-side,side };// 18
        vertices[19].pos = { side,-side,side };// 19
        vertices[20].pos = { -side,side,-side };// 20 top side
        vertices[21].pos = { side,side,-side };// 21
        vertices[22].pos = { -side,side,side };// 22
        vertices[23].pos = { side,side,side };// 23
        
        std::vector<unsigned short> indices =
        {
             0, 2, 1,    2,3,1,
             4, 5, 7,    4,7,6,
             8,10, 9,  10,11,9,
            12,13,15, 12,15,14,
            16,17,18, 18,17,19,
            20,23,21, 20,22,23
        };
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
        //std::vector<Vertex> vertices =
        //{
        //    {  1.0f, 1.0f, 1.0f }, // 0
        //    {  1.0f,-1.0f, 1.0f }, // 1
        //    { -1.0f,-1.0f, 1.0f }, // 2
        //    { -1.0f, 1.0f, 1.0f }, // 3
        //    {  1.0f, 1.0f,-1.0f }, // 4
        //    {  1.0f,-1.0f,-1.0f }, // 5
        //    { -1.0f,-1.0f,-1.0f }, // 6
        //    { -1.0f, 1.0f,-1.0f }, // 7
        //};
        //std::vector<unsigned short> indices =
        //{
        //    2,1,0, 0,3,2, // Back
        //    4,5,6, 6,7,4, // Front
        //    0,4,7, 7,3,0, // Top
        //    6,5,1, 1,2,6, // Bottem
        //    3,7,6, 6,2,3, // Left
        //    4,0,1, 1,5,4, // Right
        //};
        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightPS.cso" ) );

        //struct Color
        //{
        //    struct
        //    {
        //        float r;
        //        float g;
        //        float b;
        //        float a;
        //    } color[6];
        //};

        //Color c =
        //{
        //    {
        //        { 1.0f,0.0f,0.0f,1.0f },
        //        { 0.0f,1.0f,0.0f,1.0f },
        //        { 0.0f,0.0f,1.0f,1.0f },
        //        { 1.0f,1.0f,0.0f,1.0f },
        //        { 1.0f,0.0f,1.0f,1.0f },
        //        { 0.0f,1.0f,1.0f,1.0f },
        //    }
        //};

        //struct LightPos
        //{
        //    float x,y,z;
        //};
        //LightPos lpos =
        //{
        //    0.0f,0.0f,0.0f
        //};

        //AddStaticBind( std::make_unique<PixelConstantBuffer<LightPos>>( gfx,lpos ) );

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

void Cube::Update( float dt ) noexcept
{
    roll = wrap_angle( Rotate_Roll ? roll + dt : roll );
    pitch = wrap_angle( Rotate_Pitch ? pitch + dt : pitch );
    yaw = wrap_angle( Rotate_Yaw ? yaw + dt : yaw );
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
    return { 0.0f,0.0f,0.0f,0.0f };
}

std::string Cube::GetType() const noexcept
{
    return "[Cube]";
}

bool Cube::SpawnControlWindow() noexcept
{
    bool open = true;
    if ( ImGui::Begin( std::to_string( index ).c_str(),&open ) )
    {
        ImGui::Text( "Position" );
        ImGui::SliderFloat( "X",&x,-10.0f,10.0f );
        ImGui::SliderFloat( "Y",&y,-10.0f,10.0f );
        ImGui::SliderFloat( "Z",&z,-10.0f,10.0f );
        if ( ImGui::Button( "ResetPosition" ) )
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
        ImGui::Text( "Rotation" );
        ImGui::SliderAngle( "Roll",&roll,-180.0f,180.0f );
        ImGui::SliderAngle( "Pitch",&pitch,-180.0f,180.0f );
        ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );
        if ( ImGui::Button( "ResetRotaion" ) )
        {
            roll = 0.0f;
            pitch = 0.0f;
            yaw = 0.0f;
        }
        ImGui::Text( "Scaleing" );
        ImGui::SliderFloat( "Width",&scale_width,0.0f,10.0f );
        ImGui::SliderFloat( "Height",&scale_height,0.0f,10.0f );
        ImGui::SliderFloat( "Depth",&scale_depth,0.0f,10.0f );
        if ( ImGui::Button( "ResetScaling" ) )
        {
            scale_width = 1.0f;
            scale_height = 1.0f;
            scale_depth = 1.0f;
        }
        ImGui::Text( "Coloring" );
        ImGui::ColorEdit4( "Color",color );
        ImGui::Checkbox( "Rotate Roll",&Rotate_Roll );
        ImGui::Checkbox( "Rotate Pitch",&Rotate_Pitch );
        ImGui::Checkbox( "Rotate Yaw",&Rotate_Yaw );
    }
    ImGui::End();
    return open;
}
