#pragma once

#include "Bindable.h"

namespace Bind
{

	class Topology : public Bindable
	{
	public:
		Topology( D3D_PRIMITIVE_TOPOLOGY pt );
		void Bind( Graphics& gfx ) noexcept override;
	private:
		D3D_PRIMITIVE_TOPOLOGY pt;
	};

}