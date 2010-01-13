#include "precompile.h"
#include "jreflect.h"

#include "iwindowserver.h"
#include "isoundserver.h"
#include "iscriptserver.h"
#include "irenderserver.h"
#include "iparticleserver.h"
#include "imodelserver.h"
#include "ifileserver.h"
#include "idrawserver.h"
#include "ipersistserver.h"
#include "istringserver.h"
#include "isignalserver.h"

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
