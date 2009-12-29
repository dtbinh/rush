//****************************************************************************/
//  File:  JObject.cpp
//****************************************************************************/
#include "stdafx.h"
#include "JObject.h"
#include "JSignalServer.h"
#include "IPersistServer.h"
#include "JEvent.h"

//****************************************************************************/
/*  Utility methods
//****************************************************************************/
void TunnelEvent( JObject* pRoot, JEvent* evt )
{
    pRoot->OnEvent( evt );
    if (evt->Consumed())
    {
        return;
    }
    for (int i = 0; i < pRoot->GetNChildren(); i++)
    {
        TunnelEvent( pRoot->GetChild( i ), evt );
        if (evt->Consumed())
        {
            return;
        }
    }
}

void BubbleEvent( JObject* pRoot, JEvent* evt )
{
    for (int i = pRoot->GetNChildren() - 1; i > 0 ; i--)
    {
        BubbleEvent( pRoot->GetChild( i ), evt );
        if (evt->Consumed())
        {
            return;
        }
    }
    pRoot->OnEvent( evt );
    if (evt->Consumed())
    {
        return;
    }
}


int g_NumTypes = 0;
//****************************************************************************/
/*  JObject implementation
//****************************************************************************/
decl_class(JObject)
JObject::JObject() : 
    m_pParent       ( NULL ),
    m_RefCount      ( 0 ),
    m_bVisible      ( true ),
    m_bEnabled      ( true ),
    m_bSignalSink   ( false ),
    m_Tag           ( -1 ),
    m_bDrawBounds   ( false )
{
} // JObject::JObject

JObject::~JObject()
{
    Destroy();
} // JObject::~JObject


void JObject::Expose( JMetaClass& mc )
{
    typedef JObject MyType;
    prop  ( "Name",                GetName, SetName );
    rprop ( "Type",                ClassName        );
    field ( "Tag",                 m_Tag            );

    //  flags
    prop  ( "Visible",             IsVisible,    SetVisible    );
    prop  ( "Enabled",             IsEnabled,    SetEnabled    );
    prop  ( "SignalSink",          IsSignalSink, SetSignalSink );
    prop  ( "DrawBounds",          IsDrawBounds, SetDrawBounds );

    prop  ( "Index:d",             GetIndex,     SetIndex      );

    //  methods
    method( "Render",              DoRender         );
    method( "RenderTree",          DoRenderTree     );
    method( "InitTree",            InitTree         );
    method( "SaveTree",            SaveTree         );        
} // JObject::Expose

void JObject::SaveTree()
{
    g_pPersistServer->Save( this );
}

void JObject::DoRender()
{
    Render();
    PostRender();
}

void JObject::DoRenderTree()
{
    RenderTree();
}

JMetaClass* JObject::GetMetaClass () const 
{ 
    return s_pMetaClass; 
}

bool JObject::SetProperty( const char* prop, const char* val, int tag )
{
    JClassAttr* pAttr = g_pObjectServer->FindClassAttr( ClassName(), prop );
    if (!pAttr) return false;
    JString sval( val );
    return pAttr->Set( this, sval, tag );
} // JObject::SetProperty

bool JObject::GetProperty( const char* prop, JString& val, int tag )
{
    JClassAttr* pAttr = g_pObjectServer->FindClassAttr( ClassName(), prop );
    if (!pAttr) return false;
    return pAttr->Get( this, val, tag );
} // JObject::GetProperty

bool JObject::CallMethod( const char* method, int tag )
{
    JClassAttr* pAttr = g_pObjectServer->FindClassAttr( ClassName(), method );
    if (!pAttr) 
    {
        return OnSignal( NULL, "", method );
    }
    return pAttr->Call( this, tag );
} // JObject::CallMethod

void JObject::Destroy()
{
    g_pObjectServer->Unregister( this );

    if (JSignalServer::s_pInstance)
    {
        JSignalServer::s_pInstance->ClearSignals( this );
    }

    //  release children in reverse order
    int nCh = m_Children.size();
    for (int i = nCh - 1; i >= 0; i--) 
    {
        JObject* pChild = m_Children[i];
        pChild->SetParent( NULL );
        pChild->Release();
    }
    m_Children.clear();

    //  remove from parent children list
    if (m_pParent)
    {
        for (int i = 0; i < m_pParent->GetNChildren(); i++)
        {
            if (m_pParent->m_Children[i] == this) 
            {
                m_pParent->m_Children.erase( m_pParent->m_Children.begin() + i );
                break;
            }
        }
    }

    JWeakRefReg::s_pInstance->Invalidate( this );
} // JObject::Destroy

void JObject::SetName( const char* name )         
{ 
    if (name == NULL) 
    {
        name = "";
    }
    //  objects in the factory are indexed by their name
    //  therefore, when name is changed, we have to reindex it
    g_pObjectServer->Unregister( this );
    m_Name = name; 
    g_pObjectServer->Register( this );
} // JObject::SetName

int JObject::GetLevel() const
{
    int level = 0;
    JObject* pParent = GetParent();
    while (pParent)
    {
        level++;
        pParent = pParent->GetParent();
    }
    return level;
} // JObject::GetLevel

void JObject::Release()
{
    m_RefCount--;
    if (m_RefCount <= 0)
    {
        m_RefCount = 0;
        delete this;
    }
} // JObject::Release

void JObject::AddRef()
{
    m_RefCount++;
}

int JObject::GetIndex() const
{
    if (!m_pParent)
    {
        return 0;
    }
    return m_pParent->GetChildIndex( this );
}

