//****************************************************************************/
//  File:   JGrid.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "JGrid.h"

//****************************************************************************/
/*  JGrid implementation
//****************************************************************************/
decl_class(JGrid);
JGrid::JGrid()
{
    m_NCols         = 1;
    m_NRows         = 0;

    m_HSpacing      = 0;
    m_VSpacing      = 0;
    
    m_MarginL       = 0;
    m_MarginR       = 0;
    m_MarginT       = 0;
    m_MarginB       = 0;

    m_bStretch      = false;
    m_bUniform      = false;
}  

void JGrid::SetRowsCols( int nRows, int nCols )
{
    m_NRows = nRows;
    m_NCols = nCols;
    Layout();
}

void JGrid::SetSpacing( int width, int height )
{
    m_HSpacing = width;
    m_VSpacing = height;
    Layout();
}

void JGrid::SetMargins( int left, int right, int top, int bottom )
{
    m_MarginL   = left;
    m_MarginR   = right;
    m_MarginT   = top;
    m_MarginB   = bottom;
    Layout();
}

void JGrid::Render()
{    
    
}  

void JGrid::OnSize()
{
    Layout();    
}  


void JGrid::Layout()
{
    int nCh = GetNChildren();
    if (nCh == 0 || (m_NCols == 0 && m_NRows == 0))
    {
        return;
    }   

    Frame ext = GetExt();
    int nCols = (m_NCols == 0) ? nCh : m_NCols;
    int nRows = (m_NRows == 0) ? nCh : m_NRows;

    float cellW = (ext.w - float( m_MarginL + m_MarginR ) - 
        float( nCols - 1 )*m_HSpacing)/float( nCols );
    float cellH = (ext.h - float( m_MarginT + m_MarginB ) - 
        float( nRows - 1 )*m_VSpacing)/float( nRows );

    float cX = m_MarginL;
    float cY = m_MarginT;
    int curCh = 0;
    for (int i = 0; i < nRows; i++)
    {
        float rowH = 0.0f;
        cX = m_MarginL;
        for (int j = 0; j < nCols; j++)
        {
            JWidget* pCh = obj_cast<JWidget>( GetChild( curCh ) );
            curCh++;
            if (pCh == NULL) 
            {
                continue;
            }
            Frame chExt = pCh->GetLocalExt();
            chExt.x = cX;
            chExt.y = cY;

            if (m_bStretch)
            {
                chExt.w = cellW;
                chExt.h = cellH;
            }
            pCh->SetLocalExt( chExt );

            rowH = tmax( rowH, chExt.h );
            if (m_bUniform)
            {
                cX += cellW;
            }
            else
            {
                cX += chExt.w;
            }
            cX += m_HSpacing;
        }
        if (m_bUniform)
        {
            cY += cellH;
        }
        else
        {
            cY += rowH;
        }
        cY += m_VSpacing;
    }
}



