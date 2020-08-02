#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")

Graphics::Graphics( HWND hWnd )
	:
	hWnd( hWnd )
{
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = 0u;
	scd.BufferDesc.Height = 0u;
	scd.BufferDesc.RefreshRate.Denominator = 0u;
	scd.BufferDesc.RefreshRate.Numerator = 0u;
	scd.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
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

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer( 0u,__uuidof( ID3D11Resource ),&pBackBuffer );
	pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView
	);
}

void Graphics::EndFrame()
{
	pSwapChain->Present( 1u,0u );
}

void Graphics::ClearBuffer( float red,float green,float blue )
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(),color );
}
