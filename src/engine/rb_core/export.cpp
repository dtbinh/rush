#include "stdafx.h"
#include "JReflect.h"

#include "IWindowServer.h"
#include "ISoundServer.h"
#include "IScriptServer.h"
#include "IRenderServer.h"
#include "IParticleServer.h"
#include "IModelServer.h"
#include "IFileServer.h"
#include "IDrawServer.h"
#include "IPersistServer.h"
#include "IStringServer.h"
#include "ISignalServer.h"

IWindowServer*      g_pWindowServer     = NULL;
ISoundServer*       g_pSoundServer      = NULL;
IScriptServer*      g_pScriptServer     = NULL;
IRenderServer*      g_pRenderServer     = NULL;
IParticleServer*    g_pParticleServer   = NULL;
IModelServer*       g_pModelServer      = NULL;
IFileServer*        g_pFileServer       = NULL;
IDrawServer*        g_pDrawServer       = NULL;
IPersistServer*     g_pPersistServer    = NULL;
IStringServer*      g_pStringServer     = NULL;
ISignalServer*      g_pSignalServer     = NULL;


export(rb_core)
{
    link_class(JObject);
    link_class(JRef);
    link_class(JGroup);
    link_class(JFileServer);
    link_class(JPersistServer);
    link_class(JSignalServer);
    link_class(JStringServer);

    link_class(JCore);
}
