/***********************************************************************************/
//  File:   JObjectTree.cpp
//  Date:   14.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JDragEvent.h"

#include "SaveFileDialog.h"
#include "OpenFileDialog.h"
#include "FStream.h"
#include "Path.h"
#include "IWindowServer.h"
#include "IPersistServer.h"
#include "JObjectTree.h"

/***********************************************************************************/
/*  JObjectTree implementation
/***********************************************************************************/
decl_class(JObjectTree);
JObjectTree::JObjectTree()
{
    m_DefaultRootPos    = Vec2( 10, 400 );
    m_RootPos           = m_DefaultRootPos;
    m_NodeCaptionPos    = Vec2( 2.0f, 1.0f );

    m_pRoot             = NULL;
    m_pSelected         = NULL;
    m_pExpanded         = NULL;
    m_ConnectColor      = 0xBB2222FF;
    m_NodeHeight        = 15.0f;
    m_NodeWidth         = 90.0f;

    m_pDraggedNode      = NULL;

    m_bCanDrop          = false;
    m_DragShift         = Vec2::null;
    m_InsExt            = Frame::null;
    m_InsColor          = 0x88FF3333;
    
    SetLocalExt( Frame( 0.0f, 0.0f, 1600.0f, 1200.0f ) );
} // JObjectTree::JObjectTree

void JObjectTree::SetRootPath( const char* path )
{
    JObject* pRoot = g_pObjectServer->FindObject( path, NULL );
    if (pRoot) SetRoot( pRoot );
} // JObjectTree::SetRootPath

const char* JObjectTree::GetRootPath() const
{
    if (!m_pRoot) return "";
    return m_pRoot->GetName();
} // JObjectTree::GetRootPath

void JObjectTree::SetRoot( JObject* pRoot )
{
    m_pRoot = pRoot;
    CollapseTo( pRoot );
    SetRootPos( m_DefaultRootPos );
} // JObjectTree::SetRoot

void JObjectTree::Render()
{
    Traverse( &JObjectTree::DrawNodeCallback );

    //  draw dragged node
    if (m_pDraggedNode)
    {
        Vec2 mPos;
        g_pWindowServer->GetMousePos( mPos );
        Frame ext( mPos.x - m_DragShift.x, mPos.y - m_DragShift.y, m_NodeWidth, m_NodeHeight );
        RenderNode( m_pDraggedNode, ext, false );
        if (m_bCanDrop)
        {
            g_pDrawServer->DrawRect( m_InsExt, m_InsColor );
        }
    }
    g_pDrawServer->Flush();
} // JObjectTree::Render

void JObjectTree::SetRootPos( Vec2 pos )
{
    m_RootPos = pos;
}

void JObjectTree::CollapseTo( JObject* pObject )
{
    
} // JObjectTree::CollapseTo

