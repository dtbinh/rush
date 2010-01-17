/***********************************************************************************/
//  File:   JBoneInstance.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JBONEINSTANCE_H__
#define __JBONEINSTANCE_H__

#include "mat4.h"

class JBone;
/***********************************************************************************/
/*  Struct:  JBoneInstance
//  Desc:    Bone, instanced in the world
/***********************************************************************************/
struct JBoneInstance
{
    int             m_Index;

    Mat4            m_LocalTM;  //  bone instance transform in parent bone space
    Mat4            m_ModelTM;  //  bone instance transform int model space (root bone)

    bool            m_bVisible;

    int             m_ParentIdx;
    JBone*          m_pBone;

    JBoneInstance() :
        m_Index      ( -1 ),
        m_LocalTM    ( Mat4::identity ),
        m_ModelTM    ( Mat4::identity ),
        m_bVisible   ( true ),
        m_ParentIdx  ( -1 ),
        m_pBone      ( NULL )
    {}

}; // class JBoneInstance

#endif //__JBONEINSTANCE_H__
