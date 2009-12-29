/***********************************************************************************/
//  File:   JObjectTree.h
//  Date:   14.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JOBJECTTREE_H__
#define __JOBJECTTREE_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JObjectTree
//  Desc:   
/***********************************************************************************/
class JObjectTree : public JWidget
{
    JWeakRef<JObject>    m_pRoot;
    JWeakRef<JObject>    m_pSelected;
    JWeakRef<JObject>    m_pExpanded;

    Vec2                m_DefaultRootPos;
    Vec2                m_RootPos;
    Vec2                m_NodeCaptionPos;

    DWORD               m_ConnectColor;
    float               m_NodeHeight;
    float               m_NodeWidth;

    //  drag-n-dropping
    JObject*            m_pDraggedNode;
    bool                m_bCanDrop;
    bool                m_bClone;
    Vec2                m_DragShift;
    Frame               m_InsExt;
    DWORD               m_InsColor;

public:
                        JObjectTree     ();
    JObject*            GetRoot         () const { return (JObject*)m_pRoot; }
    const char*         GetRootPath     () const;

    JObject*            GetSelected     () const { return (JObject*)m_pSelected; }
    void                SelectObject    ( JObject* pObject );

    void                SetRootPath     ( const char* path );
    void                SetRoot         ( JObject* pRoot );

    virtual void        Render          ();
    
    void                CollapseTo      ( JObject* pObject );

    Vec2                GetRootPos      () const { return m_RootPos; } 
    void                SetRootPos      ( Vec2 pos );

    Vec2                GetDefRootPos   () const { return m_DefaultRootPos; } 
    void                SetDefRootPos   ( Vec2 pos );

    bool                DeleteNode      ();
    bool                MoveNodeUp      ();
    bool                MoveNodeDown    ();
    virtual bool        PtIn            ( int px, int py ) const;

    bool                CanDropNode     ( JObject* pNode, JObject* pDest, bool bAsChild, bool bClone );
    void                DropNode        ( JObject* pNode, JObject* pDest, bool bAsChild, bool bClone );
    void                SaveSelected    ();
    void                LoadAsChild     ();

    expose(JObjectTree)
    {
        parent  (JWidget);
        prop    ( "RootPath",       GetRootPath, SetRootPath       );
        prop    ( "DefaultRootPos", GetDefRootPos, SetDefRootPos   );
        field   ( "NodeCaptionPos", m_NodeCaptionPos               );
        field   ( "ConnectColor",   m_ConnectColor                 );
        field   ( "NodeHeight",     m_NodeHeight                   );
        prop    ( "Selected",       GetSelected, SelectObject      );
        field   ( "NodeWidth",      m_NodeWidth                    );
        field   ( "InsColor",       m_InsColor                     );
        method  ( "SaveSelected",   SaveSelected                   );
        method  ( "LoadAsChild",    LoadAsChild                    );
    }

protected:
    virtual void    OnMouse   ( JMouseEvent& e );
    virtual void    OnDrag    ( JDragEvent& e );
    virtual void    OnKey     ( JKeyEvent& e );

    void            RenderNode( JObject* pObject, const Frame& ext, bool bExpanded ) const;
    JObject*        PickNode  ( float mx, float my, Frame* pExt = NULL ) const;

    typedef bool    (JObjectTree::*TraverseCallback)( const Frame& ext, JObject* pObject, 
                        bool bExpanded, void* pContext ) const;
    void            Traverse        ( TraverseCallback callback, void* pContext = NULL ) const;
    
    struct PickContext
    {
        Vec2        m_PickPos;
        JObject*    m_pPicked;
        Frame       m_PickedExt;
    }; // PickContext

    bool            PickNodeCallback( const Frame& ext, JObject* pObject, bool bExpanded, void* pContext ) const;
    bool            DrawNodeCallback( const Frame& ext, JObject* pObject, bool bExpanded, void* pContext ) const;

}; // class JObjectTree

#endif //__JOBJECTTREE_H__