void JObjectTree::RenderNode( JObject* pObject, const Frame& ext, bool bExpanded ) const
{
    static int spID = g_pDrawServer->GetSpriteID( "button" );
    int frameID = 11;
    int nCh = pObject->GetNChildren();
    if (nCh > 0)
    {
        frameID = 12;
        float connectW = 4.0f;
        if (bExpanded) 
        {
            frameID = 14;
            Frame rcLine;
            rcLine.x = ext.r() - 2.0f;
            rcLine.h = float( nCh - 1 )*m_NodeHeight;
            rcLine.w = 2.0f;
            rcLine.y = ext.y - rcLine.h*0.5f + m_NodeHeight*0.5f;
            g_pDrawServer->DrawRect( rcLine, m_ConnectColor );
            connectW = 10.0f;
        }

        Frame rcConnect;
        rcConnect.x = ext.r() - 10.0f;
        rcConnect.h = 8.0f;
        rcConnect.w = connectW;
        rcConnect.y = ext.y + 3.0f;
        g_pDrawServer->DrawRect( rcConnect, m_ConnectColor );
    }

    Vec2 mousePos;
    g_pWindowServer->GetMousePos( mousePos );
    bool bHovered = ext.PtIn( mousePos.x, mousePos.y );

    DWORD bgColor = GetBgColor();
    DWORD fgColor = GetFgColor();

    if (bHovered)
    {
        bgColor = GetHoverBgColor();
        fgColor = GetHoverFgColor();
    }

    g_pDrawServer->DrawSprite( ext.x, ext.y, spID, frameID, bgColor );
    if (m_pSelected == pObject) 
    {
        g_pDrawServer->DrawSprite( ext.x, ext.y, spID, 13, fgColor );
    }
    g_pDrawServer->Flush();
    float strW = g_pDrawServer->GetTextWidth(  GetFontID(), pObject->GetName() );
    Frame vp;
    Frame clipExt( ext );
    clipExt.w -= 10.0f;
    bool bClip = (strW > clipExt.w);
    if (bClip)
    {
        vp = g_pRenderServer->GetViewport();
        g_pRenderServer->SetViewport( clipExt );
    }
    g_pDrawServer->DrawString( ext.x + m_NodeCaptionPos.x, ext.y + m_NodeCaptionPos.y, 
                     GetFontID(), pObject->GetName(), GetFgColor() );
    g_pDrawServer->Flush();
    if (bClip)
    {
        g_pRenderServer->SetViewport( vp );
    }
} // JObjectTree::RenderNode

JObject* JObjectTree::PickNode( float mx, float my, Frame* pExt ) const
{
    PickContext context;
    context.m_PickPos = Vec2( mx, my );
    context.m_pPicked = NULL;
    Traverse( &JObjectTree::PickNodeCallback, &context );
    if (pExt)
    {
        *pExt = context.m_PickedExt;
    }
    return context.m_pPicked;
} // JObjectTree::PickNode

void JObjectTree::Traverse( TraverseCallback callback, void* pContext ) const
{
    if (!m_pRoot) return;
    float nodeW    = m_NodeWidth;
    float nodeVisH = m_NodeHeight;
    Frame ext( m_RootPos.x, m_RootPos.y, nodeW, nodeVisH );

    //  find node path for tree expansion
    static std::vector<int> nodePath;
    nodePath.clear();
    JObject* pCurObj = m_pExpanded;
    if (pCurObj) nodePath.push_back( 0 );
    while (pCurObj && pCurObj != m_pRoot)
    {
        JObject* pParent = pCurObj->GetParent();
        if (!pParent) break;
        nodePath.push_back( pParent->GetChildIndex( pCurObj ) );
        pCurObj = pParent;
    }

    //  traverse root node (which is never collapsed)
    if ((this->*callback)( ext, m_pRoot, (m_pExpanded != NULL), pContext ) == false) return;

    //  traverse along expansion path
    pCurObj = m_pRoot;
    for (int i = nodePath.size() - 1; i >= 0; i--)
    {
        int childIdx = nodePath[i];
        if (!pCurObj) break;
        int nCh = pCurObj->GetNChildren();   
        if (childIdx >= nCh) break;
        float blockH = float( nCh )*nodeVisH;
        ext.y = ext.y - blockH*0.5f + nodeVisH*0.5f;
        ext.x += nodeW;
        float cY = ext.y;
        for (int j = 0; j < nCh; j++)
        {
            JObject* pChild = pCurObj->GetChild( j );
            Frame chExt( ext );
            chExt.y = cY;
            bool bExpanded = (j == childIdx)&&(pCurObj != m_pExpanded);
            if ((this->*callback)( chExt, pChild, bExpanded, pContext ) == false) 
            {
                return;
            }
            cY += nodeVisH;
        }
        pCurObj = pCurObj->GetChild( childIdx );
        ext.y += float( childIdx )*nodeVisH; 
    }

} // JObjectTree::Traverse

