//****************************************************************************/
//  File:   JCursor.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JCURSOR_H__
#define __JCURSOR_H__

//****************************************************************************/
//  Class:  JCursor
//  Desc:   
//****************************************************************************/
class JCursor : public JSwitch
{
    bool            m_bHideSysCursor;
    Vec2            m_Position;
    Frame           m_CurExt;

public:
                    JCursor         ();
    virtual         ~JCursor        ();
    virtual void    Render          ();
    virtual void    DrawBounds      ();
    virtual void    PostRender      ();
    bool            IsHideSysCursor () const { return m_bHideSysCursor; }
    void            HideSysCursor   ( bool bHide );
    Vec2            GetPosition     () const { return m_Position; }
    void            SetPosition     ( Vec2 pos );

    expose( JCursor )
    {
        parent( JSwitch );
        prop( "HideSysCursor", IsHideSysCursor, HideSysCursor );
        prop( "Position", GetPosition, SetPosition );
    }


protected:

}; // class JCursor

#endif // __JCURSOR_H__


