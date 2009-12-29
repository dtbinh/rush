/***********************************************************************************/
//  File:   mesh.cpp
//  Desc:   Geometry-related exporter part
//  Date:   26.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JMesh.h"
#include "JModel.h"
#include "ExpVertex.h"
#include "ExportConfig.h"
#include "rmax.h"

bool RBExport::MeshModStackIsValid( INode* node )
{
    Object* pObj = node->GetObjectRef();
    if (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        IDerivedObject* pDerivedObj = (IDerivedObject*)pObj;
        int nMod = pDerivedObj->NumModifiers();
        if (nMod == 0) return true;
        if (nMod > 1) return false;
        Class_ID id = pDerivedObj->GetModifier( 0 )->ClassID(); 
        return (id == SKIN_CLASSID || id == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ));
    }
    else
    {
        return true;
    }
    return false;
} // RBExport::MeshModStackIsValid

//  Here we process mesh geometry for given node.
//  Vertices for the geometry from the all meshes of the scene are allocated in 
//  one big vertex pool. One vertex can occur several times in this pool, because
//  we add 3 new vertices for every face.
//  After all meshes in the scene are processed, this pool is collapsed to remove
//  duplicate vertices.
void RBExport::ProcessMesh( INode* node )
{
    if (!node->Renderable() || node->IsNodeHidden())
    {
        return;
    }
    
    ObjectState os      = node->EvalWorldState( m_CurTime );
    Object*     pObject = os.obj;
    if (!pObject) 
    {
        Warn( "Could not evaluate object state in node <%s>. Mesh was skipped.", node->GetName() );
        return;
    }
    Matrix3 nodeTM          = node->GetNodeTM( m_CurTime );
    Matrix3 nodeTMAfterWSM  = node->GetObjTMAfterWSM( m_CurTime );
    Matrix3 mOffs           = nodeTMAfterWSM*Inverse( nodeTM );

    //  triangulate
    TriObject* pTriObj = 0;
    if (pObject->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) )) 
    { 
        pTriObj = (TriObject*)pObject->ConvertToType( m_CurTime, Class_ID( TRIOBJ_CLASS_ID, 0 ) );
    }
    bool bReleaseTriObj = (pTriObj != pObject);
    if (!pTriObj) 
    {
        Warn( "Could not triangulate mesh in node <%s>. Node was skipped", node->GetName() );
        return;
    }

    if (!MeshModStackIsValid( node ))
    {
        Warn( "Modifier stack for node %s should be collapsed or contain only Skin modifier.", 
                node->GetName() );
    }

    //  ensure, that vertex winding direction in polygon is CCW
    Matrix3 objTM = node->GetObjTMAfterWSM( m_CurTime );
    bool bNegScale = (DotProd( CrossProd( objTM.GetRow(0), objTM.GetRow(1) ), objTM.GetRow(2) ) >= 0.0);

    int vx[3];
    if (bNegScale) 
    {
        vx[0] = 0; 
        vx[1] = 1; 
        vx[2] = 2; 
    } 
    else 
    { 
        vx[0] = 2; 
        vx[1] = 1; 
        vx[2] = 0; 
    }

    Mesh& mesh = pTriObj->GetMesh();

    bool bHasTexCoord    = (mesh.numTVerts != 0);
    bool bHasVertexColor = (mesh.numCVerts != 0) && m_pConfig->m_bExportVertexColors;
    bool bHasNormal      = m_pConfig->m_bExportNormals; 
    bool bHasSkin        = ProcessSkin( node );

    //  some cosmetics
    BOOL res = mesh.RemoveDegenerateFaces();
    if (res) 
    {
        Spam( "Degenerate faces were fixed." );
    }

    res = mesh.RemoveIllegalFaces();
    if (res) 
    {
        Spam( "Degenerate indices were fixed." );
    }
    
    ExpNode* pExpNode = GetExportedNode( node );
    if (!pExpNode)
    {
        return;
    }

    int numPri     = mesh.getNumFaces();
    int numVert    = mesh.numVerts;

    //  initialize helper array for building vertices' 0-circles
    VertexPtrArray vertexEntry;
    vertexEntry.resize( numVert );
    memset( &vertexEntry[0], 0, sizeof( ExpVertex* )*numVert );

    Spam( "Original mesh has %d vertices and %d faces.", numVert, numPri );
    bool bHasVoidFaces = false;

    //  loop on mesh faces
    for (int i = 0; i < numPri; i++)
    {
        Face& face = mesh.faces[i];

        //  calculate face normal
        const Point3& v0 = mesh.verts[face.v[vx[0]]]; 
        const Point3& v1 = mesh.verts[face.v[vx[1]]]; 
        const Point3& v2 = mesh.verts[face.v[vx[2]]]; 
        Point3 normal = -Normalize( (v1 - v0)^(v2 - v1) ); 
        normal = mOffs.VectorTransform( normal );
        normal = c_FlipTM.VectorTransform( normal );

        //  loop on face vertices
        ExpVertex* pFaceVertex[3];
        for (int j = 0; j < 3; j++)
        {
            ExpVertex v;
            v.mtlID  = pExpNode->GetMaterialIdx( face.getMatID() );            
            v.nodeID = pExpNode->m_Index;

            if (v.mtlID < 0)
            {
                bHasVoidFaces = true;
            }
            
            //  extract vertex position and apply world-space modifier to it
            int vIdx    = face.v[vx[j]];
            Point3 pt   = mesh.verts[vIdx];
            pt = mOffs.PointTransform( pt );
            pt = c_FlipTM.PointTransform( pt );
            
            v.index = vIdx;
            v.pos   = Vec3( pt.x, pt.y, pt.z );
            //v.pos *= m_WorldScale;

            //  extract skinning info
            if (bHasSkin)
            {
                GetSkinInfo( v );
            }

            //  assign normal
            if (bHasNormal)
            {
                v.normal  = Vec3( normal.x, normal.y, normal.z );
                v.smGroup = face.smGroup;
            }
            
            //  extract vertex colors
            if (bHasVertexColor)    
            {
                const VertColor& vcol = mesh.vertCol[mesh.vcFace[i].t[vx[j]]];
                v.color = ColorToDWORD( Color( vcol.x, vcol.y, vcol.z ) );
            }

            //  extract texture coordinates
            if (bHasTexCoord)   
            {
                const Point3& texCoord = mesh.tVerts[mesh.tvFace[i].t[vx[j]]];
                v.uv.x = texCoord.x;
                v.uv.y = 1.0f - texCoord.y;

                //  second texture coordinate channel
                if (mesh.getNumMaps() > 1 && mesh.mapSupport( 2 ))
                {
                    UVVert* pUVVert = mesh.mapVerts( 2 );
                    TVFace* pTVFace = mesh.mapFaces( 2 );
                    if (pUVVert && pTVFace)
                    {
                        const Point3& tc2 = pUVVert[pTVFace[i].t[vx[j]]];
                        v.uv2.x = tc2.x;
                        v.uv2.y = 1.0f - tc2.y;
                    }
                }
            }

            //  allocate new vertex 
            pFaceVertex[j] = AddVertex( v );

            //  we want vertices in the 0-ring neighborhood to be linked into closed linked list
            if (vertexEntry[vIdx] == NULL) 
            {
                vertexEntry[vIdx] = pFaceVertex[j];
                pFaceVertex[j]->pNext = pFaceVertex[j];
            }
            else
            {
                vertexEntry[vIdx]->AddToZeroRing( pFaceVertex[j] );
            }
        }
        AddPolygon( pFaceVertex[0], pFaceVertex[1], pFaceVertex[2] );

        if (IsCanceled())
        {
            return;
        }
    }

    //  correct normals at vertices corresponding to smoothing groups
    SmoothNormals( vertexEntry );

    //  put vertices into set (this removes duplicate vertices)
    for (int i = 0; i < numVert; i++)
    {
        ExpVertex::ZeroRingIterator it( vertexEntry[i] );
        while (it)
        {
            VertexSet::iterator sIt = m_VertexSet.find( it );
            if (sIt == m_VertexSet.end())
            {
                m_VertexSet.insert( it );
            }
            else
            {
                it->pBase = (*sIt);
            }
            ++it;
        }
    }

    if (bReleaseTriObj) 
    {
        delete pTriObj;
    }

    if (bHasVoidFaces)
    {
        Warn( "Mesh %s has faces with no material assigned.", node->GetName() );
    }
} // RBExport::ProcessMesh

