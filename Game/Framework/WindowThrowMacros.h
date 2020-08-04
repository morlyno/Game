#pragma once

#define WND_EXEPT( hr ) Window::HrException( __LINE__,__FILE__,hr )
#define WND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )

#define NO_GFX() Window::NoGfxException( __LINE__,__FILE__ )