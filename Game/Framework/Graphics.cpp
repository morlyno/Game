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

//void Graphics::Drawsdjsgldfg( float angle )
//{
//	HRESULT hr;
//
//	struct Vertex
//	{
//		float x;
//		float y;
//	};
//
//	const Vertex vertecies[] =
//	{
//		{ 0.1f,0.8f },
//		{ 0.5f,0.5f },
//		{ 0.2f,-0.7f },
//		{ 0.0f,-1.0f },
//		{ -0.2f,-0.7f },
//		{ -0.5f,0.5f },
//		{ -0.1f,0.8f },
//	};
//
//	//Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
//
//	//D3D11_BUFFER_DESC bd = {};
//	//bd.ByteWidth = sizeof( vertecies );
//	//bd.StructureByteStride = sizeof( Vertex );
//	//bd.Usage = D3D11_USAGE_DEFAULT;
//	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	//bd.CPUAccessFlags = 0u;
//	//bd.MiscFlags = 0u;
//	//
//	//D3D11_SUBRESOURCE_DATA sd = {};
//	//sd.pSysMem = vertecies;
//	//
//	//GFX_THROW_INFO( pDevice->CreateBuffer(
//	//	&bd,
//	//	&sd,
//	//	&pVertexBuffer
//	//) );
//	//
//	//const UINT stride = sizeof( Vertex );
//	//const UINT offset = 0u;
//
//	//pContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
//
//
//	//Index Buffer
//	const unsigned short indices[] =
//	{
//		0,1,2,
//		0,2,3,
//		0,3,6,
//		6,3,4,
//		6,4,5,
//	};
//
//
//	//D3D11_BUFFER_DESC bdi = {};
//	//bdi.ByteWidth = sizeof( indices );
//	//bdi.StructureByteStride = sizeof( unsigned int );
//	//bdi.Usage = D3D11_USAGE_DEFAULT;
//	//bdi.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	//bdi.CPUAccessFlags = 0u;
//	//bdi.MiscFlags = 0u;
//
//	//D3D11_SUBRESOURCE_DATA sdi = {};
//	//sdi.pSysMem = indices;
//
//	//Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
//
//	//GFX_THROW_INFO( pDevice->CreateBuffer( &bdi,&sdi,&pIndexBuffer ) );
//
//	//pContext->IASetIndexBuffer( pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u );
//
//
//	////Constand Buffer
//	//struct ConstantBuffer
//	//{
//	//	struct
//	//	{
//	//		float element[4][4];
//	//	} transformation;
//	//};
//
//	//const ConstantBuffer cb =
//	//{
//	//	{
//	//		std::cos( angle ),	std::sin( angle ),	0.0f,	0.0f,
//	//		-std::sin( angle ),	std::cos( angle ),	0.0f,	0.0f,
//	//		0.0f,				0.0f,				1.0f,	0.0f,
//	//		0.0f,				0.0f,				0.0f,	1.0f,
//	//	}
//	//};
//
//	//Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
//	//D3D11_BUFFER_DESC cbd = {};
//	//cbd.ByteWidth = sizeof( cb );
//	//cbd.Usage = D3D11_USAGE_DYNAMIC;
//	//cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	//cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	//cbd.MiscFlags = 0u;
//	//cbd.StructureByteStride = 0u;
//
//	//D3D11_SUBRESOURCE_DATA csd = {};
//	//csd.pSysMem = &cb;
//
//	//GFX_THROW_INFO( pDevice->CreateBuffer( &cbd,&csd,&pConstantBuffer ) );
//
//	//pContext->VSSetConstantBuffers( 0u,1u,pConstantBuffer.GetAddressOf() );
//
//
//	////const color
//	//struct constantbuffer2
//	//{
//	//	struct
//	//	{
//	//		float r;
//	//		float g;
//	//		float b;
//	//		float a;
//	//	} colors;
//	//};
//
//	//const constantbuffer2 cb2[5] =
//	//{
//	//	{ 1.0f,0.0f,0.0f },
//	//	{ 0.0f,1.0f,0.0f },
//	//	{ 0.0f,0.0f,1.0f },
//	//	{ 1.0f,1.0f,0.0f },
//	//	{ 1.0f,0.0f,1.0f },
//	//};
//
//	//Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
//	//D3D11_BUFFER_DESC cbd2 = {};
//	//cbd2.ByteWidth = sizeof( cb2 );
//	//cbd2.Usage = D3D11_USAGE_DYNAMIC;
//	//cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	//cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	//cbd2.MiscFlags = 0u;
//	//cbd2.StructureByteStride = 0u;
//
//	//D3D11_SUBRESOURCE_DATA csd2 = {};
//	//csd2.pSysMem = &cb2;
//
//	//GFX_THROW_INFO( pDevice->CreateBuffer( &cbd2,&csd2,&pConstantBuffer2 ) );
//
//	//pContext->PSSetConstantBuffers( 0u,1u,pConstantBuffer2.GetAddressOf() );
//
//
//
//	////Create PixelShader
//	//Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
//	//GFX_THROW_INFO( D3DReadFileToBlob( L"Framework/Shader/PixelShader.cso",&pBlob ) );
//	//GFX_THROW_INFO( pDevice->CreatePixelShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pPixelShader ) );
//
//	////Set PixelShader
//	//GFX_THROW_INFO_ONLY( pContext->PSSetShader( pPixelShader.Get(),nullptr,0u ) );
//
//
//	////Create VertexShader
//	//Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
//	//GFX_THROW_INFO( D3DReadFileToBlob( L"Framework/Shader/VertexShader.cso",&pBlob ) );
//	//GFX_THROW_INFO( pDevice->CreateVertexShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pVertexShader ) );
//
//	////Set VertexShader
//	//GFX_THROW_INFO_ONLY( pContext->VSSetShader( pVertexShader.Get(),nullptr,0u ) );
//
//
//
//	////Creat input layout
//	//Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
//	//const D3D11_INPUT_ELEMENT_DESC ied[] =
//	//{
//	//	{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
//	//};
//
//	//GFX_THROW_INFO( pDevice->CreateInputLayout(
//	//	ied,
//	//	(UINT)std::size( ied ),
//	//	pBlob->GetBufferPointer(),
//	//	pBlob->GetBufferSize(),
//	//	&pInputLayout
//	//) );
//
//
//	////Bind Input Layout
//	//pContext->IASetInputLayout( pInputLayout.Get() );
//
//
//	//Bind Render Target
//	pContext->OMSetRenderTargets(
//		1u,
//		pRenderTargetView.GetAddressOf(),
//		nullptr
//	);
//
//
//	////Set Primitve Topology
//	//pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
//
//
//	//Set ViewPort
//	D3D11_VIEWPORT vp = {};
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	vp.Width = (float)WindowWidth;
//	vp.Height = (float)WindowHeight;
//	vp.MinDepth = 0;
//	vp.MaxDepth = 1;
//	pContext->RSSetViewports( 1u,&vp );
//
//
//	//Draw
//	//GFX_THROW_INFO_ONLY( pContext->Draw( (UINT)std::size( vertecies ),0u ) );
//	GFX_THROW_INFO_ONLY( pContext->DrawIndexed( std::size( indices ),0u,0u ) );
//}

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
