/***********************************************************************************/
//  File:   JUpObjectIterator.h
//  Date:   06.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JUPOBJECTITERATOR_H__
#define __JUPOBJECTITERATOR_H__

/***********************************************************************************/
//  Class:  JUpObjectIterator
//  Desc:   
/***********************************************************************************/
class JUpObjectIterator
{
    static const int	c_StackSize = 64;

    JObject*			m_pRoot;
    JObject*			m_pNode;
    JObject*			m_pParent;
    JObject*			m_pChild;

    JObject*			m_Stack		[c_StackSize];
    int					m_IdxStack	[c_StackSize];
    int					m_CurDepth;               
    int					m_CurChildIdx;


public:
                    JUpObjectIterator   ( JObject* pRoot ) : m_pRoot( pRoot ) { reset(); }
    JObject*        operator *          () const { return m_pNode; }
                    operator bool       () const { return (m_pNode != NULL); }
    int	            depth               () const { return m_CurDepth; }
    JObject*        get_parent          () const { return (m_CurDepth > 0) ? m_Stack[m_CurDepth - 1] : m_pParent; }
    void            reset               ()
    {
        m_pNode         = m_pRoot;
        if (!m_pNode) return;
        m_pParent       = m_pNode->GetParent();
        m_pChild        = m_pNode;
        m_CurChildIdx   = -1;
        m_CurDepth      = 0;
    }

    void operator++() 
    {
        depth_next();
    } // operator++

    void depth_next()
    {
        if (!m_pNode) return;
        int nCh = m_pNode->GetNChildren();
        if (nCh == 0) breadth_next();
        else
        {
            m_IdxStack[m_CurDepth] = m_CurChildIdx;
            m_CurChildIdx = 0;
            m_Stack[m_CurDepth] = m_pNode;
            m_CurDepth++;
            m_pNode = m_pNode->GetChild( 0 );
            if (m_pNode == m_pChild) breadth_next();
        }
    } // depth_next

    bool breadth_next()
    {
        JObject* pParent = get_parent();
        if (!pParent && !m_pParent)
        {
            m_pNode = NULL;
            return false;
        }
        if (pParent == m_pParent) 
        {
            m_pChild        = m_pNode;
            m_pNode         = m_pParent;
            m_CurChildIdx   = 0;
            m_CurDepth      = 0;
            m_pParent = m_pNode->GetParent();
            return false;
        }
        int nCh = pParent->GetNChildren();
        m_CurChildIdx++;
        if (m_CurChildIdx >= nCh)
        {
            m_CurDepth--;
            m_CurChildIdx = m_IdxStack[m_CurDepth];
            m_pNode = pParent;
            if (!breadth_next()) return false;
        }
        pParent = get_parent();
        if (!pParent && !m_pParent)
        {
            m_pNode = NULL;
            return false;
        }
        m_pNode = pParent->GetChild( m_CurChildIdx );
        if (m_pNode == m_pChild) breadth_next();
        return true;
    } // breadth_next
}; // class JUpObjectIterator

#endif //__JUPOBJECTITERATOR_H__