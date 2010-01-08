//****************************************************************************/
//  File:  JWeakRefReg.h
//****************************************************************************/
#ifndef __JWeakRefReg_H__
#define __JWeakRefReg_H__

#include <hash_map>

#include "uniformpool.h"

class JWeakRefHolder;

typedef stdext::hash_map<void*, JWeakRefHolder*> JWeakRefHolderHash;
typedef UniformPool<JWeakRefHolder>           JWeakRefHolderPool;
//****************************************************************************/
//  Class:  JWeakRefReg, singleton
//  Desc:   Manages weak references
//****************************************************************************/
class JWeakRefReg
{
public:
                            JWeakRefReg     ();
                            ~JWeakRefReg    ();

    JWeakRefHolder*         GetHolder       ( void* pObject );
    void                    DeleteHolder    ( JWeakRefHolder* pHolder );
    void                    Invalidate      ( void* pObject );
    int                     GetNHolders     ();

    static JWeakRefReg*     s_pInstance;

private:
    JWeakRefHolderHash      m_HolderReg;
    JWeakRefHolderPool      m_HolderPool;

}; // class JWeakRefReg

#endif // __JWeakRefReg_H__


