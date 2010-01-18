/***********************************************************************************/
//  File:   JEffectInstance.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JEFFECTINSTANCE_H__
#define __JEFFECTINSTANCE_H__

const float c_RestartTimeout = 1.0f;
/***********************************************************************************/
//  Class:  JEffectInstance
//  Desc:   
/***********************************************************************************/
class JEffectInstance : public JAnimation
{
public:
                    JEffectInstance     ();
    virtual void    Render              ();
    virtual void    Init                ();
    virtual void    PostRender          ();
    virtual void    OnPlay              (); 
    virtual void    OnStop              (); 

    const char*     GetAttachInstance   () const { return m_AttachInstance.c_str(); }
    const char*     GetAttachBone       () const { return m_AttachBone.c_str(); }
    
    void            SetAttachInstance   ( const char* name );
    void            SetAttachBone       ( const char* name );

    expose( JEffectInstance )
    {
        parent( JAnimation );
        field( "Effect",            m_Effect        );
        field( "EffectID",          m_EffectID      );
        field( "Scale",             m_Scale         );
        field( "Alpha",             m_Alpha         );
        field( "Position",          m_Position      );
        field( "Rotation",          m_Rotation      );
        field( "PosX",              m_Position.x    );
        field( "PosY",              m_Position.y    );
        field( "PosZ",              m_Position.z    );
        field( "ScreenSpace",       m_bScreenSpace  );
        prop ( "AttachInstance",    GetAttachInstance, SetAttachInstance );
        prop ( "AttachBone",        GetAttachBone, SetAttachBone );
    }

private:
    JString         m_Effect;
    float           m_Scale;
    float           m_Alpha;
    Vec3            m_Position;
    Vec3            m_Rotation;
    int             m_EffectID;
    int             m_InstanceID;
    uint32_t           m_Stamp;
    float           m_PrevTime; 
    bool            m_bScreenSpace;

    JString         m_AttachInstance;     
    JString         m_AttachBone;         
    int             m_AttachInstanceID; 
    int             m_AttachBoneID;  
}; // class JEffectInstance

#endif //__JEFFECTINSTANCE_H__