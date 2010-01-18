#ifndef __RB_RENDER_DX9_PCH_H__
#define __RB_RENDER_DX9_PCH_H__
#pragma once
#define WIN32_LEAN_AND_MEAN	

#include "windows.h"
#include "rb_common.h"
#include "Logger.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#endif // __RB_RENDER_DX9_PCH_H__