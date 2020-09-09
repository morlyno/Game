#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"

Cube::Cube( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index )
    :
    DrawableMemberData( x,y,z,roll,pitch,yaw,1.0f,1.0f,1.0f,index )
{
    if ( !IsInitialized() )
    {
        struct Vecrtex
        {
            float x,y,z;
        };
        std::vector<Vecrtex> vertices =
        {
            {  1.0f, 1.0f, 1.0f }, // 0
            {  1.0f,-1.0f, 1.0f }, // 1
            { -1.0f,-1.0f, 1.0f }, // 2
            { -1.0f, 1.0f, 1.0f }, // 3
            {  1.0f, 1.0f,-1.0f }, // 4
            {  1.0f,-1.0f,-1.0f }, // 5
            { -1.0f,-1.0f,-1.0f }, // 6
            { -1.0f, 1.0f,-1.0f }, // 7
        };
        std::vector<unsigned short> indices =
        {
            2,1,0, // Back
            0,3,2, // Back
            4,5,6, // Front
            6,7,4, // Front
            0,4,7, // Top
            7,3,0, // Top
            6,5,1, // Bottem
            1,2,6, // Bottem
            3,7,6, // Left
            6,2,3, // Left
            4,0,1, // Right
            1,5,4, // Right
        };
        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/VertexShader.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/6ColorPS.cso" ) );

        struct Color
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } color[6];
        };

        Color c =
        {
            {
                { 1.0f,0.0f,0.0f,1.0f },
                { 0.0f,1.0f,0.0f,1.0f },
                { 0.0f,0.0f,1.0f,1.0f },
                { 1.0f,1.0f,0.0f,1.0f },
                { 1.0f,0.0f,1.0f,1.0f },
                { 0.0f,1.0f,1.0f,1.0f },
            }
        };

        AddStaticBind( std::make_unique<PixelConstantBuffer<Color>>( gfx,c ) );

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

void Cube::Update( float dt ) noexcept
{
    roll = wrap_angle( Rotate_Roll ? roll + dt : roll );
    pitch = wrap_angle( Rotate_Pitch ? pitch + dt : pitch );
    yaw = wrap_angle( Rotate_Yaw ? yaw + dt : yaw );
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
    return
        DirectX::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        DirectX::XMMatrixTranslation( x,y,z );
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
