//****************************************************************************/
//  File:   JSwayPicture.h
//  Date:   28.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JSWAYPICTURE_H__
#define __JSWAYPICTURE_H__

//****************************************************************************/
//  Class:  JSwayPicture
//  Desc:   Picture control, which swaying distortion
//****************************************************************************/
class JSwayPicture : public JPicture
{
    Vec2                    m_Pivot;
    Vec2                    m_Axis;
    float                   m_Frequency;
    float                   m_Amplitude;
    float                   m_Fade;
    float                   m_FadeBias;
    Timer                   m_Timer;
    std::vector<VertexS>    m_RippleVert;

public:
                            JSwayPicture    ();
    virtual void            Render          ();
    virtual void            PostRender      ();
    virtual void            Init            ();
    virtual void            DrawBounds      ();

    expose( JSwayPicture )
    {
        parent( JPicture );
        field( "Pivot",         m_Pivot         );
        field( "Axis",          m_Axis          );
        field( "Frequency",     m_Frequency     );
        field( "Amplitude",     m_Amplitude     );
        field( "Fade",          m_Fade          );
        field( "FadeBias",      m_FadeBias      );
    }

protected:
    static void RippleCallback( void* pVert, int nVert, VertexType vType );
    static JSwayPicture* s_pThis;

    void        ProcessVert( VertexS* pVert, int nVert );
}; // class JSwayPicture


#endif // __JSWAYPICTURE_H__


