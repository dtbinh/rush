//****************************************************************************/
//  File:   JProgress.h
//  Date:   18.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPROGRESS_H__
#define __JPROGRESS_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JProgress
//  Desc:   Progress bar
//****************************************************************************/
class JProgress : public JWidget
{
    int             m_FillFrame;        //  whether tile filling sprite to the bar extents
    bool            m_bVertical;            
    
    bool            m_bEditable;        //  whether user can change position with mouse

    //  border margins
    int             m_LeftBorder;
    int             m_TopBorder;
    int             m_RightBorder;
    int             m_BottomBorder;          

    float           m_Position;         //  current position [0..1]
    float           m_MinVal;
    float           m_MaxVal;

    bool            m_bDiscrete;
    float           m_DiscreteStep;
    
    bool            m_bCaptureOnDrag;

public:
                    JProgress     ();
    virtual void    Render        (); 
    virtual void    OnMouse       ( JMouseEvent& m );

    float           GetScaledPos  () const { return m_MinVal + (m_MaxVal - m_MinVal)*m_Position; } 
    void            SetScalesPos  ( float val ) { m_Position = (val - m_MinVal)/(m_MaxVal - m_MinVal); }

    expose( JProgress )
    {
        parent(JWidget);
        field( "Position",      m_Position      );
        field( "Vertical",      m_bVertical     );
        field( "FillFrame",     m_FillFrame     );
        field( "Editable",      m_bEditable     );
        field( "LeftBorder",    m_LeftBorder    );
        field( "TopBorder",     m_TopBorder     );
        field( "RightBorder",   m_RightBorder   );
        field( "BottomBorder",  m_BottomBorder  );
        field( "Discrete",      m_bDiscrete     );
        field( "DiscreteStep",  m_DiscreteStep  );
        field( "CaptureOnDrag", m_bCaptureOnDrag );

        prop ( "ScaledPosition", GetScaledPos, SetScalesPos );
        field( "MinVal",  m_MinVal  );
        field( "MaxVal",  m_MaxVal  );
    }
}; // class JProgress

#endif // __JPROGRESS_H__


