/***********************************************************************************/
//  File:   JHotSpot.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JHOTSPOT_H__
#define __JHOTSPOT_H__

class JCharControl;
/***********************************************************************************/
//  Class:  JHotSpot
//  Desc:   
/***********************************************************************************/
class JHotSpot : public JPolyButton
{
public:
                        JHotSpot    ();
    virtual void        Render      ();
    virtual void        DrawBounds  ();
    virtual void        Init        ();
    virtual void        OnUnpress   ();

    virtual void        OnCharIn    () {}

    virtual void        OnHover     ();
    virtual void        OnStopHover ();
    
    expose( JHotSpot )
    {
        parent( JPolyButton );
        field ( "passive",      m_bPassive      );
        field ( "attractchar",  m_bAttractChar  );
        field ( "direction",    m_Direction     );
        method( "oncharin",     OnCharIn        );
    }

private:
    bool                m_bAttractChar;
    bool                m_bPassive;
    float               m_Direction;

    bool                InSpot          ( const Vec3& pos ) const;
    JCharControl*       GetCharControl  () const;
}; // class JHotSpot

#endif //__JHOTSPOT_H__