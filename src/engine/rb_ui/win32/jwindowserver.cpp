//****************************************************************************/
//  File:   JWindowServer.cpp
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "jreflect.h"
#include "jcore.h"
#include "ipersistserver.h"
#include "jwidget.h"
#include "jdialog.h"
#include "jmouseevent.h"
#include "jkeyevent.h"
#include "jdragevent.h"
#include "jdropfileevent.h"
#include "jwheelevent.h"
#include "fstream.h"
#include "isoundserver.h"
#include "iscriptserver.h"
#include "idrawserver.h"
#include "jobjectiterator.h"
#include "jrevobjectiterator.h"
#include "jwindowserver.h"

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return JWindowServer::s_pInstance->OnWndMessage( hWnd, uMsg, wParam, lParam );
}

//****************************************************************************/
/*  JWindowServer implementation
//****************************************************************************/
decl_class( JWindowServer );
JWindowServer* JWindowServer::s_pInstance = NULL;

JWindowServer::JWindowServer()
{
    m_pFocus                = NULL;
    m_pMouseCapture         = NULL;
    m_ScreenMode            = smDesktop;
    m_BitDepth              = bpp16;
    m_bHasStencil           = false;
    m_bTrayMode             = false;
    m_DefTrayIcon           = "";
    m_WndClassName          = "__RB_Window__";
    m_bAppActive            = true;
    m_hWnd                  = NULL;
    m_bMinimized            = false;
    m_bRunOnce              = false;
    m_bAlwaysUpdate         = false;
    m_bQuit                 = false;
    m_WindowBorder          = Border_Sized;

    m_DisplayWidth          = -1;
    m_DisplayHeight         = -1;
    m_DisplayFrequency      = -1;
    m_DragStartDist         = 2.0f;

    m_FPSCap                = 50.0f;
    HookedWndProc           = NULL;


    m_FPSThreadID           = 0;
    m_hFPSThread            = NULL;
    m_hFPSEvent             = NULL;

    SetName( "wnd" );
    m_LocalExt = Frame( 0.0f, 0.0f, 800.0f, 600.0f );

    s_pInstance = this;
    g_pWindowServer = this;
}

void JWindowServer::Exit()
{
    PostQuitMessage( 0 );
}

void JWindowServer::SetFocus( JWidget* pFocus )
{
    if (m_pFocus)
    {
        m_pFocus->OnFocus( false );
        m_pFocus->m_bHasFocus = false;
    }
    m_pFocus = pFocus;
    if (m_pFocus)
    {
        m_pFocus->OnFocus( true );
        m_pFocus->m_bHasFocus = true;
    }
}

