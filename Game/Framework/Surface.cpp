#include "Surface.h"

#define FULL_WINTARD
#include "Utility/MorWin.h"

#include <algorithm>

namespace Gdiplus
{
    using std::min;
    using std::max;
}
#include <gdiplus.h>

namespace gdi = Gdiplus;

#include <utility>
#include <stdexcept>
#include <sstream>
#include <cassert>

Surface::Surface( unsigned int width,unsigned int height ) noexcept
    :
    width( width ),
    height( height ),
    pBuffer( std::make_unique<Color[]>( (size_t)( width * height ) ) )
{
}

Surface::Surface( Surface&& other ) noexcept
    :
    width( other.width ),
    height( other.height ),
    pBuffer( std::move( other.pBuffer ) )
{
}

Surface& Surface::operator=( Surface&& donor ) noexcept
{
    width = donor.width;
    height = donor.height;
    pBuffer = std::move( donor.pBuffer );
    donor.pBuffer = nullptr;
    return *this;
}

Surface Surface::FromFile( std::wstring filename )
{
    gdi::Bitmap bitmap( filename.c_str() );
    
    if ( bitmap.GetLastStatus() != gdi::Ok )
    {
        throw SurfaceException( __LINE__,__FILE__,"Failed to load file",filename );
    }

    unsigned int width = bitmap.GetWidth();
    unsigned int height = bitmap.GetHeight();
    std::unique_ptr<Color[]> pBuffer = std::make_unique<Color[]>( width * height );

    for ( unsigned int y = 0; y < height; ++y )
    {
        for ( unsigned int x = 0; x < width; ++x )
        {
            gdi::Color c;
            bitmap.GetPixel( x,y,&c );
            pBuffer[y * width + x] = c.GetValue();
        }
    }
    return Surface( width,height,std::move( pBuffer ) );
}

unsigned int Surface::GetWidht() const noexcept
{
    return width;
}

unsigned int Surface::GetHeight() const noexcept
{
    return height;
}

Surface::Color* Surface::GetBufferPointer() noexcept
{
    return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPointer() const noexcept
{
    return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPointerConst() const noexcept
{
    return pBuffer.get();
}

void Surface::Copy( const Surface& s ) noexcept( !IS_DEBUG )
{
    assert( width == s.width );
    assert( height == s.height );
    memcpy( pBuffer.get(),s.pBuffer.get(),width * height * sizeof( Color ) );
}

Surface::Surface( unsigned int width,unsigned int height,std::unique_ptr<Color[]> pBufferParent ) noexcept
    :
    width( width ),
    height( height ),
    pBuffer( std::move( pBufferParent ) )
{
}

Surface::SurfaceException::SurfaceException( int line,const char* file,const char* msg ) noexcept
    :
    MorException( line,file ),
    msg( std::move( msg ) ),
    filename( std::wstring() )
{
}

Surface::SurfaceException::SurfaceException( int line,const char* file,const char* msg,const std::wstring& filename ) noexcept
    :
    MorException( line,file ),
    msg( std::move( msg ) ),
    filename( filename )
{
}
const char* Surface::SurfaceException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString() << std::endl
        << "\n[Message]\n" << msg;
    if ( filename.size() > 0 )
    {
        std::string s;
        s.resize( filename.size() );
        std::transform( filename.begin(),filename.end(),s.begin(),[]( wchar_t c ) { return (char)c; } );
        oss << std::endl << s;
    }
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Surface::SurfaceException::GetType() const noexcept
{
    return "Morl Surface Exception";
}
