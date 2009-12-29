//****************************************************************************/
//  File:   JTextureView.h
//  Date:   24.10.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JTEXTUREVIEW_H__
#define __JTEXTUREVIEW_H__

#include "JWidget.h"

//****************************************************************************/
//  Class:  JTextureView
//  Desc:   Texture thumbnail widget
//****************************************************************************/
class JTextureView : public JWidget
{
public:
                    JTextureView        ();
    virtual void    Render              ();
    
    const char*     GetTexName          () const { return m_TexName.c_str(); }
    void            SetTexName          ( const char* name );

    int             GetTexID            () const { return m_TexID; }
    void            SetTexID            ( int id );

    expose( JTextureView )
    {
        parent( JWidget );
        prop  ( "texname", GetTexName, SetTexName );
        prop  ( "texID",   GetTexID,   SetTexID   );
    }

private:
    int            m_TexID;
    JString        m_TexName;

}; // class JTextureView

#endif // __JTEXTUREVIEW_H__


