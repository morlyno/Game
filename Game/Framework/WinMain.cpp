
#include "Window.h"

int WINAPI wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow )
{
	Window wnd( 800,600,L"SexyWindow" );
	MSG msg = {};
	while ( GetMessageW( &msg,nullptr,0,0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}
}