ExpVertex* RBExport::AddVertex( ExpVertex& v )
{
    ExpVertex* pVertex = m_VertexPool.Allocate();
    *pVertex = v;
    pVertex->index = m_VertexPool.GetNElem() - 1;
    return pVertex;
} // RBExport::AddVertex

int RBExport::AddPolygon( ExpVertex* pV0, ExpVertex* pV1, ExpVertex* pV2 )
{
    m_Faces.push_back( ExpFace( pV0, pV1, pV2 ) );
    return m_Faces.size() - 1;
} //RBExport::AddPolygon

bool RBExport::ProcessSkin( INode* node )
{
    m_pSkin         = NULL;
    m_pSkinData     = NULL;
    m_pPhysique     = NULL;

    Modifier* pMod  = NULL;
    //  check for 'Skin' modifier
    pMod = FindModifier( node, SKIN_CLASSID );
    if (pMod)
    {
        m_pSkin = (ISkin*) pMod->GetInterface( I_SKIN );
        if (!m_pSkin) return false;
        m_pSkinData = m_pSkin->GetContextInterface( node );
        //  build bone reindexing table for the skin
        int nBones = m_pSkin->GetNumBones();
        for (int i = 0; i < nBones; i++)
        {
            INode* pBoneNode = m_pSkin->GetBone( i );
            ExpNode* pExportedBone = GetExportedNode( pBoneNode );
            m_pCurExpNode->m_BoneIdx.push_back( pExportedBone->m_Index );
        }
        return true;
    }
    
    
    //  check for physique modifier
    pMod = FindModifier( node, Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) );
    if (pMod)
    {
        IPhysiqueExport* iPExport = (IPhysiqueExport*)pMod->GetInterface( I_PHYINTERFACE );
        if (!iPExport) return false;
        m_pPhysique = (IPhyContextExport*) iPExport->GetContextInterface( node );
        if (!m_pPhysique) return false;
        // convert to rigid with blending
        m_pPhysique->ConvertToRigid ( TRUE );
        m_pPhysique->AllowBlending  ( TRUE );
        return true;
    }
    
    return false;
} // RBExport::ProcessSkin

