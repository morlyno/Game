#pragma once
#include "Drawable.h"

class Model
{
	class Mesh : public Drawable
	{
	public:
		Mesh( Graphics& gfx,std::vector<std::shared_ptr<Bind::Bindable>>&& binds,DirectX::FXMMATRIX matrix );
		DirectX::XMMATRIX GetTransformXM() const noexcept;
		bool SpawnControlWindow() noexcept;
		std::string GetType() const noexcept;
	private:
		DirectX::XMMATRIX ParentTransform;
	};
public:
	Model( Graphics& gfx,const std::string& path );
	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
private:
	void ParseMesh( const struct aiNode* pChild,const struct aiScene* scene );
	void CreateMesh( struct aiMesh* mesh,DirectX::FXMMATRIX matrix );
private:
	Graphics& gfx;
	std::vector<std::unique_ptr<Mesh>> meshes;
};