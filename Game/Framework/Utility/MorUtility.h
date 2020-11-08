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

	template<class DXFLOAT3>
	DXFLOAT3 XMFloat3Add( DXFLOAT3 f0,DXFLOAT3 f1 )
	{
		return { f0.x + f1.x,f0.y + f1.y,f0.z + f1.z };
	}

	template<class DXFLOAT3>
	DXFLOAT3 XMFloat3Sub( DXFLOAT3 f0,DXFLOAT3 f1 )
	{
		return { f0.x - f1.x,f0.y - f1.y,f0.z - f1.z };
	}

	template<class DXFLOAT2>
	DXFLOAT2 XMFloat2Add( DXFLOAT2 f0,DXFLOAT2 f1 )
	{
		return { f0.x + f1.x,f0.y + f1.y };
	}

	template<class DXFLOAT2>
	DXFLOAT2 XMFloat2Sub( DXFLOAT2 f0,DXFLOAT2 f1 )
	{
		return { f0.x - f1.x,f0.y - f1.y };
	}

}