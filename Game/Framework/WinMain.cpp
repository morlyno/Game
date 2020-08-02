
#include "Window.h"
#include "MorExeption.h"
#include <sstream>

int WINAPI wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow )
{
	try
	{
		Window wnd( 800,600,L"SexyWindow" );
		while ( true )
		{
			if ( const auto ecode = Window::ProcessingMessage() )
			{
				return *ecode;
			}
			wnd.Gfx().ClearBuffer( 1.0f,0.0f,0.0f );
			std::wostringstream oss;
			oss << L"width: " << wnd.GetWidth() << L" height: " << wnd.GetHeight();
			wnd.SetWindowTitle( oss.str().c_str() );
			wnd.Gfx().EndFrame();
		}
	}
	catch ( const MorException& e )
	{
		MessageBoxA( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION );
	}
	catch( const std::exception & e )
	{
		MessageBoxA( nullptr,e.what(),"Standart Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( ... )
	{
		MessageBoxA( nullptr,"No details available","Unknown Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}