void RBExport::GetSkinInfo( ExpVertex& v )
{
    //  if mesh is skinned with Character Studio Physique modifier
    if (m_pPhysique)
    {
        GetPhysiqueBinding( v );
        return;
    }

    //  if mesh is skinned with Skin modifier
    if (m_pSkinData && m_pSkin)
    {
        GetSkinBinding( v );
        return;
    }
} // RBExport::GetSkinInfo

void RBExport::GetPhysiqueBinding( ExpVertex& v )
{
    IPhyVertexExport* pExport = (IPhyVertexExport*)m_pPhysique->GetVertexInterface( v.index ); 
    if (!pExport)
    {
        Warn( "Cannot export skinning with Physique modifier." );
        return;
    }

    //  check vertex type (rigid/rigid blended)
    DWORD vType;
    vType = pExport->GetVertexType();
    m_pPhysique->ReleaseVertexInterface( pExport );    

    if (vType == RIGID_TYPE) 
    {
        //  skinned with single bone
        IPhyRigidVertex* pRigidVertex = (IPhyRigidVertex*)m_pPhysique->GetVertexInterface( v.index );
        if (!pRigidVertex)
        {
            Warn( "Cannot export rigid type skinning with Physique modifier." );
            return;
        }
        INode* pBoneNode = pRigidVertex->GetNode();
        if (!pBoneNode) return;
        ExpNode* pExportedBone = GetExportedNode( pBoneNode );
        if (!pExportedBone) return;
        v.boneIdx[0]    = m_pCurExpNode->AddBoneIdx( pExportedBone->m_Index );
        v.nBones        = 1;
        v.boneWeight[0] = 1.0f;
        m_pPhysique->ReleaseVertexInterface( pRigidVertex );
    }
    else if (vType == RIGID_BLENDED_TYPE)
    {
        IPhyBlendedRigidVertex* pBlendedVertex = 
            (IPhyBlendedRigidVertex*) m_pPhysique->GetVertexInterface( v.index );
        if (!pBlendedVertex)
        {
            Warn( "Cannot export blended type skinning with Physique modifier." );
            return;
        }
        v.nBones = pBlendedVertex->GetNumberNodes();
        for (int i = 0; i < v.nBones; i++)
        {
            INode* pBoneNode = pBlendedVertex->GetNode( i );
            ExpNode* pExportedBone = GetExportedNode( pBoneNode );
            v.boneIdx[i]    = m_pCurExpNode->AddBoneIdx( pExportedBone->m_Index );
            v.boneWeight[i] = pBlendedVertex->GetWeight( i );

            /*if (!vert.AddWeight( bIdx, weight ))
            {
                Msg( "Too much weights per vertex.  Skin:%s,  Bone:%s", 
                    curNodeName, boneNode->GetName() );
                vert.LeaveBiggerWeight( bIdx, weight );
                continue;
            }  */         
        }
        m_pPhysique->ReleaseVertexInterface( pBlendedVertex );
    }
    else
    {
        //  incorrect skinned vertex type
        Warn( "Cannot export skinning with Physique modifier: wrong vertex type." );
    }
} // RBExport::GetPhysiqueBinding

