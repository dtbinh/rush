//****************************************************************************/
//  File:   JPolyButton.h
//  Date:   21.09.2004
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPOLYBUTTON_H__
#define __JPOLYBUTTON_H__

#include "jmathtypecast.h"
#include "jbutton.h"

//****************************************************************************/
//  Class:  JPolyButton
//  Desc:   Button with polygonal bound
//****************************************************************************/
class JPolyButton : public JButton
{
public:
                    JPolyButton ();
    virtual void    DrawBounds  ();
    virtual bool    PtIn        ( int px, int py ) const;

    PolyLine2       GetContour  () const;
    void            SetContour  ( PolyLine2 pt );

    expose( JPolyButton )
    {
        parent( JButton );
        field( "Contour", m_Contour );
    }

protected:
    PolyLine2                   m_Contour;
}; // class JPolyButton


#endif // __JPolyButton_H__


