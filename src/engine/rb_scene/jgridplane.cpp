/***********************************************************************************/
//  File:   JGridPlane.cpp
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "JGridPlane.h"
#include "Mat4.h"

/***********************************************************************************/
/*  JGridPlane implementation
/***********************************************************************************/
decl_class(JGridPlane);
JGridPlane::JGridPlane()
{
    m_LinesColor    = 0xFF666666;
    m_RulersColor   = 0xFF444444;
    m_XColor        = 0xFFFF0000;
    m_YColor        = 0xFF00FF00;
    m_AxisLineWidth = 2;
    m_CellSide      = 100.0f;
    m_CellsW        = 24;
    m_CellsH        = 24;

    m_Center        = Vec3::null;
    m_Normal        = Vec3::oZ;
} // JGridPlane::JGridPlane

void JGridPlane::Render()
{
    //  create basis
    Vec3 oZ( m_Normal );
    Vec3 oX( Vec3::oX );
    Vec3 oY;
    oY.cross( oZ, oX );
    if (oY.norm() < c_FltEpsilon)
    {
        oX = Vec3::oY;
        oY.cross( oZ, oX );
        if (oY.norm() < c_FltEpsilon)
        {
            oX = Vec3::oZ;
            oY.cross( oZ, oX );
        }
    }

    Mat4 tm( oX, oY, oZ, m_Center );
    g_pDrawServer->Flush();
    g_pDrawServer->SetZEnable();
    g_pDrawServer->SetWorldTM( tm );
    
    float maxx = m_CellSide*m_CellsW*0.5f;
    float minx = -maxx; 
    float maxy = m_CellSide*m_CellsH*0.5f;
    float miny = -maxy; 
    
    for (float x = minx; x <= maxx; x += m_CellSide)
    {
        if (_fabs( x ) < c_FltEpsilon) continue;
        g_pDrawServer->DrawLine( Vec3( x, miny, 0.0f ), Vec3( x, maxy, 0.0f ), m_LinesColor, m_LinesColor );
    }

    for (float y = miny; y <= maxy; y += m_CellSide)
    {
        if (_fabs( y ) < c_FltEpsilon) continue;
        g_pDrawServer->DrawLine( Vec3( minx, y, 0.0f ), Vec3( maxx, y, 0.0f ), m_LinesColor, m_LinesColor );
    }

    g_pDrawServer->DrawLine( Vec3( minx, 0.0f, 0.0f ), Vec3( maxx, 0.0f, 0.0f ), m_XColor, m_XColor );
    g_pDrawServer->DrawLine( Vec3( 0.0f, miny, 0.0f ), Vec3( 0.0f, maxy, 0.0f ), m_YColor, m_YColor );


    g_pDrawServer->Flush();
} // JGridPlane::Render