void RBExport::GetSkinBinding( ExpVertex& v )
{
    int nWeights = m_pSkinData->GetNumAssignedBones( v.index );
    if (nWeights > c_MaxBones)
    {
        Err( "Skinned mesh %s has vertex(#%d) bound to more than %d bones.", 
            m_pCurExpNode->m_pNode->GetName(), v.index, c_MaxBones );
        nWeights = c_MaxBones;
    }

    for (int i = 0; i < nWeights; i++)
    {
        int boneIdx    = m_pSkinData->GetAssignedBone( v.index, i );
        v.boneIdx[i]   = m_pCurExpNode->m_BoneIdx[boneIdx];
        v.boneWeight[i]= m_pSkinData->GetBoneWeight( v.index, i );
    }
    v.nBones = nWeights;

    if (nWeights == 0)
    {
        Err( "Skinned mesh %s has vertex(#%d) with 0 influences.", m_pCurExpNode->m_pNode->GetName(), v.index );
        //  assign host bone as influence
        v.nBones = 1;
        v.boneIdx[0] = m_pCurExpNode->m_Index;
        v.boneWeight[0] = 1.0f;
    }
} // RBExport::GetSkinBinding

void RBExport::SmoothNormals( VertexPtrArray& vertexEntries )
{
    int nEntries = vertexEntries.size();
    for (int i = 0; i < nEntries; i++)
    {
        //  mark zero-ring as "unvisited"
        ExpVertex::ZeroRingIterator it( vertexEntries[i] );
        while (it)
        {
            it->bVisited = false;
            ++it;
        }
        //  average normals inside vertices which share smoothing groups
        it = ExpVertex::ZeroRingIterator( vertexEntries[i] );
        while (it)
        {   
            ExpVertex* pVertex = it;
            ++it;
            if (pVertex->bVisited)
            {
                continue;
            }
            //  vertex was not visited yet, so calculate average normal 
            Vec3 normal = Vec3::null;
            ExpVertex::ZeroRingIterator vit( pVertex );
            while (vit)
            {
                if (vit->smGroup&pVertex->smGroup)
                {
                    normal += vit->normal;
                }
                ++vit;
            }
            normal.normalize();
            //  assign averaged normal inside all vertices sharing this smoothing 
            vit = ExpVertex::ZeroRingIterator( pVertex );
            while (vit)
            {
                if (vit->smGroup&pVertex->smGroup)
                {
                     vit->normal   = normal;
                     vit->bVisited = true;
                }
                ++vit;
            }
        } 
    }
} // RBExport::SmoothNormals

bool RBExport::InSameMesh( const ExpVertex* pVA, const ExpVertex* pVB )
{
    if (pVA->mtlID != pVB->mtlID) return false;
    if (pVA->nodeID != pVB->nodeID) return false;
    return true;
} // RBExport::InSameMesh

void RBExport::PreprocessVertices()
{
    int nV = m_Vertices.size();
    for (int i = 0; i < nV; i++)
    {
        ExpVertex& v = *m_Vertices[i];
        
    }
} // RBExport::PreprocessVertices

