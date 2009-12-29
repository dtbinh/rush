/***********************************************************************************/
//  File:   Shader.h
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "RenderState.h"
#include "TexturePass.h"

const int c_MaxPasses = 8;
/***********************************************************************************/
//  Class:  Shader
//  Desc:   
/***********************************************************************************/
class Shader : public JObject
{
    RenderState*        m_pRenderState;
    TexturePass*        m_pPass[c_MaxPasses];

     DWORD              m_Handle;

public:
                        Shader();
    virtual void        Render();

    expose(Shader) 
    {
        parent(JObject);
    }
}; // class Shader

#endif //__SHADER_H__