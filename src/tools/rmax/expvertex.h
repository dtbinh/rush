/***********************************************************************************/
//  File:   ExpVertex.h
//  Desc:   
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __EXPVERTEX_H__
#define __EXPVERTEX_H__

inline BYTE FToBYTE( float w )
{
    if (w < 0.0f) w = 0.0f;
    if (w > 1.0f) w = 1.0f;
    return BYTE( w*255.0f );
}

const int c_MaxBones = 4;
//****************************************************************************/
//  Class:  ExpVertex
//  Desc:    
//****************************************************************************/
struct ExpVertex  
{
    Vec3        pos;                    //  vertex position 
    Vec3        normal;                 //  vertex normal
    Vec3        tangent;                //  vertex tangent
    Vec3        binormal;               //  vertex binormal
    Vec2        uv;                     //  uv pair
    Vec2        uv2;                    //  second uv pair
    int         mtlID;                  //  material index (in the model material table)
    int         nodeID;                 //  index of the node, containing this mesh
    DWORD       smGroup;                //  smoothing group flagset
    DWORD       color;                  //  vertex color
    int         nBones;                 //  number of skin bones 
    int         boneIdx[c_MaxBones];    //  indices of the skin bones
    float       boneWeight[c_MaxBones]; //  weights of the skin bones

    int         index;                  //  global vertex index
    bool        bVisited;               //  auxiliary boolean flag showing whether vertex was already visited

    int         meshID;                 //  id of the final mesh this vertex gets into

    ExpVertex*  pNext;                  //  pointer to the next vertex in the 0-ring neighborhood 
    ExpVertex*  pBase;                  //  pointer to vertex this one was collapsed to

    //  used as traits for std::set
    struct Less
    {
        bool operator()( ExpVertex* va, ExpVertex* vb ) const
        {   
            return (*va) < (*vb);
        }
    };
    
    //  compares two vertices
    bool operator <( const ExpVertex& v ) const
    {
        if (nodeID  != v.nodeID ) return  (nodeID  < v.nodeID );
        if (mtlID   != v.mtlID  ) return  (mtlID   < v.mtlID  );
        if (color   != v.color  ) return  (color   < v.color  );
        if (nBones  != v.nBones ) return  (nBones  < v.nBones );

        if (!equalf( uv.x,      v.uv.x     )) return (uv.x     < v.uv.x     );
        if (!equalf( uv.y,      v.uv.y     )) return (uv.y     < v.uv.y     );
        if (!equalf( pos.x,     v.pos.x    )) return (pos.x    < v.pos.x    );
        if (!equalf( pos.y,     v.pos.y    )) return (pos.y    < v.pos.y    );
        if (!equalf( pos.z,     v.pos.z    )) return (pos.z    < v.pos.z    );
        if (!equalf( normal.x,  v.normal.x )) return (normal.x < v.normal.x );
        if (!equalf( normal.y,  v.normal.y )) return (normal.y < v.normal.y );
        if (!equalf( normal.z,  v.normal.z )) return (normal.z < v.normal.z );
        if (!equalf( uv2.x,     v.uv2.x    )) return (uv2.x    < v.uv2.x    );
        if (!equalf( uv2.y,     v.uv2.y    )) return (uv2.y    < v.uv2.y    );
        return false;
    }

    ExpVertex() : 
        pos         ( Vec3::null ),       
        normal      ( Vec3::oZ ),
        tangent     ( Vec3::oX ),   
        binormal    ( Vec3::oY ),   
        uv          ( Vec2::null ),        
        uv2         ( Vec2::null ),    
        mtlID       ( -1 ),   
        nodeID      ( -1 ),
        smGroup     ( 0xFFFFFFFF ),
        color       ( 0xFFFFFFFF ),
        nBones      ( 0 ),
        index       ( -1 ),
        pNext       ( NULL ),
        pBase       ( NULL )
    {
        for (int i = 0; i < c_MaxBonesPerVertex; i++)
        {
            boneIdx[i] = -1;
            boneWeight[i] = 0.0f;
        }
    }

    bool AddToZeroRing( ExpVertex* pVert )
    {
        pVert->pNext = pNext;
        pNext = pVert;
        return false;
    }

    //****************************************************************************/
    //  Class:  ZeroRingIterator
    //  Desc:   Iterator to circulate inside vertex zero ring neighbourhood
    //****************************************************************************/
    class ZeroRingIterator
    {
        ExpVertex*  m_pStartVert;
        ExpVertex*  m_pCurVert;
    public:
        ZeroRingIterator( ExpVertex* pVert ) 
            : m_pStartVert( pVert ), m_pCurVert( pVert )
        {}
        inline void operator ++() 
        { 
            m_pCurVert = m_pCurVert->pNext; 
            if (m_pCurVert == m_pStartVert)
            {
                m_pCurVert = NULL;
            }
        }
        inline ExpVertex* operator ->() { return m_pCurVert; }
        inline operator ExpVertex* () { return m_pCurVert; }
    }; // class ZeroRingIterator

    DWORD GetPackedBoneIdx() const
    {
        DWORD res = 0xFFFFFFFF;
        BYTE* pRes = (BYTE*)&res;
        pRes[0] = BYTE( boneIdx[0] );
        pRes[1] = BYTE( boneIdx[1] );
        pRes[2] = BYTE( boneIdx[2] );
        pRes[3] = BYTE( boneIdx[3] );
        return res;
    }

    DWORD GetPackedBoneW() const
    {
        DWORD res = 0xFFFFFFFF;
        BYTE* pRes = (BYTE*)&res;
        pRes[0] = FToBYTE( boneWeight[0] );
        pRes[1] = FToBYTE( boneWeight[1] );
        pRes[2] = FToBYTE( boneWeight[2] );
        pRes[3] = FToBYTE( boneWeight[3] );
        return res;
    }
}; // struct ExpVertex

#endif // __EXPVERTEX_H__
