#pragma once
#include "../Utility/MorWin.h"
#include <wrl.h>
#include <vector>
#include <dxgidebug.h>
#include <string>

class DxgiInfo
{
public:
	DxgiInfo();
	~DxgiInfo() = default;
	DxgiInfo( const DxgiInfo& ) = delete;
	DxgiInfo& operator=( const DxgiInfo& ) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};