bool JObjectTree::PickNodeCallback( const Frame& ext, JObject* pObject, 
                                    bool bExpanded, void* pContext ) const
{
    if (!pContext) return false;
    PickContext* pCtx = (PickContext*)pContext;
    Frame nExt( ext );
    nExt.h = m_NodeHeight;
    if (nExt.PtIn( pCtx->m_PickPos.x, pCtx->m_PickPos.y )) 
    {
        pCtx->m_pPicked   = pObject;
        pCtx->m_PickedExt = ext;
        return false;
    }
    return true;
} // JObjectTree::PickNodeCallback

bool JObjectTree::DrawNodeCallback( const Frame& ext, JObject* pObject, 
                                    bool bExpanded, void* pContext ) const
{
    RenderNode( pObject, ext, bExpanded );
    return true;
} // JObjectTree::DrawNodeCallback

void JObjectTree::OnMouse( JMouseEvent& e )
{
    JObject* pNode = PickNode( e.MouseX(), e.MouseY() );
    if (e.Action() == aKeyDown && e.MouseKey() == meLeft)
    {
        SelectObject( pNode );
    }

    if (pNode) e.Consume();
} // JObjectTree::OnMouse

void JObjectTree::SelectObject( JObject* pObject )
{
    if (pObject) 
    {
        m_pSelected = pObject;
        m_pExpanded = m_pSelected;
        SendSignal( "Selected" );
    }
} // JObjectTree::SelectObject

bool JObjectTree::PtIn( int px, int py ) const 
{ 
    return JWidget::PtIn( px, py );
} // JObjectTree::PtIn

void JObjectTree::SetDefRootPos( Vec2 pos )
{
    m_DefaultRootPos = pos;
    SetRootPos( pos );
} // JObjectTree::SetDefRootPos

void JObjectTree::OnDrag( JDragEvent& e )
{
    Vec2 mPos = e.GetCurPos();
    if (e.Key() == mkMiddle)
    {
        if (e.GetType() == deDragStart && IsDraggable())
        {
            if (PickNode( mPos.x, mPos.y ) != NULL)
            {
                e.SetDragObject( this );
                SetDragged();
            }
        }
        if (e.GetType() == deDrag && e.GetDragObject() == this)
        {
            m_RootPos += e.GetDelta();
        }
    }
    else if (e.Key() == mkRight)
    {
        if (e.GetType() == deDragStart)
        {
            Frame ext;
            JObject* pNode = PickNode( mPos.x, mPos.y, &ext );
            if (pNode)
            {
                e.SetSource( this );
                m_pDraggedNode = pNode;
                m_DragShift = Vec2( mPos.x - ext.x, mPos.y - ext.y );
                m_bClone = (GetKeyState( VK_CONTROL ) < 0);
            }
            e.Consume();
        }
        if (e.GetType() == deDrag && e.GetSource() == this)
        {
            //  check whether node can be dropped onto hovered one
            Frame ext;
            JObject* pNode = PickNode( e.GetCurPos().x, e.GetCurPos().y, &ext );
            bool bAsChild = (mPos.x > ext.center().x);
            m_bCanDrop = CanDropNode( m_pDraggedNode, pNode, bAsChild, m_bClone );
            if (m_bCanDrop)
            {
                if (bAsChild)
                {
                    ext.x += ext.w - 10;
                    ext.w = 10;
                    m_InsExt = ext;
                }
                else
                {
                    ext.y += ext.h;
                    ext.w -= 10;
                    ext.h = 4;
                    m_InsExt = ext;
                }
            }
            e.Consume();
        }
        if (e.GetType() == deDrop && e.GetSource() == this && m_pDraggedNode)
        {
            //  find node we are going to drop onto
            Frame ext;
            JObject* pNode = PickNode( e.GetCurPos().x, e.GetCurPos().y, &ext );
            bool bAsChild = (mPos.x > ext.center().x);
            if (CanDropNode( m_pDraggedNode, pNode, bAsChild, m_bClone ))
            {
                DropNode( m_pDraggedNode, pNode, bAsChild, m_bClone );
            }
            m_pDraggedNode = NULL;
            e.Consume();
        }
    }
} // JObjectTree::OnDrag

