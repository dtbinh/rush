//****************************************************************************/
//  File:  JShadowPass.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "jshadowpass.h"
#include "imodelserver.h"

//****************************************************************************/
/*  JShadowPass implementation
//****************************************************************************/
decl_class(JShadowPass);
JShadowPass::JShadowPass()
{
}

JShadowPass::~JShadowPass()
{
}

void JShadowPass::Init()
{
}

void JShadowPass::Render()
{
    g_pModelServer->DrawShadows();
}





