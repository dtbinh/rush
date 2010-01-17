//****************************************************************************/
//  File:  JWidgetRef.cpp
//****************************************************************************/
#include "precompile.h"
#include "iwindowserver.h"
#include "jwidgetref.h"

//****************************************************************************/
/*  JWidgetRef implementation
//****************************************************************************/
decl_class(JWidgetRef);
JWidgetRef::JWidgetRef() : m_pWidget( NULL )
{
    SetXAlign( XAlign_Parent );
    SetYAlign( YAlign_Parent );
}

void JWidgetRef::Render()
{
    if (!m_pWidget)
    {
        m_pWidget = g_pObjectServer->FindObject<JWidget>( m_Widget.c_str(), this );
    }
    else
    {
        m_pWidget->RenderTree();
    }
} // JWidgetRef::Render

void JWidgetRef::OnMouse( JMouseEvent& e )
{
    if (!m_pWidget || !m_pWidget->IsVisible()) return;
    m_pWidget->SendMouseEvent( e, g_pWindowServer->GetDragEvent() );
}

void JWidgetRef::OnWheel( JWheelEvent& e )
{
    if (!m_pWidget || !m_pWidget->IsVisible()) return;
    m_pWidget->SendWheelEvent( e );
} // JWidgetRef::OnWheel

void JWidgetRef::OnKey( JKeyEvent& e )
{
    if (!m_pWidget || !m_pWidget->IsVisible()) return;
    m_pWidget->SendKeyEvent( e );
} // JWidgetRef::OnKey

void JWidgetRef::Init()
{
    SetWidget( m_Widget.c_str() );
} // JWidgetRef::Init

void JWidgetRef::SetWidget( const char* name )
{
    m_Widget = name;
    m_pWidget = obj_cast<JWidget>( g_pObjectServer->FindObject<JWidget>( name, this ) );
} // JWidgetRef::SetObject
