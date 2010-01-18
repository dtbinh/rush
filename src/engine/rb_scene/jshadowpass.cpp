//****************************************************************************/
//  File:  JShadowPass.cpp
//  Desc:  
//****************************************************************************/
#include "precompile.h"
#include "JShadowPass.h"
#include "IModelServer.h"

//****************************************************************************/
/*  JShadowPass implementation
//****************************************************************************/
decl_class(JShadowPass);
JShadowPass::JShadowPass()
{
} // JShadowPass::JShadowPass

JShadowPass::~JShadowPass()
{
} // JShadowPass::JShadowPass

void JShadowPass::Init()
{   
} // JShadowPass::Init

void JShadowPass::Render()
{   
    g_pModelServer->DrawShadows();
} // JShadowPass::Render





