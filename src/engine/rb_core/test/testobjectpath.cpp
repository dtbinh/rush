#include "stdafx.h"
#include "JObject.h"
#include "JObjectServer.h"

static const char* c_TestScript = "\
group \"g_root\"\n\
{\n\
    group \"fruits\"\n\
    {\n\
        group \"nice\"\n\
        {\n\
            group \"apple\"{}\n\
            group \"orange\"{}\n\
            group \"banana\"{}\n\
        }\n\
        group \"bad\"\n\
        {\n\
            group \"pear\"{}\n\
            group \"egg\"{}\n\
        }\n\
    }\n\
    group \"vegetables\"\n\
    {\n\
        group \"nice\"\n\
        {\n\
            group \"tomato\"{}\n\
            group \"cucubmer\"{}\n\
            group \"potato\"{}\n\
        }\n\
        group \"notnice\"\n\
        {\n\
            group \"radish\"{}\n\
            group \"egg\"{}\n\
        }\n\
        group \"other\"\n\
        {\n\
            group \"white\"\n\
            {\n\
                group \"mushroom\"{}\n\
            }\n\
            group \"none\" {}\n\
        }\n\
    }\n\
}\n";

TEST(ObjectPath)
{
    //  create object structure
    JObject* pRoot1 = g_pObjectServer->CreateFromScript( c_TestScript );
    JObject* pRoot2 = g_pObjectServer->CreateFromScript( c_TestScript );
    CHECK( pRoot1 != NULL );
    CHECK( pRoot2 != NULL );
}