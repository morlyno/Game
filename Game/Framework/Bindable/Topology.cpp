#include "Topology.h"

using namespace Bind;

Topology::Topology( D3D_PRIMITIVE_TOPOLOGY pt )
	:
	pt( pt )
{}

void Topology::Bind( Graphics & gfx ) noexcept
{
	GetContext( gfx )->IASetPrimitiveTopology( pt );
}
