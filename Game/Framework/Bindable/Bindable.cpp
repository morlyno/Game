#include "Bindable.h"

using namespace Bind;

std::string Bind::Bindable::GetKEY() const noexcept
{
	return key;
}

void Bind::Bindable::SetKEY( const std::string& _key ) noexcept
{
	key = _key;
}

ID3D11DeviceContext* Bindable::GetContext( Graphics& gfx ) noexcept
{
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice( Graphics& gfx ) noexcept
{
	return gfx.pDevice.Get();
}

DxgiInfo& Bindable::GetInfoManager( Graphics& gfx ) noexcept( IS_DEBUG )
{
#ifdef DEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "tried accessing infoManager in Releas" );
#endif
}
