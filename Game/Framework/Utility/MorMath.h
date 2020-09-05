#pragma once

static constexpr float PI = 3.14159265f;
static constexpr double PI_D = 3.1415926535897932;

template<typename T>
T fmod( T a,T b )
{
	return a - int( a / b ) * b;
}

template<typename T>
T wrap_angle( T rad )
{
	T r = (rad + (T)PI);
	r = fmod( r,((T)2.0f * (T)PI) );
	if ( r < (T)0.0f )
	{
		r += ((T)2.0f * (T)PI);
	}
	return r - (T)PI;
	//T i = fmod( rad,(T)PI_D );
	//if ( i < (T)-PI )
	//{
	//	i += ((T)PI * (T)2.0f);
	//}
	//return i;
}

template<typename T>
T sq( T val )
{
	return val * val;
}

template<typename T>
T expo( T val,int e )
{
	T rslt = val;
	for ( int i = 0; e < i; ++i )
	{
		rslt *= rslt;
	}
	return rslt;
}