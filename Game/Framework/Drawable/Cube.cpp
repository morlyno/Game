#include "Cube.h"
#include "../Bindable/BindableHeaders.h"
#include "../Utility/MorMath.h"
#include <DirectXMath.h>
#include "GeometryFactory.h"
#include "../VertexLayout.h"

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
        VertexData vd( std::move(
            VertexLayout{}
            .Add( VertexLayout::Position3D )
            .Add( VertexLayout::Normal )
        ) );


        constexpr float side = 1.0f;

        vd.Resize( 24 );

        vd[0].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 0 near side
        vd[1].Get<VertexLayout::Position3D>() = { side,-side,-side };// 1
        vd[2].Get<VertexLayout::Position3D>() = { -side,side,-side };// 2
        vd[3].Get<VertexLayout::Position3D>() = { side,side,-side };// 3
        vd[4].Get<VertexLayout::Position3D>() = { -side,-side,side };// 4 far side
        vd[5].Get<VertexLayout::Position3D>() = { side,-side,side };// 5
        vd[6].Get<VertexLayout::Position3D>() = { -side,side,side };// 6
        vd[7].Get<VertexLayout::Position3D>() = { side,side,side };// 7
        vd[8].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 8 left side
        vd[9].Get<VertexLayout::Position3D>() = { -side,side,-side };// 9
        vd[10].Get<VertexLayout::Position3D>() = { -side,-side,side };// 10
        vd[11].Get<VertexLayout::Position3D>() = { -side,side,side };// 11
        vd[12].Get<VertexLayout::Position3D>() = { side,-side,-side };// 12 right side
        vd[13].Get<VertexLayout::Position3D>() = { side,side,-side };// 13
        vd[14].Get<VertexLayout::Position3D>() = { side,-side,side };// 14
        vd[15].Get<VertexLayout::Position3D>() = { side,side,side };// 15
        vd[16].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 16 bottom side
        vd[17].Get<VertexLayout::Position3D>() = { side,-side,-side };// 17
        vd[18].Get<VertexLayout::Position3D>() = { -side,-side,side };// 18
        vd[19].Get<VertexLayout::Position3D>() = { side,-side,side };// 19
        vd[20].Get<VertexLayout::Position3D>() = { -side,side,-side };// 20 top side
        vd[21].Get<VertexLayout::Position3D>() = { side,side,-side };// 21
        vd[22].Get<VertexLayout::Position3D>() = { -side,side,side };// 22
        vd[23].Get<VertexLayout::Position3D>() = { side,side,side };// 23

        std::vector<unsigned short> indices =
        {
            0,2, 1,    2,3,1,
            4,5, 7,    4,7,6,
            8,10, 9,  10,11,9,
            12,13,15, 12,15,14,
            16,17,18, 18,17,19,
            20,23,21, 20,22,23
        };

        using namespace DirectX;

        for ( size_t i = 0; i < indices.size(); i += 3 )
        {
            auto v0 = vd[indices[i]];
            auto v1 = vd[indices[i + 1]];
            auto v2 = vd[indices[i + 2]];

            const auto p0 = XMLoadFloat3( &v0.Get<VertexLayout::Position3D>() );
            const auto p1 = XMLoadFloat3( &v1.Get<VertexLayout::Position3D>() );
            const auto p2 = XMLoadFloat3( &v2.Get<VertexLayout::Position3D>() );

            const auto n = XMVector3Normalize( XMVector3Cross( (p1 - p0),(p2 - p0) ) );

            XMStoreFloat3( &v0.Get<VertexLayout::Normal>(),n );
            XMStoreFloat3( &v1.Get<VertexLayout::Normal>(),n );
            XMStoreFloat3( &v2.Get<VertexLayout::Normal>(),n );
        }


        AddStaticBind( std::make_unique<VertexBuffer>( gfx,vd ) );

        AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

        auto pvs = std::make_unique<VertexShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightVS.cso" );
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind( std::move( pvs ) );

        AddStaticBind( std::make_unique<PixelShader>( gfx,L"Framework/Shader/ShaderByteCodes/LightPS.cso" ) );

        /*struct Color
        {
            DirectX::XMFLOAT4 c = { 0.7f, 1.0f, 0.4f, 1.0f };
        } color;
        AddStaticBind( std::make_unique<PixelConstantBuffer<Color>>( gfx,color,1u ) );*/

        AddStaticBind( std::make_unique<InputLayout>( gfx,vd.GetLayout().GetDesc(),pvsbc ) );

        AddStaticBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
    }
    else
    {
        SetIndexBufferFromStatic();
    }
    AddBind( std::make_unique<ConstBuffDoubleBoy>( gfx,*this ) );
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
    return { color[0],color[1],color[2],color[3] };
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
        ImGui::ColorPicker3( "Color",color );
        ImGui::Checkbox( "Rotate Roll",&Rotate_Roll );
        ImGui::Checkbox( "Rotate Pitch",&Rotate_Pitch );
        ImGui::Checkbox( "Rotate Yaw",&Rotate_Yaw );
    }
    ImGui::End();
    return open;
}
