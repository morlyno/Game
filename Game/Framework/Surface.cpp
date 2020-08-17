#include "Surface.h"

#define FULL_WINTARD
#include "MorWin.h"

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

Surface Surface::FromFile( std::wstring filename ) noexcept( !IS_DEBUG )
{
    gdi::Bitmap bitmap( filename.c_str() );
    
    if ( bitmap.GetLastStatus() != gdi::Ok )
    {
        throw std::runtime_error( "failed to load file" );
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

Surface::Surface( unsigned int width,unsigned int height,std::unique_ptr<Color[]> pBufferParent ) noexcept
    :
    width( width ),
    height( height ),
    pBuffer( std::move( pBufferParent ) )
{
}

