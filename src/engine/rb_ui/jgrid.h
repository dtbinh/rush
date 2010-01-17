//****************************************************************************/
//  File:   JGrid.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JGRID_H__
#define __JGRID_H__

#include "jwidget.h"

//****************************************************************************/
//  Class:  JGrid
//  Desc:   Arranges children widgets in grid
//****************************************************************************/
class JGrid : public JWidget
{
public:
                    JGrid           ();
    virtual void    Render          ();
    virtual void    OnSize          ();

    void            SetRowsCols     ( int nRows, int nCols );
    void            SetSpacing      ( int width, int height );
    void            SetMargins      ( int left, int right, int top, int bottom );

    void            Layout          ();

    expose( JGrid )
    {
        parent(JWidget);
        field( "NRows",        m_NRows );
        field( "NCols",        m_NCols );
        field( "HSpacing",     m_HSpacing );
        field( "VSpacing",     m_VSpacing );
        field( "MarginL",      m_MarginL );
        field( "MarginR",      m_MarginR );
        field( "MarginT",      m_MarginT );
        field( "MarginB",      m_MarginB );
        field( "Stretch",      m_bStretch );
        field( "Uniform",      m_bUniform );
    }

private:
    int             m_NRows;
    int             m_NCols;

    int             m_HSpacing;
    int             m_VSpacing;

    int             m_MarginL;
    int             m_MarginR;
    int             m_MarginT;
    int             m_MarginB;

    bool            m_bStretch; //  stretches children to fit the cells
    bool            m_bUniform; //  cells have equal sizes
}; // class JGrid

#endif // __JGRID_H__


