/***********************************************************************************/
//  File:   Shader.cpp
//  Date:   16.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "Shader.h"

/***********************************************************************************/
/*  Shader implementation
/***********************************************************************************/
decl_class( Shader );
Shader::Shader()
{
    m_Handle = 0xFFFFFFFF;
} // Shader::Shader

void Shader::Render()
{
    for (int i = 0; i < GetNChildren(); i++) 
    { 
        GetChild( i )->Render();
    }
} // Shader::Render
