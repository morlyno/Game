#pragma once
#include "Drawable.h"
#include "../ErrorHandle/MorExeption.h"
#include <assimp/matrix4x4.h>

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
	Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>> binds,DirectX::FXMMATRIX matrix,const std::string& name );
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	bool SpawnControlWindow() noexcept;
	std::string GetType() const noexcept;
private:
	DirectX::XMMATRIX ParentTransform;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	std::string name;
};

class Model
{
public:
	Model( Graphics& gfx,const std::string& path );
	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
	void SpawnControllWindow() noexcept;
private:
	void AnalyseScene( const struct aiNode* pNode,const struct aiScene* pScene,const aiMatrix4x4 matrix );
	std::vector<std::shared_ptr<Bind::Bindable>> ParseMesh( const struct aiMesh* pMesh );
private:
	Graphics& gfx;
	std::vector<std::unique_ptr<Mesh>> pMeshes;
};