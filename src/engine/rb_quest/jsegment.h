/***********************************************************************************/
//  File:   JSegment.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSEGMENT_H__
#define __JSEGMENT_H__

/***********************************************************************************/
//  Class:  JSegment
//  Desc:   
/***********************************************************************************/
class JSegment : public JWidget
{
    Vec3            m_Beg;
    Vec3            m_End;
    float           m_Width;
    float           m_ClickWidth;
    DWORD           m_Color;

public:
                    JSegment        ();
    virtual void    Render          ();
    virtual bool    PtIn            ( int px, int py ) const;

    float           GetBegX         () const { return m_Beg.x; }
    float           GetBegY         () const { return m_Beg.y; }
    float           GetBegZ         () const { return m_Beg.z; }
    void            SetBegX         ( float val ) { m_Beg.x = val; }
    void            SetBegY         ( float val ) { m_Beg.y = val; }
    void            SetBegZ         ( float val ) { m_Beg.z = val; }

    float           GetEndX         () const { return m_End.x; }
    float           GetEndY         () const { return m_End.y; }
    float           GetEndZ         () const { return m_End.z; }
    void            SetEndX         ( float val ) { m_End.x = val; }
    void            SetEndY         ( float val ) { m_End.y = val; }
    void            SetEndZ         ( float val ) { m_End.z = val; }

    expose(JSegment)
    {
        parent(JWidget);
        field( "Color",         m_Color            );
        field( "Width",         m_Width            );
        field( "ClickWidth",    m_ClickWidth       );
        field( "Beg",           m_Beg              );
        field( "End",           m_End              );
        prop ( "BegX",          GetBegX,  SetBegX  );
        prop ( "BegY",          GetBegY,  SetBegY  );
        prop ( "BegZ",          GetBegZ,  SetBegZ  );
        prop ( "EndX",          GetEndX,  SetEndX  );
        prop ( "EndY",          GetEndY,  SetEndY  );
        prop ( "EndZ",          GetEndZ,  SetEndZ  );
    }                

private:
}; // class JSegment                          

#endif //__JSEGMENT_H__