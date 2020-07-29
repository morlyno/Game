#pragma once

#include "MorWin.h"
#include "MorExeption.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <string>
#include <optional>

class Window
{
public:
	class Exception : public MorException
	{
	public:
		Exception( int line,const char* file,HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WindowClass
	{
	public:
		static HINSTANCE gethInst();
		static LPCWSTR getName();
	private:
		WindowClass();
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
	private:
		static constexpr LPCWSTR pName = L"Game";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window( int width,int height,LPCWSTR pWndName );
	~Window();
	static std::optional<int> ProcessingMessage()
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
	};
public:
	Mouse mouse;
	Keyboard kbd;
private:
	static LRESULT WINAPI WindowProcStartUp( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	static LRESULT WINAPI WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	LRESULT WINAPI HandleMsg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	int width;
	int height;
public:
	HWND hWnd;
};

#define MWND_EXEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define MWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )