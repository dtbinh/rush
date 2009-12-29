/***********************************************************************************/
//  File:   JSetLocation.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSETLOCATION_H__
#define __JSETLOCATION_H__

/***********************************************************************************/
//  Class:  JSetLocation
//  Desc:   
/***********************************************************************************/
class JSetLocation : public JAnimation
{
    JString                 m_Location;
    Vec3                    m_CharPos;
    float                   m_CharDir;
    JString                 m_Delegate;
    JWeakRef<JSetLocation>  m_pDelegate;

public:
                        JSetLocation    ();
    virtual void        OnPlay          ();
    virtual void        Init            ();

    expose( JSetLocation )
    {
        parent( JAnimation );
        field( "Location",  m_Location  );
        field( "CharDir",   m_CharDir   );
        field( "CharPos",   m_CharPos   );
        field( "Delegate",  m_Delegate  );
    }
}; // class JSetLocation

#endif //__JSETLOCATION_H__