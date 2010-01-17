/***********************************************************************************/
//  File:   JComboBox.cpp
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "jsignalserver.h"
#include "jbutton.h"
#include "jlistbox.h"
#include "jcombobox.h"

/***********************************************************************************/
/*  JComboBox implementation
/***********************************************************************************/
decl_class(JComboBox)
JComboBox::JComboBox()
{
    m_DropHeight    = 100.0f;
    m_pDropList     = NULL;
    m_pThumb        = NULL;
    m_LastSelection = -1;
    m_SelectedItem  = -1;
    SetFocusable( true );
}

int JComboBox::GetDropHeight() const
{
    return m_DropHeight;
}

void JComboBox::SetDropHeight( int val )
{
    m_DropHeight = val;
    if (m_pDropList)
    {
        m_pDropList->SetMaxHeight( val );
        Frame ext = m_pDropList->GetLocalExt();
        ext.h = val;
        m_pDropList->SetLocalExt( ext );
    }
}

bool JComboBox::IsDropVisible() const
{
    return (m_pDropList && m_pDropList->IsVisible());
}

void JComboBox::SetDropVisible( bool bVisible )
{
    if (m_pDropList) m_pDropList->SetVisible( bVisible );
}

void JComboBox::OnSize()
{
    if (m_pThumb)
    {
        Frame ext = GetLocalExt();
        int w = g_pDrawServer->GetFrameW( m_pThumb->GetSkinPackID(), m_pThumb->GetNormalFrame() );
        int h = g_pDrawServer->GetFrameH( m_pThumb->GetSkinPackID(), m_pThumb->GetNormalFrame() );
        if (w == 0) w = ext.h;
        if (h == 0) h = ext.h;
        ext.x = 0.0f;
        ext.y = 0.0f;
        ext.x = ext.r() - w;
        ext.w = w;
        ext.h = h;
        m_pThumb->SetLocalExt( ext );
    }

    if (m_pDropList)
    {
        Frame ext = GetLocalExt();
        Frame cext = m_pDropList->GetLocalExt();
        ext.x = 0.0f;
        ext.y = 0.0f;
        cext.y = ext.b();
        cext.w = ext.w;
        cext.h = m_DropHeight;
        m_pDropList->SetLocalExt( cext );
        m_pDropList->OnSize();
    }
}

void JComboBox::OnMouse( JMouseEvent& m )
{
    if (m.Action() == aKeyDown && m.MouseKey() == meLeft)
    {
        m_pThumb->Press();
        m.Consume();
        return;
    }

    if (m.Action() == aKeyUp && m.MouseKey() == meLeft && m_pThumb->IsPressed())
    {
        m_pThumb->Unpress();
        m.Consume();
        return;
    }
}

void JComboBox::Init()
{
    m_pDropList = g_pObjectServer->FindObject<JListBox>( "DropList", this );
    m_pThumb    = g_pObjectServer->FindObject<JButton>( "Thumb", this );

    if (!m_pDropList)
    {
        m_pDropList = new JListBox();
        m_pDropList->SetName( "DropList" );
        AddChild( m_pDropList );
    }

    if (!m_pThumb)
    {
        m_pThumb = new JButton();
        m_pThumb->SetName( "Thumb" );
        AddChild( m_pThumb );
    }

    JSignalServer::s_pInstance->Connect( m_pThumb,    "unpress",         this, "DropDown" );
    JSignalServer::s_pInstance->Connect( m_pDropList, "changeselection", this, "DropUp"   );

    m_pDropList->SetFont        ( GetFont()         );
    m_pDropList->SetFontHeight  ( GetFontHeight()   );
    m_pDropList->SetFgColor     ( GetFgColor()      );
    m_pDropList->SelectItem     ( m_SelectedItem    );

    OnSize();
}

void JComboBox::SelectItem( int idx )
{
    m_SelectedItem = idx;
    if (m_LastSelection == -1) m_LastSelection = idx;
    if (m_pDropList) m_pDropList->SelectItem( idx );
    SendSignal( "SelectedItem" );
}

void JComboBox::OnFocus( bool bEnter )
{
    if (!bEnter)
    {
        SetDropVisible( false );
        m_LastSelection = m_pDropList->GetSelectedItem();
    }
}

void JComboBox::DropDown()
{
    if (IsDropVisible())
    {
        DropUp();
        return;
    }
    SetDropVisible( true );
    m_LastSelection = m_pDropList->GetSelectedItem();
    SetFocus();
    m_pDropList->CaptureMouse();
}

void JComboBox::DropUp()
{
    SetDropVisible( false );
    m_LastSelection = m_pDropList->GetSelectedItem();
    SelectItem( m_LastSelection );
    SetFocus( false );
    m_pDropList->CaptureMouse( false );
}

void JComboBox::Render()
{
    if (!m_pDropList || !m_pThumb) return;

    Frame ext = GetExt();
    //  if dropped down, but mouse is outside - close
    if (IsDropVisible())
    {
        if (!HasFocus())
        {
            SelectItem( m_LastSelection );
            DropUp();
        }
    }
    else if (m_LastSelection != m_pDropList->GetSelectedItem())
    {
        SelectItem( m_LastSelection );
    }

    JWidget* pSelected = obj_cast<JWidget>( m_pDropList->GetChild( m_LastSelection ) );
    if (pSelected)
    {
        Frame oldExt = pSelected->GetLocalExt();
        Frame sExt( oldExt );
        sExt.y = 0.0f;
        pSelected->SetLocalExt( sExt );
        pSelected->Render();
        pSelected->SetLocalExt( oldExt );
    }

    g_pDrawServer->Flush();

    g_pDrawServer->DrawSprite( ext, GetSkinPackID(), GetSkinFrame() );
    g_pDrawServer->Flush();
}

