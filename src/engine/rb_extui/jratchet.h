//****************************************************************************/
//  File:   JRatchet.h
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JRATCHET_H__
#define __JRATCHET_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JRatchet
//  Desc:   Gizmo used for manual value tweaking by dragging mouse
//****************************************************************************/
class JRatchet : public JWidget
{
    JString         m_GizmoSprite;      //  name of the JRatchet animation sprite package
    float           m_GizmoFrameRate;   //  gizmo frame rate multiplier
    
    float           m_DefaultValue;     //  default value
    float           m_ValueStep;        //  normal step for value
    float           m_Value;            //  current value
    bool            m_bVertical;        //  whether it is vertical

    DWORD           m_Color;            

    int             m_SprID;
    int             m_CurFrame;

public:
                    JRatchet            ();
    virtual void    Render              ();

    expose( JRatchet )
    {
        parent( JWidget );
        field( "GizmoSprite",       m_GizmoSprite       );
        field( "GizmoFrameRate",    m_GizmoFrameRate    );
        field( "DefaultValue",      m_DefaultValue      );
        field( "ValueStep",         m_ValueStep         );
        field( "Value",             m_Value             );
        field( "Vertical",          m_bVertical         );
    }


protected:

}; // class JRatchet

#endif // __JRATCHET_H__


