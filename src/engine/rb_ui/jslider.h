//****************************************************************************/
//  File:   JSlider.h
//  Date:   18.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSLIDER_H__
#define __JSLIDER_H__

//****************************************************************************/
//  Class:  JSlider
//  Desc:   Slidible widget used for setting fixed-bound values
//****************************************************************************/
class JSlider : public JWidget
{
    int             m_ThumbBeg;
    int             m_ThumbMid;
    int             m_ThumbEnd;

    float           m_Position;         //  slider position [0..1]
    
    float           m_Expand;           
    bool            m_bVertical;
    bool            m_bSnapToClick;     //  set current position to the place where mouse down
    bool            m_bCaptureOnDrag;

public:
                    JSlider       ();
    virtual void    Render        (); 
    virtual void    OnMouse       ( JMouseEvent& m );

    expose( JSlider )
    {
        parent(JWidget);
        field( "Position",      m_Position          );
        field( "Expand",        m_Expand            );
        field( "Vertical",      m_bVertical         );
        field( "ThumbBeg",      m_ThumbBeg          );
        field( "ThumbMid",      m_ThumbMid          );
        field( "ThumbEnd",      m_ThumbEnd          );
        field( "SnapToClick",   m_bSnapToClick      );
        field( "CaptureOnDrag", m_bCaptureOnDrag    );
    }
}; // class JSlider

#endif // __JSLIDER_H__


