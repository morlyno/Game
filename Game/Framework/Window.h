#pragma once

#include "MorWin.h"
#include "ErrorHandle/MorExeption.h"
#include "Inputs/Mouse.h"
#include "Inputs/Keyboard.h"
#include "Graphics.h"
#include <string>
#include <optional>
#include <memory>

class Window
{
public:
	class Exception : public MorException
	{
		using MorException::MorException;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
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
	Window( int width,int height,LPCWSTR pWndName,bool CloseAll = true );
	~Window();
	static std::optional<int> ProcessingMessage();
	BOOL SetWindowTitle( const std::wstring& title );
	int GetWidth() const;
	int GetHeight() const;
public:
	Mouse mouse;
	Keyboard kbd;
	Graphics& Gfx() const;
private:
	static LRESULT WINAPI WindowProcStartUp( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	static LRESULT WINAPI WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	LRESULT WINAPI HandleMsg( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
	UINT width;
	UINT height;
	bool CloseAll;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};
