#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include <memory>

namespace Bind
{

	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer( Graphics& gfx,std::vector<unsigned short> indices,const std::string& tag );
		void Bind( Graphics& gfx ) noexcept override;
		UINT GetCount() const;
		static std::shared_ptr<IndexBuffer> Resolve( Graphics& gfx,std::vector<unsigned short> indices,const std::string& tag ) noexcept;
		static std::string GenerateKey( const std::string& tag ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		const UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
		std::string tag;
	};

}