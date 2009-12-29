/***********************************************************************************/
//  File:   JAnmSprite.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMSPRITE_H__
#define __JANMSPRITE_H__

enum SpriteAlignment
{
    Align_None      = 0,
    Align_Vertical  = 1,
    Align_XAxis     = 2,
    Align_YAxis     = 3,
    Align_Ground    = 4,
    Align_Camera    = 5
}; // enum SpriteAlignment

enum_beg(SpriteAlignment)
    enum_val( "None",       Align_None       ),
    enum_val( "Vertical",   Align_Vertical   ),
    enum_val( "XAxis",      Align_XAxis      ),
    enum_val( "YAxis",      Align_YAxis      ),
    enum_val( "Ground",     Align_Ground     ),
    enum_val( "Camera",     Align_Camera     )
enum_end(SpriteAlignment)

/***********************************************************************************/
//  Class:  JAnmSprite
//  Desc:   
/***********************************************************************************/
class JAnmSprite : public JAnimation
{
public:
                    JAnmSprite          ();
    virtual void    Render              ();
    virtual void    Init                ();
    virtual void    ResInit             ();
    virtual void    OnPlay              ();

    float           GetRotation         () const { return m_Rotation; }
    bool            IsMirrored          () const { return m_bMirrored; }
    bool            IsBackwards         () const { return m_bBackwards; }
    void            SetRotation         ( float rot ) { m_Rotation = rot; }
    void            SetMirrored         ( bool bMirrored ) { m_bMirrored = bMirrored; }
    void            SetBackwards        ( bool bBackwards ) { m_bBackwards = bBackwards; }
    int             GetSpriteID         () const { return m_PackID; }
    int             GetFrameID          () const { return m_FrameID; }
    float           GetPosX             () const { return m_Pos.x; }
    float           GetPosY             () const { return m_Pos.y; }
    float           GetPosZ             () const { return m_Pos.z; }
    void            SetPosX             ( float val ) { m_Pos.x = val; }
    void            SetPosY             ( float val ) { m_Pos.y = val; }
    void            SetPosZ             ( float val ) { m_Pos.z = val; }

    float           GetTransparency     () const;
    void            SetTransparency     ( float val );

    const char*     GetAttachInstance   () const { return m_AttachInstance.c_str(); }
    const char*     GetAttachBone       () const { return m_AttachBone.c_str(); }
    
    void            SetAttachInstance   ( const char* name );
    void            SetAttachBone       ( const char* name );

    expose(JAnmSprite)
    {
        parent(JAnimation);
        field( "StartAngle",    m_StartAngle        );
        field( "EndAngle",      m_EndAngle          );
        field( "Package",       m_PackageName       );
        field( "StartFrame",    m_StartFrame        );
        field( "NumFrames",     m_NumFrames         );
        field( "FPS",           m_FramesPerSec      );
        field( "Pivot",         m_Pivot             );
        field( "Mirrored",      m_bMirrored         );
        field( "Backwards",     m_bBackwards        );
        field( "AlwaysCache",   m_bAlwaysCache      );
        field( "Filter",        m_bFilter           );
        field( "Color",         m_Color             );
        prop ( "Transparency",  GetTransparency, SetTransparency );

        field( "Screenspace",   m_bScreenSpace      );
        field( "Scale",         m_Scale             );
        field( "Position",      m_Pos               );
        field( "Rotation",      m_Rotation          );
        prop ( "PosX",          GetPosX, SetPosX    );
        prop ( "PosY",          GetPosY, SetPosY    );
        prop ( "PosZ",          GetPosZ, SetPosZ    );

        field( "BlendFrames",    m_bBlendFrames     );
        prop ( "Attachinstance", GetAttachInstance, SetAttachInstance );
        prop ( "Attachbone",     GetAttachBone, SetAttachBone );
        field( "Align",          m_Align            );
    }

private:
    int             GetPlayedFrame( float cTime );

    float           m_StartAngle;
    float           m_EndAngle;

    JString         m_PackageName;
    int             m_StartFrame;
    int             m_NumFrames;
    float           m_FramesPerSec;

    Vec2            m_Pivot;
    Vec3            m_Pos;
    float           m_Scale;
    bool            m_bScreenSpace;

    int             m_PackID;
    int             m_FrameID;

    float           m_Rotation;
    bool            m_bMirrored;
    bool            m_bBackwards;
    bool            m_bAlwaysCache;
    bool            m_bFilter;
    bool            m_bBlendFrames;
    DWORD           m_Color;

    JString         m_AttachInstance;
    JString         m_AttachBone;

    int             m_AttachInstanceID;
    int             m_AttachBoneID;


    SpriteAlignment m_Align;

}; // class JAnmSprite

#endif //__JANMSPRITE_H__