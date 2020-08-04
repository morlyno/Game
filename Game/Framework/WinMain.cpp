
#include "App.h"
#include "MorExeption.h"
#include <sstream>

int WINAPI wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow )
{
	try
	{
		return App{}.Go();
	}
	catch ( const MorException& e )
	{
		MessageBoxA( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY );
	}
	catch( const std::exception & e )
	{
		MessageBoxA( nullptr,e.what(),"Standart Exception",MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY );
	}
	catch ( ... )
	{
		MessageBoxA( nullptr,"No details available","Unknown Exception",MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY );
	}
	return -1;
}