bool JObjectTree::CanDropNode( JObject* pNode, JObject* pDest, bool bAsChild, bool bClone )
{
    if (!pNode || !pDest)
    {
        return false;
    }

    if (!bAsChild)
    {
        //  add as child node
        if (pDest == pNode && !bClone)
        {
            return false;
        }
    }
    else
    {
        //  insert after the node we dropped onto
        JObject* pParent = pDest->GetParent();
        if (pParent == pNode && !bClone) 
        {
            return false;
        }
    }

    return true;
} // JObjectTree::CanDropNode

void JObjectTree::DropNode( JObject* pNode, JObject* pDest, bool bAsChild, bool bClone )
{
    if (!pNode || !pDest) return;
    JObject* pAddedNode = pNode;
    pNode->AddRef();

    if (bClone)
    {
        const char* nodeName = pNode->GetName();
        pAddedNode = pNode->Clone( NULL, nodeName );
    }
    else
    {
        pNode->GetParent()->RemoveChild( pNode );
    }

    if (!bAsChild)
    {
        //  insert after the node we dropped onto
        JObject* pParent = pDest->GetParent();
        pParent->AddChild( pAddedNode, pParent->GetChildIndex( pDest ) + 1 );
    }
    else
    {
        //  add as child node
        pDest->AddChild( pAddedNode );
    }
    
    pNode->Release();
} // JObjectTree::DropNode

bool JObjectTree::DeleteNode()
{
    if (!m_pSelected) return false;
    JObject* pParent = m_pSelected->GetParent();
    if (!pParent) return false;

    int idx = pParent->GetChildIndex( m_pSelected );
    pParent->RemoveChild( m_pSelected );
    if (idx >= pParent->GetNChildren()) idx = pParent->GetNChildren() - 1;
    if (idx < 0) idx = 0;
    m_pSelected = pParent->GetChild( idx );
    if (!m_pSelected) m_pSelected = pParent;
    m_pExpanded = m_pSelected;
    return true;
} // JObjectTree::DeleteNode

bool JObjectTree::MoveNodeUp()
{
    return false;
} // JObjectTree::MoveNodeUp

bool JObjectTree::MoveNodeDown()
{
    return false;
} // JObjectTree::MoveNodeDown

void JObjectTree::OnKey( JKeyEvent& e )
{
    if (e.Key() == VK_DELETE && e.Action() == aKeyDown)
    {
        DeleteNode();
        e.Consume();
        return;
    }
} // JObjectTree::OnKey

void JObjectTree::SaveSelected()
{
    if (!m_pSelected)
    {
        return;
    }
    SaveFileDialog sf;
    sf.SetDefaultExt( "rjml" );
    if (sf.Show())
    {
        Path path( sf.GetFilePath() );
        if (path.Exists())
        {
            JString ext = path.GetExt();
            ext += ".bak";
            path.SetExt( ext.c_str() );
            CopyFile( sf.GetFilePath(), path.GetFullPath(), FALSE );
        }

        FOutStream os( sf.GetFilePath() );
        g_pPersistServer->Save( m_pSelected, os, PersistFmt_JML );
    }
} // JObjectTree::SaveSelected

void JObjectTree::LoadAsChild()
{
    if (!m_pSelected)
    {
        return;
    }
    OpenFileDialog of;
    of.SetDefaultExt( "rjml" );
    of.AddFilter( "Script Files", "*.rjml" );
    if (of.Show())
    {
        const char* fName = of.GetFilePath();
        JObject* pNode = g_pPersistServer->LoadObject( fName );
        m_pSelected->AddChild( pNode );
        if (!pNode)
        {
            rlog.err( "Could not load script from file: %s", fName ); 
        }
    }
} // JObjectTree::LoadAsChild



