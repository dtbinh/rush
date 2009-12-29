//****************************************************************************/
//  File:   PSText.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSText_H__
#define __PSText_H__

//****************************************************************************/
//	Enum:  PSTextMode
//  Desc:  Rendering mode for text  
//****************************************************************************/
enum PSTextMode
{
    TextMode_String = 0,
    TextMode_Character = 1
}; // enum PSTextMode

enum_beg(PSTextMode)
    enum_val( "String",     TextMode_String ),
    enum_val( "Character",  TextMode_Character )
enum_end(PSTextMode)

//****************************************************************************/
//  Class:  PSText
//  Desc:   Renders particle as text string
//****************************************************************************/
class PSText : public PSOperator
{
    JString         m_String;       //  string to render as particle
    JString         m_Font;         //  font name to render with
    PSTextMode      m_Mode;         //  text rendering mode 
    DWORD           m_ShadowColor;  //  color of the shadow (if 0, no shadow rendered)
    Vec2            m_ShadowShift;  //  shift of the shadow relative to the main text
    bool            m_bFilter;      //  whether to filter text linearly
    bool            m_bAdditive;    //  whether to use additive blending
    bool            m_bStretch;     //  when true, text is stretched to adapt to the particle size

    int             m_FontID;

public: 
    PSText();

    virtual void Update( EmitterInstance* pInst, float dt, DWORD mode, ParticleServer* ps );
    virtual void Init();

    expose( PSText )
    {
        parent(PSOperator);
        field( "String",        m_String         );    
        field( "Font",          m_Font           );      
        field( "Mode",          m_Mode           );      
        field( "ShadowColor",   m_ShadowColor    );
        field( "ShadowShift",   m_ShadowShift    );
        field( "FilterFont",    m_bFilter        );   
        field( "Stretch",       m_bStretch       );  
        field( "Additive",      m_bAdditive      );
    }
}; // class PSText

#endif // __PSText_H__


