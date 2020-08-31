#include "Square.h"
#include "../Bindable/BindableBase.h"
#include "../Utility/MorMath.h"
#include "../ImGui/imgui.h"

Square::Square( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float scale_width,float scale_height,float scale_depth )
    :
    x( x ),
    y( y ),
    z( z ),
    roll( roll ),
    pitch( pitch ),
    yaw( yaw ),
    scale_width( scale_width ),
    scale_height( scale_height ),
    scale_depth( scale_depth )
{
    if ( !IsInitialized() )
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
        };


        //struct Vertex
        //{
        //    float x;
        //    float y;
        //    float z;
        //};
        //const std::vector<Vertex> vertices =
        //{
        //    { 1.0f,1.0f,0.0f },
        //    { 1.0f,-1.0f,0.0f },
        //    { -1.0f,-1.0f,0.0f },
        //    { -1.0f,1.0f,0.0f },

        //    { -1.0f,1.0f,1.0f },
        //    { 1.0f,1.0f,1.0f },
        //};
        //const std::vector<unsigned short> indices =
        //{
        //    0,1,2,
        //    2,3,0,
        //    3,4,0,
        //    4,5,0,
        //};

        const auto[vertices,indices] = test::Make<Vertex>( 0,0 );



        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

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
            } color[2];
        };
        const ConstBuffer cb =
        {
            {
                { 0.0f,0.0f,1.0f },
                //{ 0.0f,0.0f,1.0f },
                { 0.0f,1.0f,0.0f },
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

void Square::SpawnControlWindow() noexcept
{
    ImGui::Begin( "Square" );
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
    ImGui::End();
}
