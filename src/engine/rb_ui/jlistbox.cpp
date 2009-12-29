/***********************************************************************************/
//  File:   JListBox.cpp
//  Date:   22.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JObjectIterator.h"
#include "JLabel.h"
#include "JListBox.h"

/***********************************************************************************/
/*  JListBox implementation
/***********************************************************************************/
decl_class(JListBox);
JListBox::JListBox()
{
    m_MaxWidth          = 0;
    m_MaxHeight         = 0;
    m_bVertical         = true;
    m_SelectedItem      = -1;
    m_SelBgColor        = 0x552222FF;
    m_SelFgColor        = 0x00000000;
    m_bSelectable       = true;
    m_bSelectOnHover    = false;
    m_pTemplate         = NULL;
    m_TemplateName      = "Template";
    m_bAutoSize         = false;
} // JListBox::JListBox

JListBox::~JListBox()
{
    if (m_pTemplate)
    {
        m_pTemplate->Release();
    }
}

JStringList JListBox::GetStrings() const
{
    return JStringList();
} // JListBox::GetStrings

void JListBox::SetStrings( JStringList strings )
{
    SetNItems( strings.size() );
    
    int nCh = strings.size();
    for (int i = 0; i < nCh; i++)
    {
        JWidget* pChild = obj_cast<JWidget>( GetChild( i ) );
        if (!pChild) continue;
        pChild->SetText( strings[i].c_str() );
    }

    OnSize();
} // JListBox::SetStrings

void JListBox::OnSize()
{
    int nCh = GetNChildren();
    Frame ext = GetLocalExt();
    if (ext.w == 0.0f || ext.h == 0.0f) return;
    float cH = 0.0f;
    float cW = 0.0f;
    for (int i = 0; i < nCh; i++)
    {
        JWidget* pChild = obj_cast<JWidget>( GetChild( i ) );
        if (!pChild) continue;
        Frame cext = CalcItemRect( i );
        if (cext.w == 0) cext.w = ext.w;
        if (cext.h == 0) cext.h = ext.h;
        pChild->SetLocalExt( cext );
        cext.x += ext.x;
        cext.y += ext.y;
    }
    if (m_bAutoSize) FitToChildren();

} // JListBox::OnSize

JWidget* JListBox::GetTemplate()
{
    return m_pTemplate;
} // JListBox::GetTemplate

void JListBox::Render()
{
    Frame vp = g_pRenderServer->GetViewport();
    Frame ext = GetExt();
    if (IsClipToExt()) g_pRenderServer->SetViewport( ext );

    //  render all non-selected items first
    for (int i = 0; i < GetNChildren(); i++)
    {
        JWidget* pChild = obj_cast<JWidget>( GetChild( i ) );
        if (pChild && i != m_SelectedItem)
        {
            JObjectIterator it( pChild );
            while (it)
            {
                JWidget* pObj = obj_cast<JWidget>( *it );
                if (pObj && pObj != pChild && !pObj->IsVisible()) 
                { 
                    it.breadth_next();
                    continue;
                }
                if (ext.Intersects( pObj->GetExt() )) pObj->Render();
                ++it;
            }
        }
    }

    //  draw selection
    JWidget* pSelected = obj_cast<JWidget>( GetChild( m_SelectedItem ) );
    if (pSelected)
    {
        JObjectIterator it( pSelected );
        while (it)
        {
            JWidget* pObj = obj_cast<JWidget>( *it );
            if (pObj && pObj != pSelected && !pObj->IsVisible()) 
            { 
                it.breadth_next();
                continue;
            }
            if (ext.Intersects( pObj->GetExt() )) pObj->Render();
            ++it;
        }
        Frame ext = pSelected->GetExt();
        g_pDrawServer->Flush();
        g_pDrawServer->DrawRect( ext, m_SelBgColor );
        g_pDrawServer->Flush();
        if (m_SelFgColor != 0)
        {
            g_pDrawServer->DrawFrame( ext, m_SelFgColor );
            g_pDrawServer->Flush();
        }
    }

    if (IsClipToExt()) g_pRenderServer->SetViewport( vp );
} // JListBox::Render

