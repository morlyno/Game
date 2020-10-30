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
		static std::shared_ptr<Bindable> Store( std::shared_ptr<Bindable> bind ) noexcept
		{
			return Get()._Store( bind );
		}
		static void PurchUniques() noexcept
		{
			Get()._PurchUniques();
		}
	private:
		std::shared_ptr<Bindable> _Resolve( const std::string& key ) noexcept
		{
			const auto bind = Bindables.find( key );
			if ( bind != Bindables.end() )
			{
				return bind->second;
			}
			return {};
		}
		std::shared_ptr<Bindable> _Store( std::shared_ptr<Bindable> bind ) noexcept
		{
			return Bindables[bind->GetKey()] = bind;
		}
		void _PurchUniques() noexcept
		{
			for ( auto it = Bindables.begin(); it != Bindables.end(); )
			{
				if ( it->second.use_count() == 1 )
				{
					it = Bindables.erase( it );
				}
				else
				{
					++it;
				}
			}
		}
		static Codex& Get() noexcept
		{
			static Codex codex;
			return codex;
		}
		std::unordered_map<std::string,std::shared_ptr<Bindable>> Bindables;
	};

}