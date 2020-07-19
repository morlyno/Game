
#include "Window.h"
#include <sstream>

class App
{
public:
	App( Window& wnd )
		:
		wnd( wnd )
	{}
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;
public:
	void Go()
	{
		auto pos = wnd.mouse.getPos();
		bool lp = wnd.mouse.LeftIsPresst();
		bool rp = wnd.mouse.RightIsPresst();
		std::wstringstream ss;
		ss << L"( x: " << pos.first << L" y: " << pos.second << L" ) "<< L"left: " << std::boolalpha << lp << L" right: " << std::boolalpha << rp;
		SetWindowText( wnd.hWnd,ss.str().c_str() );
		Mouse::Event e = wnd.mouse.Read();
		std::wstringstream m;
		//m << L"pos: " << e.getPosX() << L" , " << e.getPosY() << std::endl;
		//m << std::boolalpha << e.getLPresst() << L" | " << std::boolalpha << e.getRPresst() << std::endl;
		//m << std::boolalpha << wnd.mouse.inWindow() << std::endl;
		OutputDebugString( m.str().c_str() );
	}
private:
	Window& wnd;
};

int WINAPI wWinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,PWSTR pCmdLine,int nCmdShow )
{
	Window wnd( 800,600,L"SexyWindow" );
	App app( wnd );
	while ( wnd.ProcessingMessage() )
	{
		app.Go();
	}
	return 0;
}