void JListBox::SetTemplateName( const char* name )
{
    m_TemplateName = name;
    Init();
} // JListBox::SetTemplateName

void JListBox::Init()
{
    m_pTemplate = g_pObjectServer->FindObject<JWidget>( m_TemplateName.c_str(), this );
    if (!m_pTemplate) 
    {
        m_pTemplate = new JLabel();
        m_pTemplate->SetName( "Template" );
        m_pTemplate->SetText( "listbox item" );
        m_pTemplate->SetFont( GetFont() );
        m_pTemplate->SetFontHeight( GetFontHeight() );
        m_pTemplate->SetFgColor( GetFgColor() );

        Frame ext = GetLocalExt();
        ext.h = GetFontHeight();
        m_pTemplate->SetLocalExt( ext );
    }
    m_pTemplate->AddRef();
} // JListBox::Init

void JListBox::SetNItems( int val )
{
    if (val == GetNChildren()) return;

    if (!m_pTemplate) Init();
    if (!m_pTemplate) return;

    RemoveChildren();
    for (int i = 0; i < val; i++)
    {
        char buf[256];
        sprintf( buf, "Item%d", i );
        JWidget* pChild = obj_cast<JWidget>( m_pTemplate->Clone( this, buf, true ) );
        pChild->SetVisible( false );
        pChild->SetTag( i, true );
        AddChild( pChild );              
    }

    OnSize();
} // JListBox::SetNItems

int JListBox::GetNItems() const
{
    return GetNChildren();
} // JListBox::GetItems

void JListBox::SelectItemAt( int mx, int my ) 
{
    int selIdx = -1;
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JWidget* pChild = obj_cast<JWidget>( GetChild( i ) );
        if (!pChild) continue;
        if (pChild->PtIn( mx, my )) 
        {
            selIdx = i;
            break;
        }
    }

    if (selIdx < 0) return;

    JWidget* pSelected = obj_cast<JWidget>( GetChild( m_SelectedItem ) );
    m_SelectedItem = selIdx;
    pSelected = obj_cast<JWidget>( GetChild( m_SelectedItem ) );
} // JListBox::SelectItemAt

void JListBox::SelectItem( int idx )
{
    if (idx < 0 || idx >= GetNChildren()) return;
    m_SelectedItem = idx;
    SendSignal( "ChangeSelection" );
    SendSignal( "SelectedItem" );
} // JListBox::SelectItem

void JListBox::OnMouse( JMouseEvent& m )
{
    if (!m_bSelectable) return;
    if (m.Action() == aKeyDown && m.MouseKey() == meLeft)
    {
        SelectItemAt( m.MouseX(), m.MouseY() );
        m.Consume();
        SendSignal( "ChangeSelection" );
        SendSignal( "SelectedItem" );
        return;
    }

    if (m.Action() == aMouseMove && m_bSelectOnHover)
    {
        m.Consume();
        SelectItemAt( m.MouseX(), m.MouseY() );
        return;
    }
} // JListBox::OnMouse

Frame JListBox::CalcItemRect( int idx ) const
{
    JWidget* pChild = obj_cast<JWidget>( GetChild( idx ) );
    if (!pChild) return Frame::null;
    Frame ext = pChild->GetLocalExt();
    float cX = m_bVertical ? ext.x : 0.0f;
    float cY = m_bVertical ? 0.0f : ext.y;
    float cW = ext.w;
    float cH = ext.h;
    for (int i = 0; i < idx; i++)
    {
        JWidget* pCurChild = obj_cast<JWidget>( GetChild( i ) );
        if (!pChild) continue;
        Frame cext = pCurChild->GetLocalExt();
        if (m_bVertical) cY += cext.h; 
            else cX += cext.w;
    }

    return Frame( cX, cY, cW, cH );
} // JListBox::CalcItemRect
