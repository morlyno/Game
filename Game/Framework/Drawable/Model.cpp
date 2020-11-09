#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../VertexLayout.h"
#include "../Bindable/BindableHeaders.h"
#include "../ImGui/imgui.h"
#include "../Utility/MorMath.h"

Model::Model( Graphics& gfx,const std::string& path,float x,float y,float z )
    :
    x( x ),
    y( y ),
    z( z )
{
    VertexData vd( std::move(
        VertexLayout{}
        .Add( VertexLayout::Position3D )
        .Add( VertexLayout::Normal )
    ) );
    std::vector<unsigned short> indices;

    Assimp::Importer importer;
    auto scene = importer.ReadFile( path,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );
    auto pMesh = scene->mMeshes[0];

    vd.Reserve( pMesh->mNumVertices );
    for ( size_t i = 0; i < pMesh->mNumVertices; ++i )
    {
        vd.Emplace_Back(
            *reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])
        );
    }
    indices.reserve( indices.capacity() + (size_t)pMesh->mNumFaces * 3ull );
    for ( size_t i = 0; i < pMesh->mNumFaces; ++i )
    {
        auto& face = pMesh->mFaces[i];
        assert( face.mNumIndices == 3 );
        indices.emplace_back( (unsigned short)face.mIndices[0] );
        indices.emplace_back( (unsigned short)face.mIndices[1] );
        indices.emplace_back( (unsigned short)face.mIndices[2] );
    }

    AddBind( Bind::VertexBuffer::Resolve( gfx,vd,nullptr,"Model" + path ) );
    AddBind( Bind::IndexBuffer::Resolve( gfx,indices,"Model" + path ) );
    auto pVs = Bind::VertexShader::Resolve( gfx,"LightVS.cso" );
    auto pVsBc = pVs->GetBytecode();
    AddBind( std::move( pVs ) );
    AddBind( Bind::PixelShader::Resolve( gfx,"LightPS.cso" ) );
    AddBind( Bind::InputLayout::Resolve( gfx,vd.GetLayout(),pVsBc ) );
    AddBind( Bind::Topology::Resolve( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
    struct ConstBuffer
    {
        DirectX::XMFLOAT4 c = { 0.12f,0.4f,0.2f,1.0f };
        float specularIntesity = 0.22f;
        float specularPower = 0.003f;
        float buffer[2];
    } consts;
    AddBind( Bind::PixelConstantBuffer<ConstBuffer>::Resolve( gfx,consts,1u ) );
    AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

void Model::Update( float dt ) noexcept
{
}

DirectX::XMMATRIX Model::GetTransformXM() const noexcept
{
    namespace dx = DirectX;
    return
        dx::XMMatrixScaling( scale_width,scale_height,scale_depth ) *
        dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
        dx::XMMatrixTranslation( x,y,z );
}

DirectX::XMFLOAT4 Model::GetColorXM() const noexcept
{
    return DirectX::XMFLOAT4();
}

float Model::GetSpecularPower() const noexcept
{
    return 0.0f;
}

float Model::GetSpecularIntesity() const noexcept
{
    return 0.0f;
}

bool Model::GetNormalMap() const noexcept
{
    return false;
}

bool Model::SpawnControlWindow() noexcept
{
    return false;
}

std::string Model::GetType() const noexcept
{
    return "[Model]";
}
