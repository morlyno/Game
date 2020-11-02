#pragma once

#include "Utility/MorWin.h"
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
		static HINSTANCE gethInst() noexcept;
		static LPCWSTR getName() noexcept;
	private:
		WindowClass();
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
	private:
		static constexpr LPCWSTR pName = L"Morl's DirectX Graphics Framework";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window( int width,int height,LPCWSTR pWndName,bool CloseAll = true );
	~Window();
	static std::optional<int> ProcessingMessage() noexcept;
	bool SetWindowTitle( const std::wstring& title ) noexcept;
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	void Kill() const noexcept;
	void ShowMouse() noexcept;
	void HideMouse() noexcept;
	void ClipMouse() noexcept;
	void ReleasMouse() noexcept;
	void ReadRawInputs() noexcept;
	void ReadNormalInputs() noexcept;
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
	UINT size = sizeof( RAWINPUT );
	RAWINPUT raw;
	bool MouseDisplayed = true;
	bool MouseCliped = false;
	bool RawMouse = false;
};
