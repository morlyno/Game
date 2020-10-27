#include "Topology.h"
#include <typeinfo>

using namespace Bind;

Topology::Topology( D3D_PRIMITIVE_TOPOLOGY pt )
	:
	pt( pt )
{}

void Topology::Bind( Graphics & gfx ) noexcept
{
	GetContext( gfx )->IASetPrimitiveTopology( pt );
}

std::string Bind::Topology::GenerateKey( const std::string& topo ) noexcept
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + topo;
}

std::string Bind::Topology::GetKey() const noexcept
{
	return GenerateKey( std::to_string( pt ) );
}