void RBExport::CreateMeshes()
{
    int nVertices = m_VertexSet.size();
    if (m_Faces.size() == 0 || nVertices == 0)
    {
        return;
    }

    //  reindex vertices from the set
    //  they are stored there in already sorted order
    m_Vertices.clear();
    m_Vertices.resize( nVertices );
    VertexSet::iterator it = m_VertexSet.begin();
    int curVert = 0;

    while (it != m_VertexSet.end())
    {
        ExpVertex* pVert = *it;
        m_Vertices[curVert] = pVert;
        pVert->index = curVert++;
        ++it;
    }
    
    PreprocessVertices();

    //  substitute pointers to collapsed vertices in face list 
    int nFaces = m_Faces.size();
    for (int i = 0; i < nFaces; i++)
    {
        ExpFace& face = m_Faces[i];
        if (face.pV0->pBase) face.pV0 = face.pV0->pBase;
        if (face.pV1->pBase) face.pV1 = face.pV1->pBase;
        if (face.pV2->pBase) face.pV2 = face.pV2->pBase;
    }

    //  sort faces corresponding to the new vertex order
    std::sort( m_Faces.begin(), m_Faces.end() );
    
    //  chunkify vertex soup into meshes
    const int c_MaxMeshFaces = 65532/3;
    int curMeshID    = 0;
    int curMeshFaces = 0;
    int minVert      = nVertices;
    int maxVert      = 0;
    int startFace    = 0;
    int startVByte   = 0;
    ExpVertex* pLastVert = m_Faces[0].pV0;
    for (int i = 0; i < nFaces; i++)
    {
        ExpFace& face = m_Faces[i];

        if (!InSameMesh( pLastVert, face.pV0 ) || 
            curMeshFaces >= c_MaxMeshFaces)
        {
            int nVert = maxVert - minVert + 1;
            AddMesh( curMeshID, startVByte, minVert, nVert, startFace, curMeshFaces );

            curMeshID++;
            startFace    += curMeshFaces;
            startVByte   += GetVDecl( minVert, nVert ).m_VertexSize*nVert;
            curMeshFaces = 0;
            pLastVert    = face.pV0;
            minVert      = nVertices;
            maxVert      = 0;
        }
        face.pV0->meshID = curMeshID;
        face.pV1->meshID = curMeshID;
        face.pV2->meshID = curMeshID;

        if (face.pV0->index < minVert) minVert = face.pV0->index;
        if (face.pV1->index < minVert) minVert = face.pV1->index;
        if (face.pV2->index < minVert) minVert = face.pV2->index;

        if (face.pV0->index > maxVert) maxVert = face.pV0->index;
        if (face.pV1->index > maxVert) maxVert = face.pV1->index;
        if (face.pV2->index > maxVert) maxVert = face.pV2->index;

        curMeshFaces++;
    }

    //  last mesh
    AddMesh( curMeshID, startVByte, minVert, maxVert - minVert + 1, startFace, curMeshFaces );
} // RBExport::CreateMeshes

void RBExport::AddMesh( int meshID, int startVByte, int startVert, int nVert, int startFace, int nFaces )
{
    if (nVert == 0) return;

    //  create mesh
    JMesh* pMesh = new JMesh();
    char nameBuf[_MAX_PATH];
    const char* nodeName = m_Nodes[m_Vertices[startVert]->nodeID].m_pNode->GetName();
    sprintf( nameBuf, "%s_mesh", nodeName );

    //  create skin bone list
    std::vector<int> boneReindex;
    boneReindex.resize( m_Nodes.size() );
    std::fill( boneReindex.begin(), boneReindex.end(), -1 );
    int nMeshBones = 0;
    for (int i = 0; i < nVert; i++)
    {
        ExpVertex& v = *m_Vertices[startVert + i];        
        for (int j = 0; j < v.nBones; j++)
        {
            int boneIdx = v.boneIdx[j];
            if (boneIdx == -1) continue;
            if (boneReindex[boneIdx] == -1)
            {
                pMesh->AddBone( m_Nodes[boneIdx].m_pNode->GetName() );
                boneReindex[boneIdx] = nMeshBones;
                nMeshBones++;
            }
            v.boneIdx[j] = boneReindex[boneIdx];
        }
    }
    if (pMesh->GetNSkinBones() > c_MaxBonesPerSkin)
    {
        Err( "Mesh %s exceeds bone limit. Number of ones is %d, maximum allowed is %d.",  
            nodeName, pMesh->GetNSkinBones(), c_MaxBonesPerSkin );
    }

    //  check whether all the references to bones within indices are in correct range
    for (int i = 0; i < nVert; i++)
    {
        const ExpVertex& v = *(m_Vertices[startVert + i]);
        for (int j = 0; j < v.nBones; j++)
        {
            if (v.boneIdx[j] >= 0 && v.boneIdx[j] < pMesh->GetNSkinBones())
            {
                continue;
            }
            Err( "Skin of mesh %s has incorrect bone reference: %d.",  
                    nodeName, v.boneIdx[j] );
        }
    }

    //  create vertex declaration
    VertexDeclaration vertexDecl = GetVDecl( startVert, nVert );
    int vertexStride = vertexDecl.m_VertexSize;

    //  create vertex buffer 
    int nVertBytes = nVert*vertexStride;
    Buffer vbuf( nVertBytes );
    FillVertexBuffer( vbuf.GetData(), vertexDecl, startVert, nVert );
    
    //  add vertices to model
    m_pModel->AddVertices( vbuf.GetData(), nVertBytes );

    int mtlID = m_Vertices[startVert]->mtlID;
    const char* mtlName = "";
    if (mtlID >= 0) mtlName = m_Materials[mtlID]->GetName();

    int nodeID = m_Vertices[startVert]->nodeID;
    JObject* pHostBone = m_Nodes[nodeID].m_pObject;
    
    pMesh->SetName       ( nameBuf );
    pMesh->SetVisible    ( true );
    pMesh->SetVertexRange( startVByte, nVert );
    pMesh->SetIndexRange ( startFace*3, nFaces*3 );
    pMesh->SetVDecl      ( vertexDecl );
    pMesh->SetMaterial   ( mtlName );
    pMesh->SetHostBone   ( pHostBone->GetName() );

    //  add faces to model
    std::vector<WORD> idx;
    int nIdx = 3*nFaces;
    idx.resize( nIdx );
    for (int j = 0; j < nIdx; j += 3)
    {
        ExpFace& face = m_Faces[startFace + j/3];
        idx[j + 0] = face.pV0->index - startVert;
        idx[j + 1] = face.pV1->index - startVert;
        idx[j + 2] = face.pV2->index - startVert;
    }
    m_pModel->AddIndices( &idx[0], nIdx );

    m_pModel->AddChild( pMesh );
} // RBExport::AddMeshData

