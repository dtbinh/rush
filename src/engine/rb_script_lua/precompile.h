#pragma once
#pragma warning ( disable : 4267 )

#include "rb_common.h"

#include <vector>
#include <string>
#include <hash_map>

#include "assert.h"
#include "math.h"
#include "stdlib.h"

#include "JReflect.h"
#include "Logger.h"

#include "stdlib.h"
#include "JString.h"
#include "FStream.h"
#include "RegPool.h"
#include "Path.h"
#include "IScriptServer.h"

#pragma warning ( disable : 4311 )
#pragma warning ( disable : 4312 )
#pragma warning ( disable : 4267 )
#pragma warning ( disable : 4020 )

extern "C"
{
#include "lua.h"
#include "lstate.h"
#include "lualib.h"
#include "lauxlib.h"
//#include "pluto.h"
}

