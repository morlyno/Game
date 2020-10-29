#pragma once

#include "../Graphics.h"
#include <stdexcept>
#include <cassert>

namespace Bind
{

	class Bindable
	{
	public:
		virtual void Bind( Graphics& gfx ) noexcept = 0;
		std::string GetKEY() const noexcept;
		void SetKEY( const std::string& key ) noexcept;
		virtual ~Bindable() = default;
		virtual std::string GetKey() const noexcept { assert( false && "!!!BIB!!! ERROR !!!BIB!!!" ); return {}; }
	protected:
		static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
		static ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
		static DxgiInfo& GetInfoManager( Graphics& gfx ) noexcept(IS_DEBUG);
		std::string key;
	};

}