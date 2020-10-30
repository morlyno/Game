#pragma once

#include "Bindable.h"
#include <memory>

namespace Bind
{

	class Topology : public Bindable
	{
	public:
		Topology( D3D_PRIMITIVE_TOPOLOGY pt );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<Topology> Resolve( D3D_PRIMITIVE_TOPOLOGY pt ) noexcept;
		static std::string GenerateKey( const std::string& topo ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		D3D_PRIMITIVE_TOPOLOGY pt;
	};

}