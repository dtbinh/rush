/***********************************************************************************/
//  File:   JGlyph.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JGLYPH_H__
#define __JGLYPH_H__

enum JGlyphDrag
{
    GlyphDrag_None   = 0,
    GlyphDrag_Normal = 1,
    GlyphDrag_Sticky = 2,
    GlyphDrag_Rotate = 3,
}; // JGlyphDrag

enum_beg(JGlyphDrag)
    enum_val( "None",   GlyphDrag_None   ),
    enum_val( "Normal", GlyphDrag_Normal ),
    enum_val( "Sticky", GlyphDrag_Sticky ),
    enum_val( "Rotate", GlyphDrag_Rotate )
enum_end(JGlyphDrag)

/***********************************************************************************/
//  Class:  JGlyph
//  Desc:   
/***********************************************************************************/
class JGlyph : public JWidget
{
    Vec3            m_Pos;
    float           m_Rotation;
    float           m_Scale;

    Mat3            m_Transform;

    bool            m_bPickChildren;    //  PtIn accounts for children pixels 
    bool            m_bMirrored;
    DWORD           m_Color;
    JGlyphDrag      m_DragMode;
    bool            m_bHideCursor;      //  whether to hide cursor while dragging

    bool            m_bDragging;
    Vec2            m_DragHotspot;
    Vec3            m_DragStartPos;

public:
                    JGlyph          ();
    virtual void    Render          ();
    virtual void    PostRender      ();
    virtual void    DrawBounds      ();
    virtual void    OnMouse         ( JMouseEvent& e );
    virtual bool    PtIn            ( int px, int py ) const;

    float           GetRotation     () const { return m_Rotation; }
    bool            IsMirrored      () const { return m_bMirrored; }
    void            SetRotation     ( float rot ) { m_Rotation = rot; }
    void            SetMirrored     ( bool bMirrored ) { m_bMirrored = bMirrored; }
    float           GetPosX         () const { return m_Pos.x; }
    float           GetPosY         () const { return m_Pos.y; }
    float           GetPosZ         () const { return m_Pos.z; }
    void            SetPosX         ( float val ) { m_Pos.x = val; }
    void            SetPosY         ( float val ) { m_Pos.y = val; }
    void            SetPosZ         ( float val ) { m_Pos.z = val; }

    void            OnStartDrag     () {}
    void            OnDrop          () {}

    expose(JGlyph)
    {
        parent(JWidget);
        field( "Scale",         m_Scale             );
        field( "Position",           m_Pos               );
        field( "Rotation",           m_Rotation          );
        field( "Mirrored",      m_bMirrored         );
        field( "PickChildren",  m_bPickChildren     );

        field( "Color",         m_Color             );
        field( "HideCursor",    m_bHideCursor       );
        field( "DragMode",      m_DragMode          );
        
        //  non-persistent
        prop ( "PosX",          GetPosX, SetPosX    );
        prop ( "PosY",          GetPosY, SetPosY    );
        prop ( "PosZ",          GetPosZ, SetPosZ    );
        field( "DragStartX",    m_DragStartPos.x    );
        field( "DragStartY",    m_DragStartPos.y    );
        
        //  events
        method( "OnStartDrag",  OnStartDrag         );
        method( "OnDrop",       OnDrop              );


        method( "StartDrag",    StartDrag           );
        method( "StopDrag",     StopDrag            );

    }                

private:
    void        StartDrag();
    void        StopDrag ();
    
}; // class JGlyph                          

#endif //__JGLYPH_H__