void JObject::SetIndex( int idx )
{
    if (!m_pParent)
    {
        return;
    }   
    int nCh = m_pParent->GetNChildren();
    idx = clamp( idx, 0, nCh - 1 );
    int curIdx = m_pParent->GetChildIndex( this );
    m_pParent->m_Children.erase( m_pParent->m_Children.begin() + curIdx );
    m_pParent->m_Children.insert( m_pParent->m_Children.begin() + idx, this );
}

void JObject::SwapChildren( int idx1, int idx2 )
{
    if (idx1 < 0 || idx1 >= m_Children.size()) return;
    if (idx2 < 0 || idx2 >= m_Children.size()) return;
    JObject* pTemp = m_Children[idx1];
    m_Children[idx1] = m_Children[idx2];
    m_Children[idx2] = pTemp;
} // JObject::SwapChildren

void JObject::AddChild( JObject* pObj, int idxBefore ) 
{ 
    if (!pObj) return;
    int nChildren = m_Children.size();
    for (int i = 0; i < nChildren; i++)
    {
        if (m_Children[i] == pObj) return;
    }
    if (idxBefore < 0 || idxBefore >= nChildren)
    {
        m_Children.push_back( pObj );
    }
    else
    {
        m_Children.insert( m_Children.begin() + idxBefore, pObj );
    }
    pObj->AddRef();
    pObj->m_pParent = this; 
} // JObject::AddChild

bool JObject::ReplaceChild( JObject* pOld, JObject* pNew )
{
    if (!pOld || pOld == pNew) return false;
    for (int i = 0; i < m_Children.size(); i++)
    {
        if (m_Children[i] == pOld)
        {
            m_Children[i] = pNew;
            pNew->m_pParent = this;
            pOld->m_pParent = NULL;
            pOld->Release();
            pNew->AddRef();
            return true;
        }
    }
    return false;
} // JObject::ReplaceChild

void JObject::RenderTree()
{
    Render();
    //  make another check for visibility, in case 
    //  that Render() has changed object's visibility status
    if (!IsVisible()) 
    {
        PostRender();
        return;
    }

    if (IsDrawBounds())
    {
        DrawBounds();
    }

    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        if (GetChild( i )->IsVisible())
        {
            GetChild( i )->RenderTree();
        }
    }   
    PostRender();
} // JObject::RenderTree

void JObject::SendSignals()
{
    JSignalServer::s_pInstance->SendSignal( this );
}

void JObject::PollSlots()
{
    JSignalServer::s_pInstance->PollSlot( this );
} // JObject::PollSlots

void JObject::SendSignal( const char* name )
{
    JSignalServer::s_pInstance->SendSignal( this, name );
} // JObject::SendSignal

bool JObject::PollSlot( const char* name )
{
    return JSignalServer::s_pInstance->PollSlot( this, name );
} // JObject::PollSlot

bool JObject::HasSymbolicPath( const char* path ) const
{
    if (!path || path[0] == 0) return false;
    const char* pObjName = strrchr( path, '.' );
    if (!pObjName) return (m_Name == path);
    if (!GetParent()) return false;
    JString pName( path, pObjName - path );
    return GetParent()->HasSymbolicPath( pName.c_str() );
} // JObject::HasSymbolicPath 

void JObject::RemoveChildren()
{
    int nCh = m_Children.size();
    for (int i = nCh - 1; i >= 0; i--) 
    {
        m_Children[i]->SetParent( NULL );
        m_Children[i]->Release();
        m_Children[i] = NULL;
    }
    m_Children.clear();
} // JObject::RemoveChildren

void JObject::RemoveChild( JObject* pObj )
{
    if (pObj == NULL) return;
    for (int i = 0; i < GetNChildren(); i++)
    {
        if (m_Children[i] == pObj) 
        {
            pObj->SetParent( NULL );
            m_Children.erase( m_Children.begin() + i );
            pObj->Release();
            return;
        }
    }
} // JObject::RemoveChild

bool JObject::HasParent( JObject* pParent ) const
{
    JObject* pCurParent = GetParent();
    while (pCurParent)
    {
        if (pCurParent == pParent) return true;
        pCurParent = pCurParent->GetParent();
    }
    return false;
} // JObject::HasParent

JObject* JObject::Clone( JObject* pParent, const char* name, bool bCloneSignals ) const
{
    JObject* pRes = g_pObjectServer->CloneObject( this, pParent, name, bCloneSignals );
    if (pRes)
    {
        pRes->InitTree();
    }
    return pRes;
} // JObject::Clone

void JObject::SetTag( int tag, bool bRecursive )
{
    m_Tag = tag;
    if (bRecursive)
    {
        for (int  i = 0; i < GetNChildren(); i++)
        {
            GetChild( i )->SetTag( tag, true );
        }
    }
} // JObject::SetTag

int JObject::GetChildIndex( const JObject* pChild ) const
{
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        if (m_Children[i] == pChild) 
        {
            return i;
        }
    }
    return -1;
} // JObject::GetChildIndex

void JObject::InitTree()
{
    //  init this object
    Init();

    //  perform children's subtrees initialization
    int nCh = GetNChildren();
    for (int i = 0; i < nCh; i++)
    {
        GetChild( i )->InitTree();
    }

    //  connect signals
    JSignalServer::s_pInstance->Connect( this );
} // JObject::InitTree

void JObject::GetPath( JString& path ) const
{
    path = GetName();
    
    JObject* pParent = GetParent();
    while (pParent)
    {
        path.insert( 0, "." );
        path.insert( 0, pParent->GetName() );
        pParent = pParent->GetParent();
    }
} // JObject::GetPath
    



