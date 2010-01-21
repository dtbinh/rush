/***********************************************************************************/
//  File:   JCamera.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JCAMERA_H__
#define __JCAMERA_H__

#include "jwidget.h"
#include "mat4.h"

enum JSpace
{
    Space_World      = 0,
    Space_View       = 1,
    Space_Projection = 2,
    Space_Screen     = 3
}; // enum JSpace

/***********************************************************************************/
//  Class:  JCamera
//  Desc:
/***********************************************************************************/
class JCamera : public JWidget
{
public:
                    JCamera         ();
    bool            IsOrtho         () const;
    void            SetOrtho        ( bool bOrtho );

    void            SetWorldTM      ( const Mat4& tm );
    void            SetProjTM       ( const Mat4& tm );

	const Mat4&		GetWorldTM		() const { return m_WorldTM; }
	const Mat4&		GetProjTM		() const { return m_ProjTM; }
	Mat4			GetViewTM		() const;

    bool            GetTM           ( JSpace src, JSpace dest, Mat4& tm ) const;

    void            SetViewTM       ( const Mat4& tm );
    float           GetFOVx         () const;
    void            SetFOVx         ( float val );

    float           GetZNear        () const;
    void            SetZNear        ( float val );

    float           GetZFar         () const;
    void            SetZFar         ( float val );

    float           GetAspect       () const;
    void            SetAspect       ( float val );

    Vec3            GetViewDir      () const;
    void            SetViewDir      ( Vec3 dir );

    Vec3            GetViewUp       () const;
    void            SetViewUp       ( Vec3 up );

    Vec3            GetPos          () const;
    void            SetPos          ( Vec3 at );

    virtual void    Render          ();
    virtual void    DrawBounds      ();

    virtual void    PostRender      ();

    expose(JCamera)
    {
        parent(JWidget);

        prop( "ZNear",        GetZNear,      SetZNear         );
        prop( "ZFar",         GetZFar,       SetZFar          );
        prop( "FOVx",         GetFOVx,       SetFOVx          );
        prop( "Aspect",       GetAspect,     SetAspect        );
        prop( "Ortho",        IsOrtho,       SetOrtho         );

        //  FIXME: must be "position"
        prop( "Pos",          GetPos,        SetPos           );
        prop( "ViewDir",      GetViewDir,    SetViewDir       );
        prop( "ViewUp",       GetViewUp,     SetViewUp        );

        field( "RestoreState", m_bRestoreState );
    }

private:
	float			m_ZNear;
	float			m_ZFar;
	float			m_FOVx;
	float			m_Aspect;
	Vec3			m_ViewDir;
	Vec3			m_ViewUp;
	bool            m_bOrtho;


	Mat4            m_WorldTM;
    Mat4            m_ProjTM;

    bool            m_bRestoreState;
    Frame           m_PreviousVP;
    Mat4            m_SpaceTM[4][4];

	void			ValidateProjTM();
	void			ValidateProjParam();
}; // class JCamera

/***********************************************************************************/
//  JCamera inlines
/***********************************************************************************/
inline float JCamera::GetFOVx() const
{
    return RadToDeg( m_FOVx );
} // JCamera::GetFOVx

inline void JCamera::SetFOVx( float val )
{
    m_FOVx = DegToRad( val );
    ValidateProjTM();
} // JCamera::SetFOVx

inline float JCamera::GetZNear() const
{
    return m_ZNear;
} // JCamera::GetZNear

inline float JCamera::GetZFar() const
{
    return m_ZFar;
} // JCamera::GetZFar

inline float JCamera::GetAspect() const
{
    return m_Aspect;
} // JCamera::GetAspect

inline void JCamera::SetZNear( float val )
{
    m_ZNear = val;
    ValidateProjTM();
} // JCamera::SetZNear

inline void JCamera::SetZFar( float val )
{
    m_ZFar = val;
    ValidateProjTM();
} // JCamera::SetZFar

inline void JCamera::SetAspect( float val )
{
    m_Aspect = val;
    ValidateProjTM();
} // JCamera::SetAspect

inline void JCamera::SetOrtho( bool bOrtho )
{
	bOrtho = bOrtho;
    ValidateProjTM();
} // JCamera::SetOrtho

inline bool JCamera::IsOrtho() const
{
    return m_bOrtho;
}

inline void JCamera::SetViewTM( const Mat4& tm )
{
    m_WorldTM.inverse( tm );
} // JCamera::SetViewTM

inline void JCamera::SetWorldTM( const Mat4& tm )
{
    m_WorldTM = tm;
} // JCamera::SetWorldTM

inline void JCamera::SetProjTM( const Mat4& tm )
{
    m_ProjTM = tm;
    ValidateProjParam();
} // JCamera::SetProjTM

inline Vec3 JCamera::GetPos() const
{
    return Vec3( m_WorldTM.e30, m_WorldTM.e31, m_WorldTM.e32 );
}

inline Vec3 JCamera::GetViewDir() const
{
    return Vec3( m_WorldTM.e20, m_WorldTM.e21, m_WorldTM.e22 );
}

inline Vec3 JCamera::GetViewUp() const
{
    return Vec3( m_WorldTM.e10, m_WorldTM.e11, m_WorldTM.e12 );
}

inline void JCamera::SetPos( Vec3 at )
{
    m_WorldTM.e30 = at.x;
    m_WorldTM.e31 = at.y;
    m_WorldTM.e32 = at.z;
} // JCamera::SetPos

inline Mat4 JCamera::GetViewTM() const
{
	Mat4 vtm;
	vtm.inverse( m_WorldTM );
	return vtm;
} // JCamera::GetViewTM

#endif //__JCAMERA_H__
