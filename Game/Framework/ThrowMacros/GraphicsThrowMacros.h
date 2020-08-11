#pragma once

#define	GFX_THROW_FAILD(hrcall) if ( FAILED( hr = hrcall ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPTION(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,hr )

#ifdef DEBUG
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if ( FAILED( hr = hrcall ) ) throw Graphics::HrException( __LINE__,__FILE__,hr,infoManager.GetMessages() )
#define GFX_EXCEPT_INFO(hr) infoManager.Set(); Graphics::HrException( __LINE__,__FILE__,hr,infoManager.GetMessages() )
#define GFX_THROW_DEVICE_REMOVED_INFO(hr) infoManager.Set(); throw Graphics::DeviceRemovedException( __LINE__,__FILE__,hr,infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); { auto v = infoManager.GetMessages(); if ( !v.empty() ) { throw Graphics::InfoException( __LINE__,__FILE__,v ); } }
#else
#define GFX_THROW_INFO(hrcall) if ( FAILED( hr = hrcall ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_EXCEPT_INFO(hr) Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_THROW_DEVICE_REMOVED_INFO(hr) throw Graphics::DeviceRemovedException( __LINE__,__FILE__,hr )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif