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
			return Get()._Store( bind );
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
		void _Store( std::shared_ptr<Bindable> bind )
		{
			Bindables[bind->GetKey()] = bind;
		}
		static Codex& Get() noexcept
		{
			static Codex codex;
			return codex;
		}
		std::unordered_map<std::string,std::shared_ptr<Bindable>> Bindables;
	};

}