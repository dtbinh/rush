//****************************************************************************/
//  File:  JWeakRefReg.cpp
//****************************************************************************/
#include "precompile.h"
#include "JWeakRef.h"
#include "JWeakRefReg.h"

JWeakRefReg* JWeakRefReg::s_pInstance = NULL;
//****************************************************************************/
/*  JWeakRefReg implementation
//****************************************************************************/
JWeakRefReg::JWeakRefReg()
{
    assert( s_pInstance == NULL );
    s_pInstance = this;
}

JWeakRefReg::~JWeakRefReg()
{
    s_pInstance = NULL;
}

JWeakRefHolder* JWeakRefReg::GetHolder( void* pObject )
{
    JWeakRefHolderHash::iterator it = m_HolderReg.find( pObject );
    if (it == m_HolderReg.end())
    {
        JWeakRefHolder* pHolder = m_HolderPool.Allocate();
        pHolder->m_pPtr = pObject;
        m_HolderReg[pObject] = pHolder;
        return pHolder;
    }
    return (*it).second;
} // JWeakRefReg::GetHolder

void JWeakRefReg::DeleteHolder( JWeakRefHolder* pHolder )
{
    //  delete from registry
    if (pHolder->GetPtr() != NULL)
    {
        m_HolderReg.erase( pHolder->GetPtr() );
    }
    //  delete from pool
    m_HolderPool.Free( pHolder );    
} // JWeakRefReg::DeleteHolder

void JWeakRefReg::Invalidate( void* pObject )
{
    JWeakRefHolderHash::iterator it = m_HolderReg.find( pObject );
    if (it == m_HolderReg.end())
    {
        return;
    }

    JWeakRefHolder* pHolder = (*it).second;

    //  delete from hash
    m_HolderReg.erase( pObject );

    pHolder->m_pPtr = NULL;
} // JWeakRefReg::Invalidate

int JWeakRefReg::GetNHolders()
{
    return m_HolderPool.GetNElem();
} // JWeakRefReg::GetNHolders