VertexDeclaration RBExport::GetVDecl( int startVert, int nVert )
{
    bool bIsSkinned = false;
    for (int i = 0; i < nVert; i++)
    {
        if (m_Vertices[i + startVert]->nBones > 0)
        {
            bIsSkinned = true;
        }
    }

    if (bIsSkinned)
    {
        return GetStockVDecl( VertexType_XYZNUVW4 );
    }
    return GetStockVDecl( VertexType_XYZNUV );
} // RBExport::GetVDecl

void RBExport::FillVertexBuffer( BYTE* pVert, const VertexDeclaration& vertexDecl, 
                                 int startVert, int nVert )
{
    int stride = vertexDecl.m_VertexSize;
    //  positions
    BYTE* pV = pVert;
    for (int i = 0; i < nVert; i++)
    {
        *((Vec3*)pV) = m_Vertices[startVert + i]->pos;
        pV += stride;
    }

    //  normals
    int offset = -1;
    int size   = 0;
    if (m_pConfig->m_bExportNormals && 
        vertexDecl.GetElementParam( VertexComponent_Normal, offset, size ))
    {
        pV = pVert + offset;
        for (int i = 0; i < nVert; i++)
        {
            *((Vec3*)pV) = m_Vertices[startVert + i]->normal;
            pV += stride;
        } 
    }

    //  binormals/tangents

    //  vertex colors
    if (m_pConfig->m_bExportVertexColors && 
        vertexDecl.GetElementParam( VertexComponent_Diffuse, offset, size ))
    {
        pV = pVert + offset;
        for (int i = 0; i < nVert; i++)
        {
            *((DWORD*)pV) = m_Vertices[startVert + i]->color;
            pV += stride;
        } 
    }

    //  uv's
    if (vertexDecl.GetElementParam( VertexComponent_TexCoor0, offset, size ))
    {
        pV = pVert + offset;
        for (int i = 0; i < nVert; i++)
        {
            *((Vec2*)pV) = m_Vertices[startVert + i]->uv;
            pV += stride;
        } 
    }

    //  blend weights/indices
    if (vertexDecl.GetElementParam( VertexComponent_Blend, offset, size ))
    {
        pV = pVert + offset;
        for (int i = 0; i < nVert; i++)
        {
            const ExpVertex* curV = m_Vertices[startVert + i];
            *((DWORD*)pV) = curV->GetPackedBoneW();
            *((DWORD*)(pV + 4)) = curV->GetPackedBoneIdx();
            pV += stride;
        } 
    }

} // RBExport::FillVertexBuffer

