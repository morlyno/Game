#pragma once
#include "Drawable.h"
#include "../ErrorHandle/MorExeption.h"

//class Model
//{
//	class Mesh : public Drawable
//	{
//	public:
//		Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>>&& binds,DirectX::FXMMATRIX matrix,const std::string& name );
//		DirectX::XMMATRIX GetTransformXM() const noexcept;
//		bool SpawnControlWindow() noexcept;
//		std::string GetType() const noexcept;
//	private:
//		DirectX::XMMATRIX ParentTransform;
//		std::string name;
//		float roll = 0.0f;
//		float pitch = 0.0f;
//		float yaw = 0.0f;
//	};
//public:
//	Model( Graphics& gfx,const std::string& path );
//	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
//	void SpawnContolWindow() noexcept;
//private:
//	void ParseMesh( const struct aiNode* pChild,const struct aiScene* scene );
//	void CreateMesh( struct aiMesh* mesh,DirectX::FXMMATRIX matrix );
//private:
//	Graphics& gfx;
//	std::vector<std::unique_ptr<Mesh>> meshes;
//	std::string modelname;
//};

class Mesh : public Drawable
{
public:
	Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>> binds );
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	std::string GetType() const noexcept;
	void RefrenzTransformation( DirectX::FXMMATRIX matrix );
private:
	DirectX::XMMATRIX Transformation = DirectX::XMMatrixIdentity();
};

class Node
{
	template<typename T> using Ref = std::shared_ptr<T>;
public: //-----Create-----//
	Node( Node* pParent,DirectX::FXMMATRIX matrix,const std::string& name );
	void AddChildren( Ref<Node> pChild );
	static std::shared_ptr<Node> CreateChild( Node* pParent,DirectX::FXMMATRIX matrix,const std::string& name );
	void AddMesh( Ref<Mesh> pMesh );
public: //-----Access-----//
	std::vector<Ref<Mesh>> GetMeshes() const noexcept;
	std::vector<Ref<Node>> GetChildren() const noexcept;
	DirectX::XMMATRIX GetTransform() const noexcept;
	void ControlWindow() noexcept;
	std::string GetName() const noexcept
	{
		return name;
	}
private:
	const Node* pParent;
	std::vector<Ref<Node>> Children;
	std::vector<Ref<Mesh>> Meshes;
	DirectX::XMMATRIX RelativeNodeTransformation;
	DirectX::XMMATRIX OwneTransform = DirectX::XMMatrixIdentity();
	std::string name;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};

class Model
{
	template<typename T> using Ref = std::shared_ptr<T>;
public:
	Model( Graphics& gfx,const std::string& path );
	void Draw( Graphics& gfx ) noexcept;
	void ControlTree() noexcept;
private:
	void CreateBranches( Ref<Node> pNode ) noexcept;
private:
	std::shared_ptr<Mesh> ParseMesh( Graphics& gfx,const struct aiMesh* pMesh );
	void AnalyseScene( const struct aiNode* pNode,Node* pNodeInterface );
private:
	std::vector<std::shared_ptr<Mesh>> Meshptr;
	std::vector<std::shared_ptr<Node>> Nodes;
	Ref<Node> ControlledNode;
};