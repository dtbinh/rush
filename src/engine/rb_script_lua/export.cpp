#include "precompile.h"
#include "jreflect.h"

export(rb_script_lua)
{
    link_class(JLuaServer);
    link_class(JLuaScript);
    link_class(JLuaThread);
}
