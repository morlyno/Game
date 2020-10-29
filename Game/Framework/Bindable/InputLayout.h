#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"

class VertexLayout;

namespace Bind
{

	class InputLayout : public Bindable
	{
	public:
		InputLayout( Graphics& gfx,const VertexLayout& vl,ID3DBlob* pVertexShaderBytecode );
		void Bind( Graphics& gfx ) noexcept override;
		static std::string GenerateKey( const std::string& layout ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		std::string layout;
	};

}