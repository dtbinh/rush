/***********************************************************************************/
//  File:   JAnimInstance.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMODELANIMINSTANCE_H__
#define __JMODELANIMINSTANCE_H__
#include "jweakref.h"

/***********************************************************************************/
//  Class:  JAnimInstance
//  Desc:
/***********************************************************************************/
class JAnimInstance : public JAnimation
{
public:
                    JAnimInstance   ();
    virtual void    Render          ();
    virtual void    Init            ();
    virtual void    OnPlay          ();

    void            SetModel        ( const char* name );
    void            SetAnim         ( const char* name );

    const char*     GetModel        () const { return m_Model.c_str(); }
    const char*     GetAnim         () const { return m_Anim.c_str(); }
    int             GetAnimID       () const { return m_AnimID; }
    int             GetModelID      () const { return m_ModelID; }

    const char*     GetSection      () const { return m_Section.c_str(); }
    void            SetSection      ( const char* name );

    expose( JAnimInstance )
    {
        parent( JAnimation );
        field( "Animation",     m_Anim      );
        field( "ModelInstance", m_Instance  );
        field( "Model",         m_Model     );
        field( "BlendInTime",   m_BlendInTime );
        prop ( "Section",       GetSection, SetSection );
    }

private:
    JString                     m_Anim;
    JString                     m_Model;
    JString                     m_Instance;
    JString                     m_Section;
    float                       m_Weight;

    int                         m_AnimID;
    int                         m_ModelID;
    int                         m_SectionID;

    float                       m_BlendInTime;

    int                         m_InstanceID;
    JWeakRef<JModelInstance>    m_pInstance;

    void                        EnsureInited();

}; // class JAnimInstance

#endif //__JMODELANIMINSTANCE_H__
