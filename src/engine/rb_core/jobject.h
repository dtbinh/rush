//****************************************************************************/
//  File:  JObject.h
//****************************************************************************/
#ifndef __JOBJECT_H__
#define __JOBJECT_H__

#include "jstring.h"

class JMetaClass;
class JObject;
class OutStream;
class InStream;
class JEvent;

//****************************************************************************/
//  Class:  JObject
//  Desc:
//****************************************************************************/
class JObject
{
public:
                            JObject         ();
    virtual                 ~JObject        ();
    void                    Destroy         ();

    virtual void            Expose          ( JMetaClass& pmap );
    virtual const char*     ClassName       () const { return "object"; }
    virtual void            Release         ();

    virtual void            OnEvent         ( JEvent* evt ) {}

    virtual void            Render          () {}
    virtual void            PostRender      () {}
    virtual	void	        DrawBounds	    () {}

    virtual void            Init            () {}
    virtual void            ResInit         () {}
    virtual void            AddRef          ();
    virtual JObject*        Clone           ( JObject* pParent, const char* name, bool bCloneSignals = false ) const;

    inline int              GetNRef         () const;
    inline const char*      GetName         () const;
    void                    SetName         ( const char* name );
    void                    AddChild        ( JObject* pObj, int idxBefore = -1 );
    void                    RemoveChild     ( JObject* pObj );
    void                    RemoveChildren  ();
    void                    SwapChildren    ( int idx1, int idx2 );
    inline bool             HasName         ( const char* name );

    inline int              GetTag          () const;
    void                    SetTag          ( int tag, bool bRecursive = false );

    inline JObject*         GetChild        ( int idx ) const;
    inline int              GetNChildren    () const;
    inline JObject*         GetParent       () const;
    inline void             SetParent       ( JObject* pParent );

    bool                    ReplaceChild    ( JObject* pOld, JObject* pNew );
    bool                    HasParent       ( JObject* pParent ) const;
    void                    GetPath         ( JString& path ) const;
    void                    InitTree        ();

    bool                    IsVisible       () const;
    void                    SetVisible      ( bool bVisible );

    bool                    IsPathVisible   () const;

    bool                    IsEnabled       () const;
    void                    SetEnabled      ( bool bEnabled );

    bool                    IsSignalSink    () const;
    void                    SetSignalSink   ( bool bSignalSink );

    bool                    IsDrawBounds    () const;
    void                    SetDrawBounds   ( bool bDraw );

    void                    SendSignals     ();
    void                    PollSlots       ();
    void                    SendSignal      ( const char* name );
    bool                    PollSlot        ( const char* name );
    bool                    SetProperty     ( const char* prop, const char* val, int tag = -1 );
    bool                    GetProperty     ( const char* prop, JString& val, int tag = -1 );
    bool                    CallMethod      ( const char* method, int tag = -1 );

    int                     GetLevel        () const;
    int                     GetChildIndex   ( const JObject* pChild ) const;
    bool                    HasSymbolicPath ( const char* path ) const;

    virtual void            RenderTree      ();

    virtual void            Serialize       ( OutStream& os ) const {}
    virtual bool            Unserialize     ( InStream& is ) { return true; }

    virtual bool            OnSignal        ( JObject* pSrc, const char* pSrcAttr, const char* pDstAttr ){ return false; }

    void                    SaveTree        ();

    int                     GetIndex        () const;
    void                    SetIndex        ( int idx );

    friend class            JObjectKey;
    friend class            JObjectIterator;
    friend class            JObjectServer;

    virtual JMetaClass*     GetMetaClass    () const;
    static JMetaClass*      s_pMetaClass;

private:
    std::vector<JObject*>   m_Children; //  child objects
    JObject*                m_pParent;
    JString                 m_Name;
    int                     m_RefCount;
    int                     m_Tag;

    bool                    m_bVisible;
    bool                    m_bDrawBounds;
    bool                    m_bSignalSink;
    bool                    m_bEnabled;

    void                    DoRender();
    void                    DoRenderTree();

}; // class JObject

//****************************************************************************/
//  JObject inlines
//****************************************************************************/
inline int JObject::GetNRef() const
{
    return m_RefCount;
}

inline const char* JObject::GetName() const
{
    return m_Name.c_str();
}

inline bool JObject::HasName( const char* name )
{
    return m_Name.is_equal_ci( name );
}

inline int JObject::GetTag() const
{
    return m_Tag;
}

inline int JObject::GetNChildren() const
{
    return m_Children.size();
}

inline bool JObject::IsVisible() const
{
    return m_bVisible;
}

inline void JObject::SetVisible( bool bVisible )
{
    m_bVisible = bVisible;
    SendSignal( "Visible" );
}

inline bool JObject::IsEnabled() const
{
    return m_bEnabled;
}

inline void JObject::SetEnabled( bool bEnabled )
{
    m_bEnabled = bEnabled;
    SendSignal( "Enabled" );
}

inline bool JObject::IsDrawBounds() const
{
    return m_bDrawBounds;
}

inline void JObject::SetDrawBounds( bool bDraw )
{
    m_bDrawBounds = bDraw;
}

inline bool JObject::IsSignalSink() const
{
    return m_bSignalSink;
}

inline void JObject::SetSignalSink( bool bSignalSink )
{
    m_bSignalSink = bSignalSink;
}

inline JObject* JObject::GetParent() const
{
    return m_pParent;
}

inline void JObject::SetParent( JObject* pParent )
{
    m_pParent = pParent;
}

inline JObject* JObject::GetChild( int idx ) const
{
    if (idx < 0 || idx >= m_Children.size()) return NULL;
    return m_Children[idx];
}

inline bool JObject::IsPathVisible() const
{
    bool bVisible = IsVisible();
    JObject* pParent = GetParent();
    while (pParent && bVisible)
    {
        bVisible = pParent->IsVisible();
        pParent = pParent->GetParent();
    }
    return bVisible;
} // JWidget::IsInvisible

extern int g_NumTypes;
template <class T> int GetTypeID(...)
{
    static int typeID = ++g_NumTypes;
    return typeID;
}

#include "jreflect.h"

void TunnelEvent( JObject* pRoot, JEvent* evt );
void BubbleEvent( JObject* pRoot, JEvent* evt );

#endif // __JOBJECT_H__


