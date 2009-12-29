//****************************************************************************/
//  File:   JRipplePicture.h
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JRIPPLEPICTURE_H__
#define __JRIPPLEPICTURE_H__

//****************************************************************************/
//  Class:  JRipplePicture
//  Desc:   Picture control, which features rippling distortion
//****************************************************************************/
class JRipplePicture : public JPicture
{
    Vec2                    m_RippleAxis;
    float                   m_Frequency;
    float                   m_Amplitude;
    float                   m_Fade;
    float                   m_FadeBias;
    Timer                   m_Timer;

    bool                    m_bOffscreen;
    bool                    m_bTintChildren;
    float                   m_CellSide;
    int                     m_OffscreenRT;
    std::vector<VertexS>    m_RippleVert;
    int                     m_OldRT;

    

public:
                    JRipplePicture  ();
    virtual void    Render          ();
    virtual void    PostRender      ();
    virtual void    Init            ();

    expose( JRipplePicture )
    {
        parent( JPicture );
        field( "RippleAxis",    m_RippleAxis    );
        field( "Frequency",     m_Frequency     );
        field( "Amplitude",     m_Amplitude     );
        field( "Fade",          m_Fade          );
        field( "FadeBias",      m_FadeBias      );
        field( "Offscreen",     m_bOffscreen    );
        field( "TintChildren",  m_bTintChildren );
        field( "CellSide",      m_CellSide      );
    }

protected:
    static void RippleCallback( void* pVert, int nVert, VertexType vType );
    static JRipplePicture* s_pThis;

    void        ProcessVert( VertexS* pVert, int nVert );
}; // class JRipplePicture


#endif // __JRIPPLEPICTURE_H__


