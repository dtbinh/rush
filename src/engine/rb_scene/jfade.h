//****************************************************************************/
//  File:   JFade.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JFADE_H__
#define __JFADE_H__

#include "JWeakRef.h"

enum JFadeMode
{
    FadeMode_None   = 0,
    FadeMode_In     = 1,
    FadeMode_Out    = 2,
    FadeMode_OutIn  = 3
}; // enum FadeMode

enum_beg(JFadeMode)
    enum_val( "None",   FadeMode_None  ),
    enum_val( "In",     FadeMode_In    ),
    enum_val( "Out",    FadeMode_Out   )
enum_end(JFadeMode)

//****************************************************************************/
//  Class:  JFade
//  Desc:   Used to fade in/out part of the screen
//****************************************************************************/
class JFade : public JAnimation
{
    JString             m_FadeObject;
    JWeakRef<JWidget>   m_pFadeObject;
    JFadeMode           m_FadeMode;
    DWORD               m_Color;
    bool                m_bFadeColor;

public: 
                        JFade          ();
    virtual void        Render         (); 
    virtual void        Init           ();
    virtual void        OnStop         ();

    expose( JFade )
    {
        parent(JAnimation);
        field( "Mode",          m_FadeMode      );
        field( "Color",         m_Color         );
        field( "FadeObject",    m_FadeObject    );
        field( "FadeColor",     m_bFadeColor    );
    }
}; // class JFade

#endif // __JFADE_H__


