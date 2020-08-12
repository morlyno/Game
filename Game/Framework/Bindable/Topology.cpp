#include "Topology.h"

Topology::Topology( D3D_PRIMITIVE_TOPOLOGY pt )
	:
	pt( pt )
{}

void Topology::Bind( Graphics & gfx ) noexcept
{
	GetContext( gfx )->IASetPrimitiveTopology( pt );
}
