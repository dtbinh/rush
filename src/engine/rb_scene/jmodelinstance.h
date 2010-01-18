/***********************************************************************************/
//  File:   JModelInstance.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMODELINSTANCE_H__
#define __JMODELINSTANCE_H__

/***********************************************************************************/
//  Class:  JModelInstance
//  Desc:   
/***********************************************************************************/
class JModelInstance : public JObject
{
public:
                            JModelInstance      ();
    virtual void            Render              ();   
    virtual void            DrawBounds          (); 
    virtual void            ResInit             ();  
    int                     GetInstanceID       () const { return m_InstanceID; }
    int                     GetModelID          () const { return m_ModelID; }

    void                    GetWorldTM          ( Mat4& tm ) const;

    const char*             GetAttachInstance   () const { return m_AttachInstance.c_str(); }
    const char*             GetAttachBone       () const { return m_AttachBone.c_str(); }
    
    void                    SetAttachInstance   ( const char* name );
    void                    SetAttachBone       ( const char* name );

    float                   GetTransparency     () const;
    void                    SetTransparency     ( float val );

    const char*             GetModel            () const { return m_Model.c_str(); }
    void                    SetModel            ( const char* model );

    expose( JModelInstance )
    {
        parent( JObject );
        field( "Model",             m_Model             );
        rprop( "ModelID",           GetModelID          ); 
        field( "Scale",             m_Scale             );
        field( "Position",          m_Position          );
        field( "Rotation",          m_Rotation          );
        field( "HasShadow",         m_bHasShadow        );
        rprop( "InstanceID",        GetInstanceID       ); 
        field( "Color",             m_Color             );
        field( "ZBias",             m_ZBias             );
        prop ( "Transparency",      GetTransparency,    SetTransparency     );
        prop ( "AttachInstance",    GetAttachInstance,  SetAttachInstance   );
        prop ( "AttachBone",        GetAttachBone,      SetAttachBone       );
        field( "CameraPlacement",   m_bCameraPlacement  );
        field( "PosX",              m_Position.x        );
        field( "PosY",              m_Position.y        );
        field( "PosZ",              m_Position.z        );
        field( "UseCamera",         m_bUseCamera        );
    }

private:
    JString                 m_Model;
    float                   m_Scale;
    Vec3                    m_Position;
    Vec3                    m_Rotation;
    int                     m_ModelID;
    int                     m_Dummy;

    uint32_t                   m_Color;
    JString                 m_AttachInstance;     
    JString                 m_AttachBone;         

    int                     m_InstanceID;

    bool                    m_bHasShadow;    
    bool                    m_bDrawSkeleton;

    bool                    m_bCameraPlacement;
    bool                    m_bUseCamera;

    Mat4                    m_PreTM;
    float                   m_ZBias;

    int                     m_AttachInstanceID; 
    int                     m_AttachBoneID;     

}; // class JModelInstance

#endif //__JMODELINSTANCE_H__