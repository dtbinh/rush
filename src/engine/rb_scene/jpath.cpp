//****************************************************************************/
//  File:  JPath.cpp
//  Desc:
//****************************************************************************/
#include "precompile.h"
#include "jpath.h"

//****************************************************************************/
/*  JPath implementation
//****************************************************************************/
decl_class(JPath);
JPath::JPath()
{
    m_bSmooth   = false;
    m_MarkerT   = 0.0f;
    m_MarkerPos = Vec2::null;
    m_MarkerDir = Vec2::oY;

    m_LineWidth = 20.0f;
    m_Color = 0x33FFFF55;
} // JPath::JPath

float JPath::GetLength() const
{
    return m_Path.GetLength( false );
}

void JPath::SetMarkerT( float val )
{
    val = clamp( val, 0.0f, 1.0f );
    m_MarkerT   = val;
    Vec2 tangent;
    m_MarkerPos = m_Path.GetParamPoint( val, false, &tangent );
    m_MarkerDir.x = tangent.x;
    m_MarkerDir.y = tangent.y;
}

void JPath::SetMarkerPos( Vec2 pos )
{
    SetMarkerT( m_Path.GetPointParam( pos, false ) );
}

void JPath::SetPath( PolyLine2 path )
{
    m_Path = path;
    SetMarkerT( m_MarkerT );
}

void JPath::Render()
{
    g_pDrawServer->DrawPolyline( m_LineWidth, m_Path, m_Color, false, false );
}





