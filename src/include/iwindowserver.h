/*****************************************************************************/
/*  File:  IWindowServer.h
/*****************************************************************************/
#ifndef __IWINDOWSERVER_H__
#define __IWINDOWSERVER_H__

/*****************************************************************************/
/*  Enum:  JBitDepth
/*  Desc:  
/*****************************************************************************/
enum JBitDepth
{
    bppUnknown  = 0, 
    bpp16       = 16,
    bpp32       = 32
}; // enum JBitDepth

/*****************************************************************************/
/*  Enum:  JScreenMode
/*  Desc:  
/*****************************************************************************/
enum JScreenMode
{
    smUnknown     = 0, 
    smWindow      = 1, 
    smDesktop     = 2, 
    smFullscreen  = 3, 
    smExclusive   = 4, 
}; // enum ScreenMode

class JObject;
class JWidget;
class JDragEvent;

/*****************************************************************************/
/*  Class:  IWindowServer
/*  Desc:  
/*****************************************************************************/
class IWindowServer 
{
public:
    virtual void        GetMousePos         ( Vec2& pos ) const = 0;
    virtual void        SetMousePos         ( const Vec2& pos ) = 0;

    virtual void*       GetRootHandle       () const = 0;
    virtual void        SetRootHandle       ( void* hwnd ) = 0;
    virtual void        GetRootExt          ( Frame& ext ) const = 0;
    virtual void        SetRootExt          ( const Frame& ext ) = 0;
    virtual JScreenMode GetScreenMode       () const = 0;

    virtual bool        DestroyWnd          () = 0;
    virtual bool        CreateWnd           ( JScreenMode screenMode ) = 0;

    virtual void        Exit                () = 0;
    virtual void        RenderFrame         () = 0;
    virtual int         RunApplicationLoop  () = 0;

    virtual JWidget*    GetMouseCapture     () const = 0;
    virtual JWidget*    GetFocus            () const = 0;

    virtual JDragEvent& GetDragEvent        () = 0;

    virtual void        SetMouseCapture     ( JWidget* pWidget ) = 0;
    virtual void        SetFocus            ( JWidget* pWidget ) = 0;

    virtual void        AddWindow           ( JObject* pWidget ) = 0;
    
}; // class IWindowServer

extern IWindowServer* g_pWindowServer;

#endif // __IWINDOWSERVER_H__


