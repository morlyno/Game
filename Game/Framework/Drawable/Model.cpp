#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../VertexLayout.h"
#include "../Bindable/BindableHeaders.h"
#include "../ImGui/imgui.h"
#include "../Utility/MorMath.h"

Model::Model( Graphics& gfx,const std::string& path )
	:
	gfx( gfx )
{
	auto imp = Assimp::Importer();
	const auto pScene = imp.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);
	if ( pScene == nullptr )
	{
		using namespace std::string_literals;
		throw MorException( __LINE__,__FILE__,"[Assimp Exceptiom]\n"s + imp.GetErrorString() );
	}
	AnalyseScene( pScene->mRootNode,pScene,pScene->mRootNode->mTransformation );
}

void Model::Draw( Graphics& gfx ) const noexcept(!IS_DEBUG)
{
	for ( const auto& m : pMeshes )
	{
		m->Draw( gfx );
	}
}

void Model::SpawnControllWindow() noexcept
{
	if ( ImGui::Begin( "Model" ) )
	{
		for ( size_t i = 0; i < pMeshes.size(); ++i )
		{
			if ( ImGui::TreeNode( pMeshes[i]->GetType().c_str() ) )
			{
				pMeshes[i]->SpawnControlWindow();
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void Model::AnalyseScene( const aiNode* pNode,const aiScene* pScene,const aiMatrix4x4 parentmatrix )
{
	assert( pNode != nullptr );
	assert( pScene != nullptr );
	const auto transform = pNode->mTransformation * parentmatrix;
	for ( size_t i = 0; i < pNode->mNumMeshes; ++i )
	{
		const auto mesh = pScene->mMeshes[pNode->mMeshes[i]];
		pMeshes.push_back(
			std::make_unique<Mesh>(
				gfx,
				ParseMesh( mesh ),
				DirectX::XMMatrixTranspose( *reinterpret_cast<DirectX::FXMMATRIX*>(&transform) ),
				mesh->mName.C_Str()
			)
		);
	}
	for ( size_t i = 0; i < pNode->mNumChildren; ++i )
	{
		AnalyseScene( pNode->mChildren[i],pScene,transform );
	}
}

std::vector<std::shared_ptr<Bind::Bindable>> Model::ParseMesh( const aiMesh* pMesh )
{
	VertexData vd( std::move(
		VertexLayout{}
		.Add( VertexLayout::Position3D )
		.Add( VertexLayout::Normal )
	) );
	std::vector<unsigned short> indices;
	for ( size_t i = 0; i < pMesh->mNumVertices; ++i )
	{
		vd.Emplace_Back(
			*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i]),
			*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])
		);
	}
	for ( size_t i = 0; i < pMesh->mNumFaces; ++i )
	{
		auto& face = pMesh->mFaces[i];
		assert( face.mNumIndices == 3 );
		indices.push_back( face.mIndices[0] );
		indices.push_back( face.mIndices[1] );
		indices.push_back( face.mIndices[2] );
	}
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
	using namespace std::string_literals;
	binds.push_back( Bind::VertexBuffer::Resolve( gfx,vd,nullptr,"Model"s + pMesh->mName.C_Str() ) );
	binds.push_back( Bind::IndexBuffer::Resolve( gfx,std::move( indices ),"Model"s + pMesh->mName.C_Str() ) );
	auto pVs = Bind::VertexShader::Resolve( gfx,"LightVS.cso" );
	auto pVsBc = pVs->GetBytecode();
	binds.push_back( std::move( pVs ) );
	binds.push_back( Bind::PixelShader::Resolve( gfx,"LightPS.cso" ) );
	binds.push_back( Bind::InputLayout::Resolve( gfx,vd.GetLayout(),pVsBc ) );
	binds.push_back( Bind::Topology::Resolve( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	struct PSConst
	{
		DirectX::XMFLOAT4 matirial = { 0.2f,0.4f,0.12f,1.0f };
		float specularIntesity = 0.22f;
		float specularPower = 0.003f;
		float padding[2] = { 0.0f };
	} PSC;
	binds.push_back( Bind::PixelConstantBuffer<PSConst>::Resolve( gfx,PSC,1u ) );
	return std::move( binds );
}

Mesh::Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>> binds,DirectX::FXMMATRIX matrix,const std::string& name )
	:
	ParentTransform( matrix ),
	name( name )
{
	for ( auto& b : binds )
	{
		AddBind( b );
	}
	AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return
		dx::XMMatrixRotationRollPitchYaw( roll,pitch,yaw ) *
		ParentTransform *
		dx::XMMatrixRotationRollPitchYaw( Mor::PI,0.0f,0.0f ) *
		dx::XMMatrixTranslation( x,y,z );
}

bool Mesh::SpawnControlWindow() noexcept
{
	bool open = true;
	if ( ImGui::Begin( GetType().c_str(),&open ) )
	{
		constexpr float offset = 100.0f;
		constexpr float DragSpeed_Position = 0.1f;
		constexpr float DragSpeed_Rotation = 1.0f;
		constexpr float DragSpeed_Scaling = 0.1f;
		const float item_width = (ImGui::GetWindowSize().x - offset) / 3.0f - 20.0f - 8.0f;

		// -----Position----- //
		ImGui::Text( "Position" );
		ImGui::PushItemWidth( item_width );

		ImGui::SameLine( offset );
		if ( ImGui::Button( "X",ImVec2( 20,0 ) ) ) { x = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##X",&x,DragSpeed_Position,0.0f,0.0f,"%.2f" );

		ImGui::SameLine();
		if ( ImGui::Button( "Y",ImVec2( 20,0 ) ) ) { y = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##Y",&y,DragSpeed_Position,0.0f,0.0f,"%.2f" );

		ImGui::SameLine();
		if ( ImGui::Button( "Z",ImVec2( 20,0 ) ) ) { z = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##Z",&z,DragSpeed_Position,0.0f,0.0f,"%.2f" );

		// -----Rotation----- //
		ImGui::Text( "Rotation" );
		ImGui::SameLine( offset );

		roll = Mor::ToDegreees( roll );
		pitch = Mor::ToDegreees( pitch );
		yaw = Mor::ToDegreees( yaw );

		if ( ImGui::Button( "R##Roll",ImVec2( 20,0 ) ) ) { roll = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##Roll",&roll,DragSpeed_Rotation,0.0f,0.0f,"%.2f" );

		ImGui::SameLine();
		if ( ImGui::Button( "P##Pitch",ImVec2( 20,0 ) ) ) { pitch = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##Pitch",&pitch,DragSpeed_Rotation,0.0f,0.0f,"%.2f" );

		ImGui::SameLine();
		if ( ImGui::Button( "Y##Yaw",ImVec2( 20,0 ) ) ) { yaw = 0.0f; }
		ImGui::SameLine( 0.0f,0.0f );
		ImGui::DragFloat( "##Yaw",&yaw,DragSpeed_Rotation,0.0f,0.0f,"%.2f" );

		roll = Mor::ToRadians( roll );
		pitch = Mor::ToRadians( pitch );
		yaw = Mor::ToRadians( yaw );

		ImGui::PopItemWidth();
	}
	ImGui::End();
	return open;
}

std::string Mesh::GetType() const noexcept
{
	return "[Model_Mesh]" + name;
}
