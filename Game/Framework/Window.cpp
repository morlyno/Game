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

	if ( !AdjustWindowRect( &rect,WS_OVERLAPPEDWINDOW,FALSE ) )
	{
		throw MWND_LAST_EXCEPT();
	}

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
	if ( hWnd == nullptr )
	{
		throw MWND_LAST_EXCEPT();
	}
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
	case WM_MOUSEMOVE:
		if ( pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height )
		{
			mouse.MouseMove( pt.x,pt.y );
			if ( !mouse.inwindow )
			{
				SetCapture( hWnd );
				mouse.EnterWindow( pt.x,pt.y );
			}
		}
		else
		{
			if ( wParam & ( MK_LBUTTON | MK_RBUTTON ) )
			{
				mouse.MouseMove( pt.x,pt.y );
			}
			else
			{
				mouse.LeaveWindow( pt.x,pt.y );
				ReleaseCapture();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		mouse.LeftPresst( pt.x,pt.y );
		break;
	case WM_RBUTTONDOWN:
		mouse.RightPresst( pt.x,pt.y );
		break;
	case WM_MBUTTONDOWN:
		mouse.MittelPresst( pt.x,pt.y );
		break;
	case WM_LBUTTONUP:
		mouse.LeftReleast( pt.x,pt.y );
		break;
	case WM_RBUTTONUP:
		mouse.RightReleast( pt.x,pt.y );
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
	if ( uMsg == WM_LBUTTONUP )
	{
		OutputDebugString( L"L Up\n" );
	}
	if ( uMsg == WM_LBUTTONDOWN )
	{
		OutputDebugString( L"L Down\n" );
	}
	if ( uMsg == WM_RBUTTONUP )
	{
		OutputDebugString( L"R Up\n" );
	}
	if ( uMsg == WM_RBUTTONDOWN )
	{
		OutputDebugString( L"R Down\n" );
	}
	return DefWindowProc( hWnd,uMsg,wParam,lParam );
}

//Exception Stuff
Window::Exception::Exception( int line,const char* file,HRESULT hr ) noexcept
	:
	MorException( line,file ),
	hr( hr )
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[ErrorCode] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Mor Window Exeption";
}

std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,hr,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>( &pMsgBuf ),0,nullptr
	);
	// 0 string length returned indicates a failure
	if ( nMsgLen == 0 )
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree( pMsgBuf );
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode( hr );
}
