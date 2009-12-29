/***********************************************************************************/
//  File:   JBone.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JBONE_H__
#define __JBONE_H__

/***********************************************************************************/
//  Class:  JBone
//  Desc:   
/***********************************************************************************/
class JBone : public JObject
{
    Mat4        m_TM;

public:
    JBone() : m_TM( Mat4::identity ) {}  

    void        SetTransform( const Mat4& tm )  { m_TM = tm; }
    const Mat4& GetTransform() const            { return m_TM; }

    Vec3        GetPosition () const;
    void        SetPosition ( Vec3 pos );
    Mat3        GetRotation () const;
    void        SetRotation ( Mat3 tm );
    Vec3        GetScale    () const;
    void        SetScale    ( Vec3 scale );

    expose( JBone )
    {
        parent( JObject );
        prop( "Scale",       GetScale,    SetScale    );
        //  FIXME: must be "position" and "rotation"
        prop( "Rot",    GetRotation, SetRotation );
        prop( "Pos",    GetPosition, SetPosition );
    }
}; // class JBone

#endif //__JBONE_H__