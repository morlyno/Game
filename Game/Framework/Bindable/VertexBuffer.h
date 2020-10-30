#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include "../VertexLayout.h"
#include <memory>
#include "../BindableCodex.h"

namespace Bind
{

	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer( Graphics& gfx,const VertexData& vd,const std::string& tag );
		void Bind( Graphics& gfx ) noexcept override;
		template<typename Func>
		static std::shared_ptr<VertexBuffer> Resolve( Graphics& gfx,const VertexData& vd,Func& func,const std::string& tag ) noexcept
		{
			if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( tag ) ) )
			{
				return std::dynamic_pointer_cast<VertexBuffer>(bind);
			}
			func();
			const auto& bind = Bind::Codex::Store( std::make_shared<VertexBuffer>( gfx,vd,tag ) );
			return std::dynamic_pointer_cast<VertexBuffer>(bind);
		}
		static std::string GenerateKey( const std::string& tag ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		const UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		std::string tag;
	};

}