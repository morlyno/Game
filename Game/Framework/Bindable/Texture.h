#pragma once

#include "Bindable.h"
#include "../Surface.h"

namespace Bind
{

	class Texture : public Bindable
	{
	public:
		Texture( Graphics& gfx,const Surface& s );
		void Bind( Graphics& gfx ) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};

}