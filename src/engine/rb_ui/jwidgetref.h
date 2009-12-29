//****************************************************************************/
//  File:   JWidgetRef.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JWIDGETREF_H__
#define __JWIDGETREF_H__

//****************************************************************************/
//  Class:  JWidgetRef
//  Desc:    
//****************************************************************************/
class JWidgetRef : public JWidget
{
public:
                            JWidgetRef      ();
    virtual void            Render          ();
    virtual void            Init            ();

    virtual void            OnMouse         ( JMouseEvent& e );
    virtual void            OnWheel         ( JWheelEvent& e );
    virtual void            OnKey           ( JKeyEvent&   e );

    const char*             GetWidget       () const { return m_Widget.c_str(); }
    void                    SetWidget       ( const char* name );

    expose( JWidgetRef )
    {
        parent(JWidget);
        prop( "Widget", GetWidget, SetWidget );
    }
private:
    JString             m_Widget;
    JWeakRef<JWidget>        m_pWidget;
}; // class JWidgetRef

#endif // __JWIDGETREF_H__


