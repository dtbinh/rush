/***********************************************************************************/
//  File:   JRevObjectIterator.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JREVOBJECTITERATOR_H__
#define __JREVOBJECTITERATOR_H__

//****************************************************************************/
//  Class:  JRevObjectIterator
//  Desc:   
//****************************************************************************/
class JRevObjectIterator 
{
    static const int    c_StackSize = 64;

    JObject*            m_pRoot;
    JObject*            m_pNode;

    JObject*            m_Stack     [c_StackSize];
    int                 m_IdxStack  [c_StackSize];
    int                 m_CurDepth;                 //  length of current node's parental chain
    //  (not including current node)
    int                 m_CurChildIdx;


public:
                    JRevObjectIterator ( JObject* pRoot ) : m_pRoot( pRoot ) { reset(); }
    JObject*        operator *      () const { return m_pNode; }
    JObject*        get_parent      () const { return (m_CurDepth > 0) ? m_Stack[m_CurDepth - 1] : NULL; }
    operator        bool            () const { return (m_pNode != NULL); }
    int             depth           () const { return m_CurDepth; }
    void            reset           ()
    {
        m_pNode         = m_pRoot;
        if (!m_pRoot) return;
        m_CurChildIdx   = m_pRoot->GetNChildren() - 1;
        m_CurDepth      = 0;
        while (depth_next());
        if (m_CurChildIdx == -1) m_pNode = NULL;
    }

    bool depth_next()
    {
        if (!m_pNode) return false;
        int nCh = m_pNode->GetNChildren();
        if (nCh == 0) return false;
        m_IdxStack[m_CurDepth] = m_CurChildIdx;
        m_CurChildIdx = nCh - 1;
        m_Stack[m_CurDepth] = m_pNode;
        m_CurDepth++;
        m_pNode = m_pNode->GetChild( m_CurChildIdx );
        return true;
    } // depth_next

    bool breadth_next()
    {
        JObject* pParent = get_parent();
        if (!pParent) 
        {
            m_pNode = NULL;
            return false;
        }
        int nCh = pParent->GetNChildren();
        m_CurChildIdx--;
        if (m_CurChildIdx < 0)
        {
            m_CurDepth--;
            m_CurChildIdx = m_IdxStack[m_CurDepth];
            m_pNode = pParent;
            if (!breadth_next()) return false;
        }
        pParent = get_parent();
        m_pNode = pParent->GetChild( m_CurChildIdx );
        return true;
    }  

    void operator++() 
    {
        JObject* pParent = get_parent();
        int nCh = pParent->GetNChildren();
        m_CurChildIdx--;
        if (m_CurChildIdx < 0)
        {
            m_CurDepth--;
            m_CurChildIdx = m_IdxStack[m_CurDepth];
            m_pNode = pParent;
            if (m_pNode == m_pRoot) 
            {
                m_pNode = NULL;
                return;
            }
        }
        else
        {
            pParent = get_parent();
            m_pNode = pParent->GetChild( m_CurChildIdx );
            while (depth_next());
        }
    } 

}; // class JRevObjectIterator

#endif //__JREVOBJECTITERATOR_H__