HRESULT JWindowServer::OnWndMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_LBUTTONDBLCLK:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkLeft;
            e.m_InputAction = aDblClick;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }
    case WM_NCMOUSEHOVER:
        {
            ShowCursor( TRUE );
            break;
        }

    case WM_MBUTTONDBLCLK:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkMiddle;
            e.m_InputAction = aDblClick;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_RBUTTONDBLCLK:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkRight;
            e.m_InputAction = aDblClick;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_RBUTTONDOWN:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkRight;
            e.m_InputAction = aKeyDown;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_RBUTTONUP:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkRight;
            e.m_InputAction = aKeyUp;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_LBUTTONDOWN:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkLeft;
            e.m_InputAction = aKeyDown;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_LBUTTONUP:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkLeft;
            e.m_InputAction = aKeyUp;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_MBUTTONDOWN:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkMiddle;
            e.m_InputAction = aKeyDown;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_MBUTTONUP:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkMiddle;
            e.m_InputAction = aKeyUp;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            OnMouse( e );
            break;
        }

    case WM_MOUSEMOVE:
        {
            JMouseEvent e;
            e.m_MouseKey    = mkNone;
            e.m_InputAction = aMouseMove;
            e.m_MouseX      = LOWORD( lParam );
            e.m_MouseY      = HIWORD( lParam );
            if (GetKeyState( VK_LBUTTON ) < 0) e.m_MouseKey = mkLeft;
            if (GetKeyState( VK_MBUTTON ) < 0) e.m_MouseKey = mkMiddle;
            if (GetKeyState( VK_RBUTTON ) < 0) e.m_MouseKey = mkRight;
            OnMouse( e );
            break;
        }

    case WM_MOUSEWHEEL:
        {
            JWheelEvent e;
            e.m_Delta = (short) HIWORD( wParam );
            OnWheel( e );
            break;
        }

    case WM_KEYDOWN:
        {
            JKeyEvent e;
            e.m_InputAction = aKeyDown;
            e.m_Key         = wParam;
            e.m_Char        = 0;

            MSG msg;
            msg.hwnd        = NULL;
            msg.message     = uMsg;
            msg.wParam      = wParam;
            msg.lParam      = lParam;
            TranslateMessage( &msg );
            PeekMessage( &msg, NULL, WM_CHAR, WM_CHAR, TRUE );
            if (msg.message == WM_CHAR)
            {
                e.m_Char = msg.wParam;
            }
            OnKey( e );
            break;
        }
    case WM_KEYUP:
        {
            JKeyEvent e;
            e.m_InputAction = aKeyUp;
            e.m_Key         = wParam;
            e.m_Char        = 0;
            OnKey( e );
            break;
        }
    case WM_CREATE:
        {
            break;
        }
    case WM_CLOSE:
        {
            PostQuitMessage( 0 );
            break;
        }

    case WM_ACTIVATEAPP:
        {
            m_bAppActive = (wParam != 0);
            break;
        }

    case WM_QUIT:
        {
            break;
        }

    case WM_SETCURSOR:
        {
            return TRUE;
        }
    case WM_SIZE:
        {
            int w = LOWORD( lParam );
            int h = HIWORD( lParam );
            g_pRenderServer->ResizeBackBuffer( w, h );
        }
    case WM_PAINT:
        {
            if (g_pRenderServer && g_pDrawServer)
            {
                g_pRenderServer->ClearViewport( 0xFF000000 );
                g_pRenderServer->ClearDepthStencil( true, true );
                JCore::s_pInstance->RenderTree();
                g_pDrawServer->Flush();
                g_pRenderServer->EndFrame();
            }
            break;
        }
    case WM_DROPFILES:
        {
             HDROP hDrop = (HDROP)wParam;
             POINT dropPt;
             DragQueryPoint( hDrop, &dropPt );
             UINT nDropFiles = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );
             char path[_MAX_PATH];
             for (UINT i = 0; i < nDropFiles; i++)
             {
                DragQueryFile( hDrop, i, path, _MAX_PATH );
                JDropFileEvent evt( path, Vec2( dropPt.x, dropPt.y ) );
                TunnelEvent( this, &evt );
             }
        }
        break;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void JWindowServer::OnMouse( JMouseEvent& e )
{
    Vec2 cPos( float( e.MouseX() ), float( e.MouseY() ) );

    //  cancel dragging if it was ended, or no object was picked at the start
    if (m_DragEvent.m_Type == deDrop ||
        m_DragEvent.m_Type == deDragStart)
    {
        m_DragEvent.Reset();
    }
    if (e.Action() == aKeyDown)
    {
        m_DragEvent.SetStartPos( cPos );
        m_DragEvent.m_Key       = e.m_MouseKey;
    }
    m_DragEvent.SetCurPos( cPos );
    m_DragEvent.m_bConsumed = false;
    if (e.Action() == aKeyUp ||
        (e.MouseKey() == mkNone && m_DragEvent.m_Type != deNone))
    {
        if (!m_DragEvent.m_bSticky ||
            (m_DragEvent.m_pObject && !m_DragEvent.m_pObject->IsDragged()))
        {
            m_DragEvent.m_Type      = deDrop;
        }
    }
    if (m_DragEvent.m_Key != mkNone &&
        m_DragEvent.m_Type == deNone &&
        cPos.dist( m_DragEvent.m_StartPos ) >= m_DragStartDist)
    {
        m_DragEvent.m_Type      = deDragStart;
        m_DragEvent.m_pSource   = NULL;
        m_DragEvent.m_pTarget   = NULL;
        m_DragEvent.m_pObject   = NULL;
    }

    m_MousePos = cPos;

    //  first send mouse event to mouse capture and focus widgets
    if (m_pMouseCapture)
    {
        m_pMouseCapture->SendMouseEvent( e, m_DragEvent );
        if (e.Consumed())
        {
            return;
        }
    }

    if (m_pFocus)
    {
        m_pFocus->SendMouseEvent( e, m_DragEvent );
        if (e.Consumed())
        {
            return;
        }
    }

    //  route event to the children subtree
    int nCh = GetNChildren();
    for (int i = nCh - 1; i >= 0; i--)
    {
        JWidget* pChildWidget = obj_cast<JWidget>( GetChild( i ) );
        if (!pChildWidget) continue;
        pChildWidget->SendMouseEvent( e, m_DragEvent );
        if (e.Consumed()) break;
    }

    //  send drop event to the source, if has not been sent yet
    if (m_DragEvent.m_Type == deDrop)
    {
        if (m_DragEvent.m_pSource &&
            !m_DragEvent.m_pSource->PtIn( cPos.x, cPos.y ))
        {
            m_DragEvent.m_pSource->OnDrag( m_DragEvent );
        }
        if (m_DragEvent.m_pObject)
        {
            m_DragEvent.m_pObject->OnDrag( m_DragEvent );
            m_DragEvent.m_pObject->SetDragged( false );
        }
    }

    //  send drag event to the dragged object, if it has not been sent yet
    if (m_DragEvent.m_Type == deDrag)
    {
        if (m_DragEvent.m_pObject &&
            !m_DragEvent.m_pObject->PtIn( cPos.x, cPos.y ))
        {
            m_DragEvent.m_pObject->OnDrag( m_DragEvent );
        }
    }

}

