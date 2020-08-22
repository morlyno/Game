#pragma once

#include <memory>
#include <string>
#include "ErrorHandle/MorExeption.h"

class Surface
{
public:
	class SurfaceException : public MorException
	{
	public:
		SurfaceException( int line,const char* file,const char* msg ) noexcept;
		SurfaceException( int line,const char* file,const char* msg,const std::wstring& filename ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
	private:
		std::string msg;
		std::wstring filename;
	};
public:
	class Color
	{
	private:
		unsigned int argb;
	public:
		Color() = default;
		Color( unsigned char r,unsigned char g,unsigned b ) noexcept
		{
			argb = ( ( r << 16u ) | ( g << 8u ) | b );
		}
		Color( unsigned char a,unsigned char r,unsigned char g,unsigned b ) noexcept
		{
			argb = ( ( a << 24u ) | ( r << 16u ) | ( g << 8u ) | b );
		}
		Color( unsigned int dw ) noexcept
			:
			argb( dw )
		{}
		void SetA( unsigned char a ) noexcept
		{
			argb = ( argb & 0x00FFFFFFu ) | ( a << 24u );
		}
		void SetR( unsigned char r ) noexcept
		{
			argb = ( argb & 0xFF00FFFFu ) | ( r << 16u );
		}
		void SetG( unsigned char g ) noexcept
		{
			argb = ( argb & 0xFFFF00FFu ) | ( g << 8u );
		}
		void SetB( unsigned char b ) noexcept
		{
			argb = ( argb & 0xFFFFFF00u ) | b;
		}
		unsigned char GetA() const noexcept
		{
			return (unsigned char)( ( argb >> 24u ) & 0xFFu );
		}
		unsigned char GetR() const noexcept
		{
			return (unsigned char)( ( argb >> 16u ) & 0xFFu );
		}
		unsigned char GetG() const noexcept
		{
			return (unsigned char)( ( argb >> 8u ) & 0xFFu );
		}
		unsigned char GetB() const noexcept
		{
			return (unsigned char)( argb & 0xFFu );
		}
	};
public:
	Surface( unsigned int width,unsigned int height ) noexcept;
	Surface( Surface& ) = delete;
	Surface( Surface&& other ) noexcept;
	Surface& operator=( Surface& ) = delete;
	Surface& operator=( Surface&& donor ) noexcept;
	static Surface FromFile( std::wstring filename ) noexcept( !IS_DEBUG );
	unsigned int GetWidht() const noexcept;
	unsigned int GetHeight() const noexcept;
	Color* GetBufferPointer() noexcept;
	const Color* GetBufferPointer() const noexcept;
	const Color* GetBufferPointerConst() const noexcept;
	void Copy( const Surface& s ) noexcept( !IS_DEBUG );
private:
	Surface( unsigned int width,unsigned int height,std::unique_ptr<Color[]> pBufferParent ) noexcept;
private:
	unsigned int width;
	unsigned int height;
	std::unique_ptr<Color[]> pBuffer;
};