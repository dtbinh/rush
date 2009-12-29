#ifndef __RB_UI_PCH_H__
#define __RB_UI_PCH_H__


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define STRICT

#include "rb_common.h"
#include "Logger.h"

#include <vector>
#include <string>

#include "IRenderServer.h"
#include "IDrawServer.h"

#include "PolyLine2.h"
#include "Frame.h"
#include "JObject.h"
#include "JMathTypeCast.h"
#include "JMouseEvent.h"
#include "JKeyEvent.h"
#include "JWheelEvent.h"

#include "JReflect.h"
#include "JWidget.h"
#include "JDialog.h"

#include "JWeakRef.h"

#pragma warning (push)
#pragma warning (disable : 4311)
#pragma warning (disable : 4312)
#pragma warning (pop)


#endif // 
