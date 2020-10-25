#pragma once

#include <unordered_map>
#include <memory>
#include "Bindable/Bindable.h"

namespace Bind
{

	class Codex
	{
	public:
		static std::shared_ptr<Bindable> Resolve( const std::string& key ) noexcept
		{
			return Get()._Resolve( key );
		}
		static void Store( std::shared_ptr<Bindable> bind ) noexcept
		{
			Get()._Store( std::move( bind ) );
		}
		static void PurchByRef() noexcept
		{
			Get()._PurchByRef();
		}
	private:
		std::shared_ptr<Bindable> _Resolve( const std::string& key ) noexcept
		{
			const auto i = Bindables.find( key );
			if ( i != Bindables.end() )
			{
				return i->second;
			}
			return {};
		}
		void _Store( std::shared_ptr<Bindable> bind ) noexcept
		{
			Bindables[bind->GetKEY()] = bind;
		}
		void _PurchByRef() noexcept
		{
			for ( auto i = Bindables.begin(); i != Bindables.end(); )
			{
				if ( i->second.use_count() == 1 )
				{
					i = Bindables.erase( i );
				}
				else
				{
					++i;
				}
			}
		}
	public:
		static Codex& Get() noexcept
		{
			static Codex codex;
			return codex;
		}
	private:
		std::unordered_map<std::string,std::shared_ptr<Bindable>> Bindables;
	};

}