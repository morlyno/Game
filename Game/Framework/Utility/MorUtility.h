#pragma once

namespace Mor
{

	template<class Container>
	void erase_by_index( Container& con,size_t index )
	{
		con.erase( con.begin() + index );
	}

	template<class iter>
	[[nodiscard]] iter wrap_iterator( iter begin,iter end,iter src_iter )
	{
		if ( src_iter == end )
		{
			return begin;
		}
		return src_iter;
	}

}