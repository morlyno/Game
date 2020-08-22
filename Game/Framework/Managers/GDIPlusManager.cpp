#define FULL_WINTARD
#include "../Utility/MorWin.h"

#include "GDIPlusManager.h"

#include <algorithm>

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>

#pragma comment( lib,"gdiplus.lib" )

//static Init
int GDIPlusManager::RefCount = 0;
ULONG_PTR GDIPlusManager::token = 0;


GDIPlusManager::GDIPlusManager()
{
	if ( RefCount == 0 )
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup( &token,&input,nullptr );
	}
	++RefCount;
}

GDIPlusManager::~GDIPlusManager()
{
	--RefCount;
	if ( RefCount == 0 )
	{
		Gdiplus::GdiplusShutdown( token );
	}
}
