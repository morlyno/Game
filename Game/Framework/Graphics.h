#pragma once
#include "MorWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ErrorHandle/MorExeption.h"
#include "ErrorHandle/DxgiInfoManager.h"
#include <vector>

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
		HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsg = {} ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorDesciption() const noexcept;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};
	class InfoException : public Exception
	{
	public:
		InfoException( int line,const char* file,std::vector<std::string> infoMsg ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
public:
	Graphics( UINT width,UINT height,HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics operator=( const Graphics ) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer( float red,float green,float blue );
	void Drawsdjsgldfg( float angle );
	//void Resize( UINT width,UINT height )
	//{
	//	DXGI_MODE_DESC md = { 0 };
	//	md.Width = width;
	//	md.Height = height;
	//	pSwapChain->ResizeTarget( &md );
	//}
private:
#ifdef DEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pPixelShader;
	UINT											WindowWidth;
	UINT											WindowHeight;
	HWND											hWnd;
};