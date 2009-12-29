//****************************************************************************/
//  File:   JWindowServer.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JWINDOWSERVER_H__
#define __JWINDOWSERVER_H__

#include "IWindowServer.h"
#include "JWeakRef.h"
#include "JWidget.h"
#include <windows.h>

enum JWindowBorder
{
    Border_None = 0,
    Border_Fixed = 1,
    Border_Sized = 2
};

enum_beg(JWindowBorder)
enum_val( "None",    Border_None   ),
enum_val( "Fixed",   Border_Fixed  ),
enum_val( "Sized",   Border_Sized  )
enum_end(JWindowBorder)

enum_beg(JBitDepth)
enum_val( "Unknown",    bppUnknown  ),
enum_val( "bpp16",      bpp16       ),
enum_val( "bpp32",      bpp32       )
enum_end(JBitDepth)

enum_beg(JScreenMode)
enum_val( "Unknown",    smUnknown     ),
enum_val( "Window",     smWindow      ),
enum_val( "Desktop",    smDesktop     ),
enum_val( "Fullscreen", smFullscreen  ),
enum_val( "Exclusive",  smExclusive   )
enum_end(JScreenMode)

class JMouseEvent;
class JKeyEvent;
class JDragEvent;
class JWheelEvent;

//****************************************************************************/
//  Class:  JWindowServer, singleton
//  Desc:   Top application window, performs message routing and widget management
/*  Todo:   Refactor the class to make it not so fat
//****************************************************************************/
class JWindowServer : public JWidget, public IWindowServer
{
public:
                        JWindowServer       ();
    JWidget*            PickWidget          ( int px, int py );
    HRESULT             OnWndMessage        ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    void                Render              ();

    int                 RunApplicationLoop  ();

    virtual void        GetMousePos         ( Vec2& pos ) const { pos = m_MousePos; }
    virtual void        SetMousePos         ( const Vec2& pos );

    virtual void*       GetRootHandle       () const { return (void*)m_hWnd; }
    virtual void        SetRootHandle       ( void* hwnd ) { m_hWnd = (HWND)hwnd; }
    virtual void        GetRootExt          ( Frame& ext ) const;
    virtual void        SetRootExt          ( const Frame& ext );
    virtual void        Exit                ();
    virtual JScreenMode GetScreenMode       () const { return m_ScreenMode; }

    float               GetMouseX           () const { return m_MousePos.x; }
    float               GetMouseY           () const { return m_MousePos.y; }
    void                SetMouseX           ( float val );
    void                SetMouseY           ( float val );

    void                ToggleMinimized     ();

    JDragEvent&         GetDragEvent        () { return m_DragEvent; }

    virtual void        Init                ();

    virtual bool        DestroyWnd          ();
    virtual bool        CreateWnd           ( JScreenMode screenMode );

    bool                IsTrayMode          () const { return m_bTrayMode; }
    void                SetTrayMode         ( bool bTray = true );

    void                SetMouseCapture     ( JWidget* pCapture );
    JWidget*            GetMouseCapture     () const { return m_pMouseCapture; }

    void                SetFocus            ( JWidget* pFocus );
    JWidget*            GetFocus            () const { return m_pFocus; }

    void                SetFPSCap           ( float val ) { m_FPSCap = val; }
    float               GetFPSCap           () const { return m_FPSCap; }

    void                ReloadWidgets       ();

    static JWindowServer* s_pInstance;

    //  FIXME: dirty way to override JObject's RenderTree
    virtual void        RenderTree          () { Render(); }

    virtual void        RenderFrame         ();

    expose(JWindowServer)
    {
        parent(JWidget);
        field( "ScreenMode",        m_ScreenMode            );
        field( "BitDepth",          m_BitDepth              );
        field( "FPSCap",            m_FPSCap                );
        field( "HasStencil",        m_bHasStencil           );
        field( "DefTrayIcon",       m_DefTrayIcon           );
        field( "DisplayWidth",      m_DisplayWidth          );
        field( "DisplayHeight",     m_DisplayHeight         );
        field( "DisplayFrequency",  m_DisplayFrequency      );
        field( "AlwaysUpdate",      m_bAlwaysUpdate         );
        field( "DragStartDist",     m_DragStartDist         );
        field( "RunOnce",           m_bRunOnce              );
        field( "WindowBorder",      m_WindowBorder          );

        prop ( "TrayMode",          IsTrayMode, SetTrayMode );

        method( "ToggleMinimized",  ToggleMinimized         );
        method( "Exit",             Exit                    );
        method( "ReloadWidgets",    ReloadWidgets           );
        prop  ( "MouseX",           GetMouseX, SetMouseX    );
        prop  ( "MouseY",           GetMouseY, SetMouseY    );
    }

protected:
    virtual void        OnMouse             ( JMouseEvent& e );
    virtual void        OnWheel             ( JWheelEvent& e );
    virtual void        OnKey               ( JKeyEvent&   e );

    void                SendWheelEvent      ( JWheelEvent& e, JWidget* pObj );
    void                RenderTree          ( JObject* pObj, bool bRenderFocus, const Frame& curExt );
    bool                HasInvisibleParent  ( JObject* pObj );

private:
    JWeakRef<JWidget>   m_pFocus;
    JWeakRef<JWidget>   m_pMouseCapture;

    JScreenMode         m_ScreenMode; 
    JBitDepth           m_BitDepth;    
    JWindowBorder       m_WindowBorder;
    bool                m_bTrayMode;  
    bool                m_bMinimized;  
    bool                m_bHasStencil;
    bool                m_bAppActive;
    JString             m_DefTrayIcon;
    JString             m_WndClassName;

    Vec2                m_MousePos;
    int                 m_DisplayWidth;
    int                 m_DisplayHeight;
    int                 m_DisplayFrequency;
    float               m_FPSCap;
    bool                m_bAlwaysUpdate;

    JDragEvent          m_DragEvent;
    float               m_DragStartDist;
    bool                m_bRunOnce;

    bool                m_bQuit;

    HWND                m_hWnd;
    WNDPROC             HookedWndProc;

    DWORD               m_FPSThreadID;
    HANDLE              m_hFPSThread;
    HANDLE              m_hFPSEvent;

    static DWORD WINAPI FPSThreadProcStart  ( LPVOID lpParam );
    DWORD WINAPI        FPSThreadProc       ( LPVOID lpParam );

}; // class JWindowServer

#endif // __JWINDOWSERVER_H__


