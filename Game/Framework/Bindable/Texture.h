#pragma once

#include "Bindable.h"
#include "../Surface.h"
#include <memory>

namespace Bind
{

	class Texture : public Bindable
	{
	public:
		Texture( Graphics& gfx,const Surface& s,UINT slot );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<Texture> Resolve( Graphics& gfx,const Surface& s,UINT slot ) noexcept;
		static std::string GenerateKey( const std::string& filename,UINT slot ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
		std::string filename;
		UINT slot;
	};

}