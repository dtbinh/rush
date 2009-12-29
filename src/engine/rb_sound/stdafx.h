#ifndef __RB_SOUND_PCH_H__
#define __RB_SOUND_PCH_H__
#pragma once

// Works with Windows 2000 and later and Windows 98 or later
#undef _WIN32_IE
#undef WINVER
#undef _WIN32_WINDOWS
#undef _WIN32_WINNT
#define WINVER         0x0500 
#define _WIN32_WINDOWS 0x0410 
#define _WIN32_WINNT   0x0500 

#include <windows.h>

#include "rb_common.h"
#include "Logger.h"
#include <wchar.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#define DXUT_ERR(A,B) S_FALSE

#include "ISoundServer.h"
#include <dsound.h>
#include "FStream.h"
#include "JReflect.h"

#endif // __RB_SOUND_PCH_H__

