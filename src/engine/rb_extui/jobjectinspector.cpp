/***********************************************************************************/
//  File:   JObjectInspector.cpp
//  Date:   15.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JLabel.h"
#include "JButton.h"
#include "JCheckBox.h"
#include "JPropertyEditor.h"
#include "JSignalServer.h"
#include "JObjectInspector.h"

/***********************************************************************************/
/*  JObjectInspector implementation
/***********************************************************************************/
decl_class(JObjectInspector);
JObjectInspector::JObjectInspector()
{
    m_pObject           = NULL;
    m_LeftColWidth      = 80;
    m_RightColWidth     = 100;
    m_DefaultRowHeight  = 16;
    m_FontHeight        = 12;
    m_EvenRowColor      = 0xAAFFFFFF;
    m_OddRowColor       = 0x55333333;
    m_LabelsColor       = 0xFFFFFFFF;
    m_SelectionColor    = 0xFF0000FF;
    m_SelectedIdx       = -1;

    m_pEditorsFactory   = NULL;
    m_pDynamicWidgets   = NULL;
} // JObjectInspector::JObjectInspector

void JObjectInspector::Init()
{
    if (!m_pEditorsFactory)
    {
        m_pEditorsFactory = g_pObjectServer->FindObject<JWidget>( "editors", this );
        if (!m_pEditorsFactory) 
        {
            m_pEditorsFactory = new JWidget();
            m_pEditorsFactory->SetName( "editors" );
            AddChild( m_pEditorsFactory );
        }
    }
    if (!m_pDynamicWidgets) 
    {
        m_pDynamicWidgets = g_pObjectServer->FindObject<JWidget>( "dynamicwidgets", this );
        if (!m_pDynamicWidgets) 
        {
            m_pDynamicWidgets = new JWidget();
            m_pDynamicWidgets->SetName( "dynamicwidgets" );
            AddChild( m_pDynamicWidgets );
        }
    }
} // JObjectInspector::Init

void JObjectInspector::Render()
{
} // JObjectInspector::Render

void JObjectInspector::PostRender()
{
    
} // JObjectInspector::PostRender

void JObjectInspector::OnMouse( JMouseEvent& e )
{
    if (!m_pDynamicWidgets) return;
    if (e.Action() == aKeyDown && e.MouseKey() == mkLeft)
    {
        int nCh = m_pDynamicWidgets->GetNChildren();
        Vec2 pt( e.MouseX(), e.MouseY() );
        for (int i = 0; i < nCh; i++)
        {
            JWidget* pChild = (JWidget*)m_pDynamicWidgets->GetChild( i );
            if (pChild->PtIn( pt.x, pt.y ))
            {
                m_SelectedIdx = i;
            }
        }
    }
} // JObjectInspector::OnMouse

