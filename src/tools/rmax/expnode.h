/***********************************************************************************/
//  File:   ExpNode.h
//  Desc:   
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __EXPORTEDNODE_H__
#define __EXPORTEDNODE_H__

/***********************************************************************************/
/* Struct:  ExpNode
/* Desc:    Auxiliary structure to contain exported node description
/***********************************************************************************/
struct ExpNode
{
    int                 m_Index;
    INode*              m_pNode;
    JObject*            m_pObject;

    std::vector<int>    m_MtlIdx;
    std::vector<int>    m_BoneIdx;

    ExpNode() : 
        m_Index( -1 ), 
        m_pNode( NULL ), 
        m_pObject( NULL ) 
    {}

    int GetMaterialIdx( int mtlID ) const
    {
        if (m_MtlIdx.size() == 0) return -1;
        return m_MtlIdx[mtlID%m_MtlIdx.size()];
    }

    int GetBoneIdx( int boneID ) const
    {
        if (boneID < 0 || boneID >= m_BoneIdx.size())
        {
            return -1;
        }
        return m_BoneIdx[boneID];
    }

    int AddBoneIdx( int boneIdx )
    {
        int nBones = m_BoneIdx.size();
        for (int i = 0; i < nBones; i++)
        {
            if (m_BoneIdx[i] == boneIdx)
            {
                return i;
            }
        }
        m_BoneIdx.push_back( boneIdx );
        return m_BoneIdx.size() - 1;
    }
}; // struct ExpNode

#endif // __EXPORTEDNODE_H__
