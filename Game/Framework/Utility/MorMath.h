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
	constexpr float Tow_PI = (T)PI * (T)2.0f;
	rad = fmod( rad,Tow_PI );
	if ( rad < (T)0.0f )
	{
		rad += Tow_PI;
	}
	return rad;
}

template<typename T>
T sq( T val )
{
	return val * val;
}

template<typename T>
T expo( T val,int e )
{
	T rslt = 1;
	for ( int i = 0; e < i; ++i )
	{
		rslt *= val;
	}
	return rslt;
}