/***********************************************************************************/
//  File:   ColGeom.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __COLGEOM_H__
#define __COLGEOM_H__

#include "ode/ode.h"
#include "PhysBody.h"
#include "PhysEntity.h"
#include "PhysMaterial.h"
#include "JWeakRef.h"

enum WorldAxis
{
    waNone  = 0,
    waX     = 1,
    waY     = 2,
    waZ     = 3
}; // enum WorldAxis

enum_beg(WorldAxis)
enum_val( "None",   waNone ),
enum_val( "X",      waX    ),
enum_val( "Y",      waY    ),
enum_val( "Z",      waZ    )
enum_end(WorldAxis)

/***********************************************************************************/
//  Class:  ColGeom
//  Desc:   
/***********************************************************************************/
class ColGeom : public PhysEntity
{
public:
                            ColGeom         ();
                            ~ColGeom        ();
    
    Mat4                    GetTM           () const;
    void                    SetTM           ( const Mat4& tm ); 
    virtual bool            IsPlaceable     () const { return true; }

    Vec3                    GetPos          () const;
    void                    SetPos          ( Vec3 pos );

    Vec3                    GetRotation     () const;
    void                    SetRotation     ( Vec3 pos );

    virtual void            Init            ();

    virtual dGeomID         CreateGeom      ( dSpaceID spaceID ) { return 0; } 
    const char*             GetHost         () const { return m_HostName.c_str(); }
    void                    SetHost         ( const char* name ) { m_HostName = name; }

    const Mat4&             GetOffsetTM     () const;
    void                    SetOffsetTM     ( const Mat4& tm );

    virtual dMass           GetMass         () const;

    virtual void            Synchronize     ( bool bFromSolver );
    virtual void            BindToSolver    ();

    dGeomID                 GetID           () const;
    float                   GetBodyScale    () const;
    float                   GetDensity      () const;

    expose(ColGeom)
    {
        parent(PhysEntity);
        field( "Host",       m_HostName                 );
        field( "Material",   m_Material                 );
        prop ( "Position",        GetPos, SetPos             );
        prop ( "Rotation",        GetRotation, SetRotation   );
        field( "OffsetTM",   m_OffsetTM                 );
    }

protected:
    JWeakRef<PhysMaterial>  m_pMaterial;    //  geom material

private:
    dGeomID                 m_ID;       
    JWeakRef<PhysBody>      m_pBody;        //  host body (if dynamic)
    int                     m_BoneID;       //  host bone (if static)

    Mat4                    m_TM;           //  current world transform of the geom
    Mat4                    m_OffsetTM;     //  offset transform from the host body or bone    
    JString                 m_HostName;     //  name of the host body (for dynamic geoms) or bone (static ones)
    JString                 m_Material;     //  name of the geom material

}; // class ColGeom

/***********************************************************************************/
/*  ColGeom inlines
/***********************************************************************************/
inline Vec3 ColGeom::GetPos() const 
{ 
    return m_TM.translation(); 
}

inline void ColGeom::SetPos( Vec3 pos ) 
{ 
    m_TM.translation() = pos; 
} 

inline void ColGeom::SetTM( const Mat4& tm )
{
    m_TM = tm;
} // ColGeom::SetTM

inline Vec3 ColGeom::GetRotation() const
{
    Mat3 rot( m_TM );
    return rot.to_euler();
} // ColGeom::GetRotation

inline void ColGeom::SetRotation( Vec3 pos )
{
    Mat3 rot;
    rot.from_euler( DegToRad( pos.x ), DegToRad( pos.y ), DegToRad( pos.z ) );
    Vec3 tr = m_TM.translation();
    m_TM = Mat4( rot, tr );
} // ColGeom::SetRotation

inline Mat4 ColGeom::GetTM() const
{
    return m_TM;
} // ColGeom::GetTM

inline const Mat4& ColGeom::GetOffsetTM() const
{
    return m_OffsetTM;
}

inline void ColGeom::SetOffsetTM( const Mat4& tm )
{
    m_OffsetTM = tm;
}

inline dGeomID ColGeom::GetID() const
{
    return m_ID;
}

inline float ColGeom::GetBodyScale() const
{
    if (m_pBody)
    {
        return m_pBody->GetBodyScale();
    }
    Vec3 scale, trans;
    Mat3 rot;
    m_TM.factorize( scale, rot, trans );
    return scale.x;
} // ColGeom::GetBodyScale

inline dMass ColGeom::GetMass() const 
{ 
    dMass m; 
    dMassSetZero( &m ); 
    return m; 
}

inline float ColGeom::GetDensity() const
{
    float density = m_pMaterial ? m_pMaterial->GetDensity() : 5.0f;
    return density;
}

#endif //__COLGEOM_H__