// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#pragma warning ( disable : 4312 )

#include "rb_common.h"

#include <direct.h>

#include "math.h"
#include "assert.h"


#include "JReflect.h"
#include "IDrawServer.h"
#include "IRenderServer.h"

#include "Vec3.h"
#include "Vec4.h"
#include "Mat3.h"
#include "Mat4.h"
#include "AABox.h"

#include "Stream.h"
#include "FStream.h"
#include "UniformPool.h"
#include "JMathTypeCast.h"
#include "JWidget.h"
#include "JMesh.h"
#include "JBone.h"
#include "JMaterial.h"
#include "Timer.h"
#include "Path.h"

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "modstack.h"
#include "cstudio/phyexp.h"
#include "iskin.h"
#include "stdmat.h"
#include "decomp.h"
#include "utilapi.h"

#include "maxutils.h"
#include "ExpVertex.h"
#include "ExpNode.h"
#include "ExpFace.h"
#include "TrackPRS.h"

extern HINSTANCE g_hInstance;

bool FileExists( const char* path );
