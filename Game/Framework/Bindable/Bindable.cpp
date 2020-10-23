#include "Bindable.h"

using namespace Bind;

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
