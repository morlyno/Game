#include "Window.h"
#include "ThrowMacros/WindowThrowMacros.h"
#include <sstream>
#include <cassert>

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
	wc.hCursor = LoadCursor( nullptr,IDC_ARROW );
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

Window::Window( int width,int height,LPCWSTR pWndName,bool CloseAll )
	:
	width( width ),
	height( height ),
	CloseAll( CloseAll )
{
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;

	if ( AdjustWindowRect( &rect,WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,FALSE ) == 0 )
	{
		throw WND_LAST_EXCEPT();
	}

	hWnd = CreateWindowEx(
		0,
		WindowClass::getName(),
		pWndName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,
		rect.right - rect.left,rect.bottom - rect.top,
		nullptr,
		nullptr,
		WindowClass::gethInst(),
		this
	);
	if ( hWnd == nullptr )
	{
		throw WND_LAST_EXCEPT();
	}

	pGfx = std::make_unique<Graphics>( width,height,hWnd );

	ShowWindow( hWnd,SW_SHOWDEFAULT );
	UpdateWindow( hWnd );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

std::optional<int> Window::ProcessingMessage()
{
	MSG msg = {};
	while ( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		if ( msg.message == WM_QUIT )
		{
			return (int)msg.wParam;
		}
		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}
	return {};
}

BOOL Window::SetWindowTitle( const std::wstring& title )
{
	return SetWindowText( hWnd,title.c_str() );
}

int Window::GetWidth() const
{
	return (int)width;
}

int Window::GetHeight() const
{
	return (int)height;
}

Graphics& Window::Gfx() const
{
	if ( !pGfx )
	{
		throw NO_GFX();
	}
	return *pGfx;
}

LRESULT WINAPI Window::WindowProcStartUp( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	if ( uMsg == WM_NCCREATE )
	{
		const CREATESTRUCTW* const pCreateStruct = reinterpret_cast<CREATESTRUCTW*>( lParam );
		Window* const pWindow = static_cast<Window*>( pCreateStruct->lpCreateParams );
		assert( pWindow && "Window pointer not created" );
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
	switch ( uMsg )
	{
	case WM_DESTROY:
		{
			if ( CloseAll )
			{
				PostQuitMessage( 0 );
			}
			return 0;
		}
	//case WM_SIZE:
	//	{
	//		if ( !( wParam & SIZE_MINIMIZED ) )
	//		{
	//			auto w = LOWORD( lParam );
	//			auto h = HIWORD( lParam );
	//			width = (UINT)w;
	//			height = (UINT)h;
	//		}
	//		break;
	//	}
	/*Mouse Messages*/
	case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			if ( pt.x >= 0 && pt.x < (short)width && pt.y >= 0 && pt.y < (short)height )
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
				const POINTS pt = MAKEPOINTS( lParam );
				if ( wParam & ( MK_LBUTTON | MK_RBUTTON ) )
				{
					mouse.MouseMove( pt.x,pt.y );
				}
				else
				{
					mouse.LeaveWindow( pt.x,pt.y );
					if ( ReleaseCapture() == 0 )
					{
						throw WND_LAST_EXCEPT();
					}
				}
			}
			break;
		}
	case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.LeftPresst( pt.x,pt.y );
			break;
		}
	case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.RightPresst( pt.x,pt.y );
			break;
		}
	case WM_MBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.MittelPresst( pt.x,pt.y );
			break;
		}
	case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.LeftReleast( pt.x,pt.y );
			break;
		}
	case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.RightReleast( pt.x,pt.y );
			break;
		}
	case WM_MBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			mouse.MittelReleast( pt.x,pt.y );
			break;
		}
	case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS( lParam );
			if ( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
			{
				mouse.WheelUp( pt.x,pt.y );
			}
			if ( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
			{
				mouse.WheelDown( pt.x,pt.y );
			}
			break;
		}
	/*Mouse Messegas Ends*/

	/*Keyboard Messegas*/
	case WM_KEYDOWN:
		{
			kbd.OnKeyPresst( wParam );
			break;
		}
	case WM_KEYUP:
		{
			kbd.OnKeyReleas( wParam );
			break;
		}
	case WM_CHAR:
		{
			kbd.OnChar( wParam );
			break;
		}
	}
	/*Keyboard Messegas Ends*/

	return DefWindowProc( hWnd,uMsg,wParam,lParam );
}

//hr Exception Stuff
Window::HrException::HrException( int line,const char* file,HRESULT hr ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{
}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[ErrorCode] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Mor Window Exeption";
}

std::string Window::HrException::TranslateErrorCode( HRESULT hr ) noexcept
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

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorString() const noexcept
{
	return TranslateErrorCode( hr );
}

//gfx Exception Stuff
const char* Window::NoGfxException::GetType() const noexcept
{
	return "Mor Window Exception No Gfx";
}
