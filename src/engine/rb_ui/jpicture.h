//****************************************************************************/
//  File:   JPicture.h
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JPICTURE_H__
#define __JPICTURE_H__

//****************************************************************************/
//  Class:  JPicture
//  Desc:   Draws static picture from the sprite package
//****************************************************************************/
class JPicture : public JWidget
{
public:
                    JPicture        ();
    virtual void    Render          ();
    virtual void    Init            ();
    virtual void    DrawBounds      ();

    bool            IsTiled         () const { return m_bTile; }
    bool            IsStretched     () const { return m_bStretch; }
    virtual bool    PtIn            ( int px, int py ) const;

    expose( JPicture )
    {
        parent( JWidget );
        field ( "Tile",         m_bTile         );
        field ( "Stretch",      m_bStretch      );
        field ( "Mirror",       m_bMirror       );
        field ( "Rotation",     m_Rotation      );
        field ( "ZEnable",      m_bZEnable      );
        field ( "AutoSize",     m_bAutoSize     );
        field ( "SpritePivot",  m_bSpritePivot  );
        field ( "PixelPrecise", m_bPixelPrecise );
    }

private:
    bool            m_bTile;        //  the picture is tiled
    bool            m_bStretch;     //  picture is stretched to fit to widget extents
    bool            m_bZEnable;
    bool            m_bMirror;
    bool            m_bAutoSize;
    bool            m_bSpritePivot;
    bool            m_bPixelPrecise;
    float           m_Rotation;
}; // class JPicture


#endif // __JPICTURE_H__


