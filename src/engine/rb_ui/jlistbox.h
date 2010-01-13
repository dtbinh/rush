/***********************************************************************************/
//  File:   JListBox.h
//  Date:   22.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JLISTBOX_H__
#define __JLISTBOX_H__

#include "JWeakRef.h"
#include "JWidget.h"

/***********************************************************************************/
//  Class:  JListBox
//  Desc:   Generic widget container, which arranges them horizontally or vertically
/*              and allows to select one
/***********************************************************************************/
class JListBox : public JWidget
{
    float               m_MaxWidth;         
    float               m_MaxHeight;        
    bool                m_bVertical;        
    bool                m_bSelectable;      //  whether user can select items
    bool                m_bSelectOnHover;   //  when true item is selected when user hovers mouse
    int                 m_SelectedItem;     //  currently item beibg selected, -1 when none

    JWeakRef<JWidget>   m_pTemplate;        //  item prototype widget
    JString             m_TemplateName;
    uint32_t               m_SelBgColor;       //  background color of selected item
    uint32_t               m_SelFgColor;       //  text color of selected item

    bool                m_bAutoSize;

public:
                        JListBox            ();
    virtual             ~JListBox           ();

    virtual void        Render              ();
    virtual void        Init                ();

    JWidget*            GetTemplate         ();
    int                 GetSelectedItem     () const { return m_SelectedItem; }
    void                SelectItem          ( int idx );
    int                 GetNItems           () const;
    void                SetNItems           ( int val );

    const char*         GetTemplateName     () const { return m_TemplateName.c_str(); }
    void                SetTemplateName     ( const char* name );

    float               GetMaxHeight        () const { return m_MaxHeight; }
    float               GetMaxWidth         () const { return m_MaxWidth; }
    void                SetMaxHeight        ( float val ) { m_MaxHeight = val; }
    void                SetMaxWidth         ( float val ) { m_MaxWidth  = val; }

    virtual Frame       CalcItemRect        ( int idx ) const;
    virtual void        OnMouse             ( JMouseEvent& m );
    virtual void        OnSize              ();
    JStringList         GetStrings          () const;
    void                SetStrings          ( JStringList strings );
    virtual void        OnChangeSelection   () {}

    expose(JListBox)
    {
        parent  (JWidget);
        field   ( "MaxWidth",           m_MaxWidth  );
        field   ( "MaxHeight",          m_MaxHeight );
        field   ( "Vertical",           m_bVertical );
        prop    ( "NumItems",           GetNItems, SetNItems );
        prop    ( "SelectedItem",       GetSelectedItem, SelectItem );
        method  ( "ChangeSelection",    OnChangeSelection );
        prop    ( "Strings",            GetStrings, SetStrings );
        field   ( "SelBgColor",         m_SelBgColor );
        field   ( "SelFgColor",         m_SelFgColor );
        field   ( "Selectable",         m_bSelectable );
        field   ( "SelectOnHover",      m_bSelectOnHover );
        field   ( "AutoSize",           m_bAutoSize );
        prop    ( "Template",           GetTemplateName, SetTemplateName );
    }

protected:
    void            SelectItemAt    ( int mx, int my ); 
}; // class JListBox

#endif //__JLISTBOX_H__