//****************************************************************************/
//  File:  JObjectIterator.h
//****************************************************************************/
#ifndef __JOBJECTITERATOR_H__
#define __JOBJECTITERATOR_H__

#include "JObject.h"

//****************************************************************************/
//  Class:  JObjectIterator
//  Desc:   
//****************************************************************************/
class JObjectIterator 
{
    static const int	c_StackSize = 64;

    JObject*			m_pRoot;
    JObject*			m_pNode;

    JObject*			m_Stack		[c_StackSize];
    int					m_IdxStack	[c_StackSize];
    int					m_CurDepth;                 //  length of current node's parental chain
                                                    //  (not including current node)
    int					m_CurChildIdx;


public:
                    JObjectIterator ( JObject* pRoot ) : m_pRoot( pRoot ) { reset(); }
    void            operator++      () { depth_next(); }
    JObject*        operator *      () const { return m_pNode; }
    JObject*        get_parent      () const { return (m_CurDepth > 0) ? m_Stack[m_CurDepth - 1] : NULL; }
                    operator bool   () const { return (m_pNode != NULL); }
    int	            depth           () const { return m_CurDepth; }
    void            reset           ()
    {
        m_pNode         = m_pRoot;
        m_CurChildIdx   = 0;
        m_CurDepth      = 0;
    }

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
            assert( m_CurDepth < c_StackSize );
            m_CurDepth++;
            m_pNode = m_pNode->GetChild( 0 );
        }
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
        m_CurChildIdx++;
        if (m_CurChildIdx >= nCh)
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

}; // class JObjectIterator

#endif // __JOBJECTITERATOR_H__


