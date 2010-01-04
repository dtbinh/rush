//****************************************************************************/
//  File:  Frame.cpp
//  Desc:  2D rectangle
//****************************************************************************/
#include "precompile.h"
#include "frame.h"

//****************************************************************************/
/*  Frame implementation
//****************************************************************************/
Frame Frame::null = Frame( 0.0f, 0.0f, 0.0f, 0.0f );
float Frame::dist( const Frame& rc ) const
{
    const Vec2 pa( x, y );
    const Vec2 pb( r(), y );
    const Vec2 pc( x, b() );
    const Vec2 pd( pb.x, pc.y );

    const Vec2 qa( rc.x, rc.y );
    const Vec2 qb( rc.r(), rc.y );
    const Vec2 qc( rc.x, rc.b() );
    const Vec2 qd( qb.x, qc.y );

    if (qa.x >= pb.x)
    {
        if (qd.y <= pa.y) return qc.dist( pb );
        if (qa.y >= pd.y) return qa.dist( pd );
        return qa.x - pb.x;
    }
    else if (qb.x <= pa.x)
    {
        if (qd.y <= pa.y) return qd.dist( pa );
        if (qa.y >= pd.y) return qb.dist( pc );
        return pa.x - qb.x;
    }
    else if (qa.y >= pd.y)
    {
        return qa.y - pd.y;
    }
    else if (qd.y <= pa.y)
    {
        return pa.y - qd.y;
    }
    return 0.0f;
} // dist

void Frame::Clip( const Frame& rct )
{
    int x1 = r();
    int y1 = b();

    if (x < rct.x) x = rct.x;
    if (y < rct.y) y = rct.y;
    if (x1 > rct.r()) x1 = rct.r();
    if (y1 > rct.b()) y1 = rct.b();

    w = x1 - x;
    h = y1 - y;

    if (w < 0.0f) w = 0.0f;
    if (h < 0.0f) h = 0.0f;
} // Frame::Clip
