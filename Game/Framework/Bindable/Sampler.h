#pragma once

#include "Bindable.h"

namespace Bind
{

	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx );
		void Bind( Graphics& gfx ) noexcept override;
		static std::string GenerateKey() noexcept;
		std::string GetKey() const noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};

}