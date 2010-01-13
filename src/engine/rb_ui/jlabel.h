//****************************************************************************/
//  File:   JLabel.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JLABEL_H__
#define __JLABEL_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JLabel
//  Desc:   Static text widget
//****************************************************************************/
class JLabel : public JWidget
{
public:
                    JLabel          ();
    virtual void    Render          (); 
    JXAlign         GetTextXAlign   () const { return m_TextXAlign; } 
    JYAlign         GetTextYAlign   () const { return m_TextYAlign; } 
    uint32_t           GetShadowColor  () const { return m_ShadowColor; }
    void            SetTextXAlign   ( JXAlign align ) { m_TextXAlign = align; }
    void            SetTextYAlign   ( JYAlign align ) { m_TextYAlign = align; }
    void            SetMarginY      ( int val ) { m_MarginY = val; }
    void            SetMarginX      ( int val ) { m_MarginX = val; }

    expose( JLabel )
    {
        parent(JWidget);
        field( "TextXAlign",    m_TextXAlign    );
        field( "TextYAlign",    m_TextYAlign    );
        field( "MarginX",       m_MarginX       );
        field( "MarginY",       m_MarginY       );
        field( "ShadowColor",   m_ShadowColor   );
        field( "ShadowShiftX",  m_ShadowShiftX  );
        field( "ShadowShiftY",  m_ShadowShiftY  );
        field( "WrapText",      m_bWrapText     );
        field( "RowsGap",       m_RowsGap       );
        field( "TemplateText",  m_TemplateText  );
        field( "Spacing",       m_Spacing       );
    }

private:
    JXAlign         m_TextXAlign;   //  in-widget text horizontal alignment
    JYAlign         m_TextYAlign;   //  in-widget text vertical alignment

    uint32_t           m_ShadowColor;  //  color of the text's shadow, when 0 - no shadow is drawn
    int             m_ShadowShiftX;
    int             m_ShadowShiftY;
    
    int             m_MarginX;      
    int             m_MarginY;      
    bool            m_bWrapText;
    int             m_RowsGap;
    int             m_Spacing;

    JString         m_TemplateText;

    void            DrawSubstring   ( const Frame& ext, const char* text, int nChar = -1 );
}; // class JLabel

#endif // __JLABEL_H__



