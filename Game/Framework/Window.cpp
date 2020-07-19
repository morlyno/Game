#include "Window.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

HINSTANCE Window::WindowClass::gethInst()
{
	return wndClass.hInst;
}

LPCWSTR Window::WindowClass::getName()
{
	return pName;
}

Window::WindowClass::WindowClass()
	:
	hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProcStartUp;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pName;
	wc.hIconSm = nullptr;
	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( pName,hInst );
}

Window::Window( int width,int height,LPCWSTR pWndName )
	:
	width( width ),
	height( height )
{
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	AdjustWindowRect( &rect,WS_OVERLAPPEDWINDOW,FALSE );
	hWnd = CreateWindowEx(
		0,
		WindowClass::getName(),
		pWndName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		rect.right - rect.left,rect.bottom - rect.top,
		nullptr,
		nullptr,
		WindowClass::gethInst(),
		this
	);
	ShowWindow( hWnd,SW_SHOWDEFAULT );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

LRESULT WINAPI Window::WindowProcStartUp( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	if ( uMsg == WM_NCCREATE )
	{
		const CREATESTRUCTW* const pCreateStruct = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWindow = static_cast<Window*>( pCreateStruct->lpCreateParams );
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>( pWindow ) );
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>( &WindowProc ) );
		return pWindow->HandleMsg( hWnd,uMsg,wParam,lParam );
	}
	return DefWindowProc( hWnd,uMsg,wParam,lParam );
}

LRESULT WINAPI Window::WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	Window* const pWindow = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd,GWLP_USERDATA ) );
	return pWindow->HandleMsg( hWnd,uMsg,wParam,lParam );
}

LRESULT WINAPI Window::HandleMsg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	const POINTS pt = MAKEPOINTS( lParam );
	switch ( uMsg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	/*Mouse Messages*/
	case WM_NCMOUSEMOVE:
		mouse.LeaveWindow( pt.x,pt.y );
		break;
	case WM_MOUSEMOVE:
		mouse.MouseMove( pt.x,pt.y );
		break;
	case WM_LBUTTONDOWN:
		mouse.LeftPresst( pt.x,pt.y );
		break;
	case WM_RBUTTONDOWN:
		mouse.RightPresst( pt.x,pt.y );
		break;
	case WM_LBUTTONUP:
		mouse.LeftReleast( pt.x,pt.y );
		break;
	case WM_RBUTTONUP:
		mouse.RightReleast( pt.x,pt.y );
		break;
	case WM_MBUTTONDOWN:
		mouse.MittelPresst( pt.x,pt.y );
		break;
	case WM_MBUTTONUP:
		mouse.MittelReleast( pt.x,pt.y );
		break;
	case WM_MOUSEWHEEL:
	{
		if ( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
		{
			mouse.WheelUp( pt.x,pt.y );
		}
		if ( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
		{
			mouse.WheelDown( pt.x,pt.y );
		}
	}
	/*Mouse Messegas Ends*/
	}
	return DefWindowProc( hWnd,uMsg,wParam,lParam );
}
