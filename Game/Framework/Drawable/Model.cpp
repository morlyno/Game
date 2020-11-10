#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../VertexLayout.h"
#include "../Bindable/BindableHeaders.h"

Model::Mesh::Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>>&& binds,DirectX::FXMMATRIX matrix )
	:
	ParentTransform( matrix )
{
	for ( auto&& b : binds )
	{
		AddBind( std::move( b ) );
	}
	AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

DirectX::XMMATRIX Model::Mesh::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return
		// TODO(Mor)
		ParentTransform;
}

bool Model::Mesh::SpawnControlWindow() noexcept
{
	return
		// TODO(Mor)
		false;
}

std::string Model::Mesh::GetType() const noexcept
{
	return "[ModelChild]";
}

Model::Model( Graphics& gfx,const std::string& path )
	:
	gfx( gfx )
{
	auto importer = Assimp::Importer();
	const auto scene = importer.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);
	auto pRootNode = scene->mRootNode;
	ParseMesh( pRootNode,scene );
}

void Model::Draw( Graphics& gfx ) const noexcept(!IS_DEBUG)
{
	for ( const auto& m : meshes )
	{
		m->Draw( gfx );
	}
}

void Model::ParseMesh( const aiNode* pChild,const aiScene* scene )
{
	for ( size_t i = 0; i < pChild->mNumMeshes; ++i )
	{
		CreateMesh( scene->mMeshes[pChild->mMeshes[i]],*reinterpret_cast<DirectX::FXMMATRIX*>(&pChild->mTransformation) );
	}
	for ( size_t i = 0; i < pChild->mNumChildren; ++i )
	{
		ParseMesh( pChild->mChildren[i],scene );
	}
}

void Model::CreateMesh( aiMesh* mesh,DirectX::FXMMATRIX matrix )
{
	VertexData vd( std::move(
		VertexLayout{}
		.Add( VertexLayout::Position3D )
		.Add( VertexLayout::Normal )
	) );
	std::vector<unsigned short> indices;
	for ( size_t i = 0; i < mesh->mNumVertices; ++i )
	{
		vd.Emplace_Back(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mNormals[i])
		);
	}
	for ( size_t i = 0; i < mesh->mNumFaces; ++i )
	{
		auto& face = mesh->mFaces[i];
		assert( face.mNumIndices == 3u );
		indices.push_back( face.mIndices[0]);
		indices.push_back( face.mIndices[1]);
		indices.push_back( face.mIndices[2]);
	}
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
	binds.push_back( Bind::VertexBuffer::Resolve( gfx,vd,nullptr,mesh->mName.C_Str() ) );
	binds.push_back( Bind::IndexBuffer::Resolve( gfx,indices,mesh->mName.C_Str() ) );
	auto pVs = Bind::VertexShader::Resolve( gfx,"LightVS.cso" );
	auto pVsBc = pVs->GetBytecode();
	binds.push_back( std::move( pVs ) );
	binds.push_back( Bind::PixelShader::Resolve( gfx,"LightPS.cso" ) );
	binds.push_back( Bind::InputLayout::Resolve( gfx,vd.GetLayout(),pVsBc ) );
	binds.push_back( Bind::Topology::Resolve( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	struct PSConstants
	{
		DirectX::XMFLOAT4 matirial = { 0.2f,0.14f,0.4f,1.0f };
		float specularIntesity = 0.22f;
		float specularPower = 0.003f;
		float padding[2];
	} PSC;
	binds.push_back( Bind::PixelConstantBuffer<PSConstants>::Resolve( gfx,PSC,1u ) );
	meshes.push_back( std::make_unique<Mesh>( gfx,std::move( binds ),matrix ) );
}
