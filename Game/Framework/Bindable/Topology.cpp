#include "Topology.h"
#include <typeinfo>
#include "../BindableCodex.h"

using namespace Bind;

Topology::Topology( D3D_PRIMITIVE_TOPOLOGY pt )
	:
	pt( pt )
{}

void Topology::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->IASetPrimitiveTopology( pt );
}

std::shared_ptr<Topology> Bind::Topology::Resolve( D3D_PRIMITIVE_TOPOLOGY pt ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( std::to_string( pt ) ) ) )
	{
		return std::dynamic_pointer_cast<Topology>(bind);
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<Topology>( pt ) );
	return std::dynamic_pointer_cast<Topology>(bind);
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