void JWindowServer::SendWheelEvent( JWheelEvent& e, JWidget* pObj )
{
    if (!pObj) return;
    JRevObjectIterator it( pObj );
    if (pObj != this && !e.Consumed()) pObj->OnWheel( e );
    while (it && !e.Consumed())
    {
        JWidget* pChild = obj_cast<JWidget>( *it );
        ++it;
        if (!pChild || !pChild->IsPathVisible() || pChild == this) continue;
        pChild->OnWheel( e );
    }
}

void JWindowServer::OnWheel( JWheelEvent& e )
{
    SendWheelEvent( e, m_pMouseCapture );
    if (e.Consumed()) return;

    SendWheelEvent( e, m_pFocus );
    if (e.Consumed()) return;

    JRevObjectIterator it( this );
    while (it && !e.Consumed())
    {
        JWidget* pChild = obj_cast<JWidget>( *it );
        ++it;
        if (!pChild || !pChild->IsPathVisible() ||
            pChild == this ||
            pChild->HasParent( m_pFocus )) continue;
            pChild->OnWheel( e );
    }
}


void JWindowServer::OnKey( JKeyEvent& e )
{
    //  pass to focused widget
    JRevObjectIterator it( m_pFocus );
    while (it && !e.Consumed())
    {
        JWidget* pObj = obj_cast<JWidget>( *it );
        ++it;
        if (!pObj || !pObj->IsPathVisible()) continue;
        pObj->OnKey( e );
    }

    //  pass to all parents of the focused widget
    if (m_pFocus)
    {
        JWidget* pCurrent = obj_cast<JWidget>( m_pFocus->GetParent() );
        while (pCurrent && !e.Consumed() && pCurrent != this)
        {
            pCurrent->OnKey( e );
            pCurrent = obj_cast<JWidget>( pCurrent->GetParent() );
        }
    }

    //  if not still processed route event to whole tree
    if (!e.Consumed())
    {
        JRevObjectIterator it( this );
        while (it && !e.Consumed())
        {
            JObject* pObj = *it;
            ++it;
            if (!pObj) continue;
            JWidget* pWidget = obj_cast<JWidget>( pObj );
            if (!pWidget || !pWidget->IsPathVisible()) continue;
            pWidget->AddRef();
            pWidget->OnKey( e );
            if (pWidget->GetNRef() <= 1)
            {
                //  object was deleted as a result of key event
                pWidget->Release();
                break;
            }
            pWidget->Release();
        }
    }
}

