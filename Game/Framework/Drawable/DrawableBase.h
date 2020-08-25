#pragma once

#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
public:
	bool IsInitialized() const
	{
		return !staticbinds.empty();
	}
	void AddStaticBind( std::unique_ptr<Bindable> addbind ) noexcept( !IS_DEBUG )
	{
		assert( "Use AddStaticIndexBuffer for binding an IndexBuffer" && typeid( *addbind ) != typeid( IndexBuffer ) );
		staticbinds.push_back( std::move( addbind ) );
	}
	void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> ibuf ) noexcept( !IS_DEBUG )
	{
		assert( "pIndexBuffer already set" && pIndexBuffer == nullptr );
		pIndexBuffer = ibuf.get();
		staticbinds.push_back( std::move( ibuf ) );
	}
	void SetIndexBufferFromStatic() noexcept( !IS_DEBUG )
	{
		assert( "pIndexBuffer already set" && pIndexBuffer == nullptr );
		for ( const auto& sb : staticbinds )
		{
			if ( const auto pib = dynamic_cast<IndexBuffer*>( sb.get() ) )
			{
				pIndexBuffer = pib;
				return;
			}
		}
		assert( "Faild to find IndexBuffer form Static" && pIndexBuffer == nullptr );
	}
	void ClearStatic()
	{
		staticbinds.clear();
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticbinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticbinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticbinds;