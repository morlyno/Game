#pragma once

#include "MorWin.h"

class Window
{
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
private:
	static LRESULT WINAPI WindowProcStartUp( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	static LRESULT WINAPI WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	LRESULT WINAPI HandleMsg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	int width;
	int height;
	HWND hWnd;
};