void JWindowServer::ReloadWidgets()
{
    for (int i = 0; i < GetNChildren(); i++)
    {
        JObject* pChild = GetChild( i );
        g_pPersistServer->LoadObject( pChild->GetName(), pChild );
        pChild->InitTree();
    }
}

JWidget* JWindowServer::PickWidget( int px, int py )
{
    JRevObjectIterator it( this );
    ++it;
    JWidget* pRes = NULL;
    while (it)
    {
        JWidget* pObj = obj_cast<JWidget>( *it );
        ++it;
        if (!pObj || !pObj->IsPathVisible() || !pObj->PtIn( px, py ))
        {
            continue;
        }
        pRes = pObj;
    }
    return pRes;
}

void JWindowServer::RenderTree( JObject* pObj, bool bRenderFocus, const Frame& curExt )
{
    if (!pObj) return;
    if (!pObj->IsVisible())
    {
        return;
    }

    JWidget* pWidget = obj_cast<JWidget>( pObj );

    bool bSkipRender = (!bRenderFocus && pWidget && pWidget->HasFocus() != bRenderFocus);

    Frame ext( curExt );
    //  render object
    Frame vp;
    if (pObj != this)
    {
        //  automatic slots polling
        if (pObj->IsSignalSink())
        {
            pObj->PollSlots();
        }
        if (pWidget)
        {
            if (!bSkipRender)
            {
                if (pWidget->IsClipToExt() && g_pRenderServer)
                {
                    vp = g_pRenderServer->GetViewport();
                    g_pRenderServer->SetViewport( pWidget->GetExt() );
                }
                pWidget->AdjustSize();
                pWidget->Render();
            }

            ext = pWidget->GetExt();
        }
        else
        {
            pObj->Render();
        }
    }

    //  draw debug bounds, if overriden
    if (IsDrawBounds() && !pObj->IsDrawBounds())
    {
        pObj->DrawBounds();
    }

    //  render children
    int nCh = pObj->GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        JObject* pChild = pObj->GetChild( i );
        RenderTree( pChild, bRenderFocus, ext );
    }

    //  post-render object
    if (pObj != this) pObj->PostRender();

    //  restore viewport
    if (pWidget && pWidget != this && pWidget->IsClipToExt() && g_pRenderServer)
    {
        g_pRenderServer->SetViewport( vp );
    }
}

bool JWindowServer::HasInvisibleParent( JObject* pObj )
{
    if (!pObj) return false;
    JObject* pParent = pObj;
    while (pParent)
    {
        if (!pParent->IsVisible()) return true;
        pParent = pParent->GetParent();
    }
    return false;
}

void JWindowServer::SetMouseCapture( JWidget* pCapture )
{
    m_pMouseCapture = pCapture;
}

