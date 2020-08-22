#include "Graphics.h"
#include <sstream>
#include "Macros/GraphicsThrowMacros.h"
#include "ImGui/imgui_impl_dx11.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


Graphics::Graphics( UINT width,UINT height,HWND hWnd )
	:
	WindowWidth( width ),
	WindowHeight( height ),
	hWnd( hWnd )
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = 0u;
	scd.BufferDesc.Height = 0u;
	scd.BufferDesc.RefreshRate.Denominator = 0u;
	scd.BufferDesc.RefreshRate.Numerator = 0u;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1u;
	scd.SampleDesc.Quality = 0u;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1u;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0u;

	UINT deviceswapFlag = 0u;

#ifdef DEBUG
	deviceswapFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceswapFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	) );

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO( pSwapChain->GetBuffer( 0u,__uuidof( ID3D11Resource ),&pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView
	) );

	pContext->OMSetRenderTargets(
		1u,
		pRenderTargetView.GetAddressOf(),
		nullptr
	);

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)WindowWidth;
	vp.Height = (float)WindowHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	pContext->RSSetViewports( 1u,&vp );

	//ImGui Setup
	ImGui_ImplDX11_Init( pDevice.Get(),pContext.Get() );
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if ( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if ( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			GFX_THROW_DEVICE_REMOVED_INFO( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			GFX_THROW_INFO( hr );
		}
	}
}

void Graphics::ClearBuffer( float red,float green,float blue ) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(),color );
}

void Graphics::DrawIndexed( UINT indexcount ) noexcept( !IS_DEBUG )
{
	GFX_THROW_INFO_ONLY( pContext->DrawIndexed( indexcount,0u,0u ) );
}

void Graphics::SetProjection( const DirectX::XMMATRIX& proj ) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetCamera( DirectX::CXMMATRIX cam ) noexcept
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
	return camera;
}

Graphics::HrException::HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsg ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{
	for ( const auto& m : infoMsg )
	{
		info += m;
		info.push_back( '\n' );
	}
	if ( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[ErrorCode] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Desciption] " << GetErrorDesciption() << std::endl;
	if ( !info.empty() )
	{
		oss << "[Error Info] " << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Mor Gfx Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

std::string Graphics::HrException::GetErrorDesciption() const noexcept
{
	return TranslateErrorCode( hr );
}

std::string Graphics::HrException::TranslateErrorCode( HRESULT hr ) noexcept
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

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Mor Gfx Device Removed Exception";
}

Graphics::InfoException::InfoException( int line,const char* file,std::vector<std::string> infoMsg ) noexcept
	:
	Exception( line,file )
{
	for ( const auto& m : infoMsg )
	{
		info += m;
		info.push_back( '\n' );
	}
	if ( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Info]" << std::endl
		<< GetErrorInfo() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Mor Gfx Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
