#pragma once
#include "MorWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "MorExeption.h"

class Graphics
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
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDesciption() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};
public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics operator=( const Graphics ) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer( float red,float green,float blue );
	//void Resize( UINT width,UINT height )
	//{
	//	DXGI_MODE_DESC md = { 0 };
	//	md.Width = width;
	//	md.Height = height;
	//	pSwapChain->ResizeTarget( &md );
	//}
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
	HWND											hWnd;
};