void JWindowServer::Render()
{
    if (g_pRenderServer)
    {
        g_pRenderServer->SetViewport( GetExt() );
    }

    if (HasInvisibleParent( m_pFocus ))
    {
        m_pFocus->SetFocus( false );
    }

    if (HasInvisibleParent( m_pMouseCapture ))
    {
        m_pMouseCapture->CaptureMouse( false );
    }

    SendSignals();
    RenderTree( this, false, GetExt() );
    if (m_pFocus)
    {
        RenderTree( m_pFocus, true, m_pFocus->GetParentExt() );
    }

    //  find current cursor
    Vec2 mPos;
    GetMousePos( mPos );
    JWidget* pCursorObj = NULL;
    JObjectIterator it( this );
    ++it;
    while (it)
    {
        JWidget* pWidget = obj_cast<JWidget>( *it );
        if (!pWidget)
        {
            ++it;
            continue;
        }
        if (!pWidget->IsVisible() || !pWidget->IsEnabled())
        {
            it.breadth_next();
            continue;
        }
        if (pWidget->GetCursor()[0] != 0 && pWidget->PtIn( mPos.x, mPos.y ))
        {
            pCursorObj = pWidget;
        }
        ++it;
    }
    SetCursor( pCursorObj ? pCursorObj->GetCursor() : "default" );

}

void JWindowServer::GetRootExt( Frame& ext ) const
{
    ext = GetExt();
}

void JWindowServer::SetRootExt( const Frame& ext )
{
    SetExt( ext );
}

void JWindowServer::Init()
{
    //  FIXME: proper initialization/naming of the window server
    g_pPersistServer->LoadObject( "rootwindow", this );
    SetName( "wnd" );

    if (m_bRunOnce)
    {
        HWND hOldWnd = FindWindow( m_WndClassName.c_str(), m_Text.c_str() );
        if (hOldWnd != NULL)
        {
            rlog.err( "Only one instance of the window is allowed: %s", m_WndClassName.c_str() );
            return;
        }
    }

    if (m_hWnd != NULL)
    {
        SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)MsgProc );

        RECT rct;
        GetWindowRect( m_hWnd, &rct );
        SetExt( Frame( 0.0f, 0.0f, rct.right - rct.left, rct.bottom - rct.top ) );
    }
    else
    {
        CreateWnd( m_ScreenMode );
    }
    ::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}

bool JWindowServer::DestroyWnd()
{
    if (m_hWnd == NULL)
    {
        return false;
    }
    ::DestroyWindow( m_hWnd );
    m_hWnd = NULL;
    return true;
}

bool JWindowServer::CreateWnd( JScreenMode screenMode )
{
    if (m_hWnd != NULL)
    {
        return false;
    }

    m_ScreenMode = screenMode;

    HINSTANCE hInst = GetModuleHandle( NULL );
    //  register the window class
    WNDCLASSA wndClass = { CS_DBLCLKS, MsgProc, 0, 0, hInst, NULL, NULL,
                            (HBRUSH)GetStockObject( BLACK_BRUSH ),
                            NULL, m_WndClassName.c_str() };
    RegisterClassA( &wndClass );

    RECT rcDesktop;
    SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesktop, 0 );

    Frame wrc = GetExt();
    Frame rcParent;
    uint32_t wndStyle = WS_POPUP;
    uint32_t wndExStyle = 0;
    if (m_ScreenMode == smWindow)
    {
        wrc.w = m_DisplayWidth;
        wrc.h = m_DisplayHeight;
        rcParent = wrc;
        wndStyle = WS_POPUP;
        if (m_WindowBorder != Border_None)
        {
            wndStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |
                    WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        if (m_WindowBorder == Border_Sized)
        {
            wndStyle |= WS_MAXIMIZEBOX | WS_SIZEBOX | WS_VISIBLE | WS_THICKFRAME;
        }
        wndExStyle = WS_EX_ACCEPTFILES;
    }
    else if (m_ScreenMode == smDesktop)
    {
        rcParent.x = rcDesktop.left;
        rcParent.y = rcDesktop.top;
        rcParent.w = rcDesktop.right - rcDesktop.left;
        rcParent.h = rcDesktop.bottom - rcDesktop.top;
        wndStyle = WS_POPUP;
        wndExStyle = WS_EX_ACCEPTFILES;
    }
    else if (m_ScreenMode == smFullscreen || m_ScreenMode == smExclusive)
    {
        DEVMODE mode;
        EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &mode );
        rcParent.x = 0;
        rcParent.y = 0;
        rcParent.w = m_DisplayWidth;
        rcParent.h = m_DisplayHeight;
        wndStyle = WS_POPUP | WS_VISIBLE;
    }

    wrc = ApplyAlignment( wrc, rcParent, GetXAlign(), GetYAlign() );

    SetExt( wrc );
    AdjustSize();

    // Create the render window
    m_hWnd = CreateWindowEx( wndExStyle, m_WndClassName.c_str(), m_Text.c_str(), wndStyle,
                            wrc.x, wrc.y, wrc.w, wrc.h,
                            0, NULL, hInst, 0 );

    if (m_ScreenMode == smWindow && m_WindowBorder != Border_None)
    {
        RECT clRect;
        GetClientRect( m_hWnd, &clRect );
        wrc.w += (wrc.w - (clRect.right - clRect.left));
        wrc.h += (wrc.h - (clRect.bottom - clRect.top));
    }

    ShowWindow( m_hWnd, SW_SHOW );

    int wndX = wrc.x;
    int wndY = wrc.y;
    if (m_ScreenMode == smWindow)
    {
        wndX = (rcDesktop.right - m_DisplayWidth)/2;
        wndY = (rcDesktop.bottom - m_DisplayHeight)/2;
    }
    MoveWindow( m_hWnd, wndX, wndY, wrc.w, wrc.h, true );
    UpdateWindow( m_hWnd );
    return true;
}

