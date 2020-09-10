#pragma once

template<class Container>
void erase_by_index( Container& con,size_t index )
{
	con.erase( con.begin() + index );
}