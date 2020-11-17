#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../VertexLayout.h"
#include "../Bindable/BindableHeaders.h"
#include "../ImGui/imgui.h"
#include "../Utility/MorMath.h"

Mesh::Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>> binds )
{
	for ( auto& b : binds )
	{
		AddBind( b );
	}
	AddBind( std::make_shared<Bind::TransformCBuf>( gfx,*this ) );
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return Transformation;
}

std::string Mesh::GetType() const noexcept
{
	return "[Model_Mesh]";
}

void Mesh::RefrenzTransformation( DirectX::FXMMATRIX matrix )
{
	Transformation = matrix;
}

Node::Node( Node* pParent,DirectX::FXMMATRIX matrix,const std::string& name )
	:
	pParent( pParent ),
	RelativeNodeTransformation( matrix ),
	name( name )
{
}

void Node::AddChildren(Ref<Node> pChild )
{
	Children.push_back( std::move( pChild ) );
}

std::shared_ptr<Node> Node::CreateChild( Node* pParent,DirectX::FXMMATRIX matrix,const std::string& name )
{
	return std::make_shared<Node>( pParent,matrix,name );
}

void Node::AddMesh( Ref<Mesh> pMesh )
{
	Meshes.push_back( pMesh );
}

std::vector<Node::Ref<Mesh>> Node::GetMeshes() const noexcept
{
	return Meshes;
}

std::vector<Node::Ref<Node>> Node::GetChildren() const noexcept
{
	return Children;
}

DirectX::XMMATRIX Node::GetTransform() const noexcept
{
	namespace dx = DirectX;
	return
		dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		RelativeNodeTransformation *
		(pParent ? pParent->GetTransform() : DirectX::XMMatrixIdentity());
}


void Node::ControlWindow() noexcept
{
	// -----Rotation----- //
	ImGui::Text( "Rotation" );

	constexpr float DragSpeed_Rotation = 1.0f;
	const auto texwidth = ImGui::CalcTextSize( "Rotation" ).x;
	const float item_width = (ImGui::GetColumnWidth() - texwidth) / 3.0f - 20.0f - 8.0f;
	
	ImGui::PushItemWidth( item_width );

	ImGui::SameLine( texwidth,15.0f );

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

	roll = Mor::wrap_angle( roll );
	pitch = Mor::wrap_angle( pitch );
	yaw = Mor::wrap_angle( yaw );

	ImGui::PopItemWidth();
}

Model::Model( Graphics& gfx,const std::string& path )
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
	
	for ( size_t i = 0; i < pScene->mNumMeshes; ++i )
	{
		Meshptr.push_back( std::move(
			ParseMesh( gfx,pScene->mMeshes[i] )
		) );
	}
	const auto pRN = pScene->mRootNode;
	Nodes.push_back( std::make_shared<Node>( nullptr,DirectX::XMMatrixTranspose( *reinterpret_cast<DirectX::FXMMATRIX*>(&pRN->mTransformation) ),pRN->mName.C_Str() ) );
	AnalyseScene( pRN,Nodes.back().get() );
}

void Model::Draw( Graphics& gfx ) noexcept
{
	for ( const auto& n : Nodes )
	{
		const auto NodeTransform = n->GetTransform();
		for ( auto& m : n->GetMeshes() )
		{
			m->RefrenzTransformation( NodeTransform );
			m->Draw( gfx );
		}
	}
}

void Model::ControlTree() noexcept
{
	if ( ImGui::Begin( "Model" ) )
	{
		ImGui::Columns( 2 );
		ImGui::SetColumnOffset( 1,150 );

		CreateBranches( Nodes.front() );

		ImGui::NextColumn();
		if ( ControlledNode )
		{
			ImGui::Text( ControlledNode->GetName().c_str() );
			ControlledNode->ControlWindow();
		}
		ImGui::NextColumn();
	}
	ImGui::End();
}

void Model::CreateBranches( Ref<Node> pNode ) noexcept
{
	auto flags = ImGuiTreeNodeFlags_OpenOnDoubleClick |
				 ImGuiTreeNodeFlags_OpenOnArrow |
				 ImGuiTreeNodeFlags_SpanFullWidth;

	if ( pNode->GetChildren().size() == 0 )
	{
		flags |= ImGuiTreeNodeFlags_Bullet;
	}
	auto open = ImGui::TreeNodeEx( pNode->GetName().c_str(),flags );
	if ( ImGui::IsItemClicked() )
	{
		ControlledNode = pNode;
	}
	if ( open )
	{
		for ( Ref<Node> child : pNode->GetChildren() )
		{
			CreateBranches( child );
		}
		ImGui::TreePop();
	}
}

std::shared_ptr<Mesh> Model::ParseMesh( Graphics& gfx,const aiMesh* pMesh )
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
	return std::make_shared<Mesh>( gfx,std::move( binds ) );
}

void Model::AnalyseScene( const aiNode* pNode,Node* pNodeInterface )
{
	assert( pNode != nullptr );
	assert( pNodeInterface != nullptr );

	// Mesh Part
	for ( size_t i = 0; i < pNode->mNumMeshes; ++i )
	{
		const auto index = pNode->mMeshes[i];
		pNodeInterface->AddMesh( Meshptr[index] );
	}

	// Children Part
	for ( size_t i = 0; i < pNode->mNumChildren; ++i )
	{
		auto pNodeChild = pNode->mChildren[i];
		const auto dxmatrix = DirectX::XMMatrixTranspose( *reinterpret_cast<DirectX::FXMMATRIX*>(&pNodeChild->mTransformation) );
		auto pChild = Node::CreateChild( pNodeInterface,dxmatrix,pNodeChild->mName.C_Str() );
		Nodes.push_back( pChild );
		pNodeInterface->AddChildren( pChild );
		AnalyseScene( pNodeChild,pChild.get() );
	}
}