uint32_t WINAPI JWindowServer::FPSThreadProcStart( LPVOID lpParam )
{
   return s_pInstance->FPSThreadProc( lpParam );
}

uint32_t WINAPI JWindowServer::FPSThreadProc( LPVOID lpParam )
{
   while (!m_bQuit)
   {
      uint32_t frameLen = uint32_t( 1000.0f/m_FPSCap );
      Sleep( frameLen );
      SetEvent( m_hFPSEvent );
   }
   SetEvent( m_hFPSEvent );
   return 0;
}

int JWindowServer::RunApplicationLoop()
{
    m_hFPSEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_hFPSThread = CreateThread( NULL, 0, FPSThreadProcStart, 0, 0, &m_FPSThreadID );
    SetThreadPriority( m_hFPSThread, THREAD_PRIORITY_TIME_CRITICAL );

    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

    while (msg.message != WM_QUIT && IsWindow( m_hWnd ))
    {
        bool bHaveMsg = (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0);
        if (bHaveMsg)
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            bool bRender = true;
            if (!m_bAppActive && !m_bAlwaysUpdate)
            {
                bRender = false;
            }
            if (bRender)
            {
                RenderFrame();
            }
            else Sleep( 100 );
            WaitForSingleObject( m_hFPSEvent, INFINITE );
        }
    }
    m_bQuit = true;
    return (int)msg.wParam;
}

void JWindowServer::RenderFrame()
{
    if (g_pRenderServer)
    {
        if (!g_pRenderServer->StartFrame())
        {
            Sleep( 100 );
            return;
        }
        g_pRenderServer->ClearViewport( 0xFF000000 );
        g_pRenderServer->ClearDepthStencil( true, true );
    }

    JCore::s_pInstance->RenderTree();

    if (g_pSoundServer)
    {
        g_pSoundServer->Update();
    }

    if (g_pDrawServer)
    {
        g_pDrawServer->Flush();
    }

    if (g_pRenderServer)
    {
        g_pRenderServer->EndFrame();
    }
}

void JWindowServer::SetMousePos( const Vec2& pos )
{
    m_MousePos = pos;
    SetCursorPos( m_MousePos.x, m_MousePos.y );
}

void JWindowServer::SetMouseX( float val )
{
    m_MousePos.x = val;
    SetCursorPos( m_MousePos.x, m_MousePos.y );
}

void JWindowServer::SetMouseY( float val )
{
    m_MousePos.y = val;
    SetCursorPos( m_MousePos.x, m_MousePos.y );
}

void JWindowServer::ToggleMinimized()
{

}

void JWindowServer::SetTrayMode( bool bTray )
{

}





