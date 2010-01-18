//****************************************************************************/
//  File:   JPath.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPATH_H__
#define __JPATH_H__

//****************************************************************************/
//  Class:  JPath
//  Desc:   Picewise-linear path
//****************************************************************************/
class JPath : public JObject
{
public: 
                    JPath         ();
    
    float           GetMarkerT    () const;
    void            SetMarkerT    ( float val );
    Vec2            GetMarkerPos  () const;
    void            SetMarkerPos  ( Vec2 pos );
    Vec2            GetMarkerDir  () const;
    float           GetLength     () const;

    float           GetMarkerPosX () const { return m_MarkerPos.x; }
    float           GetMarkerPosY () const { return m_MarkerPos.y; }

    PolyLine2       GetPath       () const;
    void            SetPath       ( PolyLine2 path );

    virtual void    Render        ();

    expose( JPath )
    {
        parent(JObject);
        prop ( "Path",          GetPath, SetPath );
        field( "Smooth",        m_bSmooth );
        prop ( "MarkerT",       GetMarkerT, SetMarkerT );
        prop ( "MarkerPos",     GetMarkerPos, SetMarkerPos );
        rprop( "MarkerDir",     GetMarkerDir );
        rprop( "Length:d",      GetLength );
        rprop( "MarkerPosX:d",  GetMarkerPosX );
        rprop( "MarkerPosY:d",  GetMarkerPosY );
        field( "LineWidth",     m_LineWidth );
        field( "Color",         m_Color );
    }

private:
    PolyLine2       m_Path;
    bool            m_bSmooth;
    
    float           m_MarkerT;
    Vec2            m_MarkerPos;
    Vec2            m_MarkerDir;

    float           m_LineWidth;
    uint32_t           m_Color;
}; // class JPath

inline float JPath::GetMarkerT() const 
{ 
    return m_MarkerT; 
}

inline Vec2 JPath::GetMarkerPos() const
{
    return m_MarkerPos;
}

inline Vec2 JPath::GetMarkerDir() const
{
    return m_MarkerDir;
}

inline PolyLine2 JPath::GetPath() const
{
    return m_Path;
}    

#endif // __JPATH_H__