JWidget* JObjectInspector::CreateBlock( JMetaClass* pMeta )
{
    JWidget* pBlockTemplate = g_pObjectServer->FindObject<JWidget>( "block_template", this );
    if (!pMeta || !pBlockTemplate) return NULL;
    int nAttr = pMeta->GetNOwnAttr();
    if (nAttr == 0)
    {
        return NULL;
    }

    JWidget* pBlock = (JWidget*)pBlockTemplate->Clone( m_pDynamicWidgets, pMeta->GetName(), true );
    if (!pBlock) return NULL;
    
    JLabel* pCaption = g_pObjectServer->FindObject<JLabel>( "classname", pBlock );
    if (pCaption)
    {
        pCaption->SetText( pMeta->GetName() );
    }
    
    JWidget* pControls = g_pObjectServer->FindObject<JWidget>( "controls", pBlock );
    if (!pControls)
    {
        return pBlock;
    }

    Frame cExt = pControls->GetLocalExt();
    float cY = 0.0f;
    int curRow = 0;
    for (int i = 0; i < nAttr; i++)
    {
        JClassAttr* pAttr = pMeta->GetAttr( i );
        const char* typeName = pAttr->GetType();
        char name[_MAX_PATH];
        sprintf( name, "%s_%s", pAttr->GetName(), pAttr->GetType() );
        JWidget* pEditor = CreateEditor( typeName, name );
        if (!pEditor) continue;

        JLabel* pCaption = new JLabel();
        pControls->AddChild( pCaption );
        pControls->AddChild( pEditor );
        float rowH = pEditor->GetExt().h;
        pCaption->SetText       ( pAttr->GetName() );
        pCaption->SetFont       ( GetFont() );
        pCaption->SetFontHeight ( m_FontHeight );
        pCaption->SetLocalExt   ( Frame( 0.0f, cY, m_LeftColWidth + m_RightColWidth, rowH ) );
        pCaption->SetBgColor    ( curRow&1 ? m_EvenRowColor : m_OddRowColor );
        pCaption->SetFgColor    ( m_LabelsColor );
        pCaption->SetName       ( pAttr->GetName() );

        pEditor->SetTag         ( i, true );
        pEditor->SetLocalExt    ( Frame( m_LeftColWidth, cY, m_RightColWidth, rowH ) );

        if (!strnicmp( "enum", pAttr->GetType(), strlen( "enum" )))
        {
            JPropertyEditor* pPropEditor = obj_cast<JPropertyEditor>( pEditor );
            static JStringList lst;
            pAttr->Enumerate( lst );
            pPropEditor->SetEnumList( lst );
            m_pObject->SendSignal( pAttr->GetName() );
        }
        JSignalServer::s_pInstance->Connect( m_pObject, pAttr->GetName(), pEditor, "outval" );
        JSignalServer::s_pInstance->Connect( pEditor, "outval", m_pObject, pAttr->GetName() );
        cY += rowH;
        curRow++;
    }
    
    pControls->SetLocalExt( Frame( cExt.x, cExt.y, m_LeftColWidth + m_RightColWidth, cY ) );
    Frame blockExt = pBlock->GetLocalExt();
    pBlock->SetLocalExt( Frame( 0.0f, 0.0f, blockExt.w, cY + blockExt.h ) ); 
    pBlock->SetVisible( true );
    return pBlock;
}

void JObjectInspector::SetObject( JObject* pObj )
{
    if (!m_pDynamicWidgets) return;
    if (m_pObject == pObj) return;

    //  check if setting one of the children
    for (int i = 0; i < m_pDynamicWidgets->GetNChildren(); i++)
    {
        if (pObj == m_pDynamicWidgets->GetChild( i )) return;
    }

    m_pObject = pObj;

    m_pDynamicWidgets->RemoveChildren();

    if (!pObj) return;
    JMetaClass* pMeta = g_pObjectServer->FindClass( pObj->ClassName() );
    while (pMeta)
    {
        JWidget* pBlock = CreateBlock( pMeta );
        pMeta = pMeta->GetParent();
        if (!pBlock) continue;
        m_pDynamicWidgets->AddChild( pBlock, 0 );
        float h = pBlock->GetLocalExt().h;
        float w = pBlock->GetLocalExt().w;
    }
    
    float cY = 0.0f;
    int nBlocks = m_pDynamicWidgets->GetNChildren(); 
    for (int i = 0; i < nBlocks; i++)
    {
        JWidget* pBlock = obj_cast<JWidget>( m_pDynamicWidgets->GetChild( i ) );
        if (!pBlock) continue;
        Frame ext = pBlock->GetLocalExt();
        ext.y = cY;
        pBlock->SetLocalExt( ext );
        cY += pBlock->GetLocalExt().h;
    }

    Frame ext = GetExt();
    ext.h = cY;
    ext.w = m_LeftColWidth + m_RightColWidth;
    SetExt( ext );
} // JObjectInspector::SetObject

JWidget* JObjectInspector::CreateEditor( const char* className, const char* editorName )
{
    JWidget* pEditor = NULL;
    if (m_pEditorsFactory) 
    {
        int nEdTypes = m_pEditorsFactory->GetNChildren();
        for (int i = 0; i < nEdTypes; i++)
        {
            JObject* pEdType = m_pEditorsFactory->GetChild( i );
            if (!stricmp( className, pEdType->GetName() ))
            {
                pEditor = (JWidget*)pEdType->Clone( m_pDynamicWidgets, editorName, true );
                break;
            }
        }
    }

    if (!pEditor) return NULL;
    pEditor->SetExt( Frame( 0.0f, 0.0f, m_RightColWidth, m_DefaultRowHeight ) );
    pEditor->SetFont( GetFont() );
    return pEditor;
} // JObjectInspector::CreateEditor


