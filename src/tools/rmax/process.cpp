/***********************************************************************************/
//  File:   process.cpp
//  Desc:   Max scene tree walking related exporter code
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JBone.h"
#include "JModel.h"
#include "JModelAnim.h"
#include "ExportConfig.h"
#include "rmax.h"
#include "maxutils.h"

bool FileExists( const char* path )
{
    FILE* fp = fopen( path, "rb" );
    if (!fp) return false;
    fclose( fp );
    return true;
}

bool RBExport::IsCanceled() const
{
    return (m_bCanceled || (m_bBreakOnError && m_NErrors > 0));
} // RBExport::IsCanceled

ExpNode* RBExport::GetExportedNode( INode* node )
{
    for (int i = 0; i < m_Nodes.size(); i++)
    {
        if (m_Nodes[i].m_pNode == node)
        {
            return &m_Nodes[i];
        }
    }
    return NULL;
} // RBExport::GetExportedNode

void RBExport::Cleanup()
{
    m_NumNodesTotal = 0;
    m_NErrors       = 0;
    m_NWarnings     = 0;
    m_bCanceled     = false;

    m_Nodes.clear();
    m_Materials.clear();

    m_VertexSet.clear();
    m_VertexPool.Clear();
    m_Vertices.clear();
    m_Textures.clear();

    m_Faces.clear();

    if (m_pModel) m_pModel->Release();
    if (m_pAnimation) m_pAnimation->Release();

    m_pModel = NULL;
    m_pAnimation = NULL;
} // RBExport::Cleanup

void RBExport::PreProcessScene()
{
    //  initialize data structures
    Cleanup();
    m_CurTime = m_pInterface->GetTime();
    m_SamplingInterval = 1000.0f/float( GetFrameRate() );

    PreProcessNode( m_pInterface->GetRootNode() );

    //  check for duplicate bone names
    int nNodes = m_Nodes.size();
    for (int i = 0; i < nNodes; i++)
    {
        const char* name = m_Nodes[i].m_pNode->GetName();
        for (int j = i + 1; j < nNodes; j++)
        {
            const char* cname = m_Nodes[j].m_pNode->GetName();
            if (!stricmp( name, cname ))
            {
                Warn( "There are at least two nodes with the name %s. Please, rename it to make it unique.", name );
            }
        }
    }
} // RBExport::PreProcessScene

void RBExport::PreProcessNode( INode* node )
{
    m_NumNodesTotal++;
    
    //  cache node information
    ExpNode expNode;
    expNode.m_Index = m_Nodes.size();
    expNode.m_pNode = node;
    m_Nodes.push_back( expNode );
    m_pCurExpNode = &m_Nodes.back();

    //  gather materials for the current node
    ExtractMaterials( node );
    
    //  preprocess children in the same manner
    for (int i = 0; i < node->NumberOfChildren(); i++) 
    {
        PreProcessNode( node->GetChildNode( i ) );
    }
} // RBExport::PreProcessNode

BOOL RBExport::Process( INode* node ) 
{
    if (IsCanceled())
    {
        return FALSE;
    }
    
    m_pCurExpNode = GetExportedNode( node );
    
    if (!m_pCurExpNode)
    {
        Warn( "Error in scene graph structure. No entry for node: %s", node->GetName() );
        return TREE_CONTINUE;
    }

    m_CurNode++;
    SetProgress( float( m_CurNode )/float( m_NumNodesTotal ) ); 
    
    if (m_bExportSelected && !node->Selected()) 
    {
        return FALSE;
    }

    Spam( "Processing node: %s", node->GetName() );
    Indent();

    const char* pNodeName = node->GetName();
    ExpNode* pExportedNode = GetExportedNode( node );

    //  check if physics object
    const char* c_PhysicsObjectTag = "physics_";
    if (!strncmp( pNodeName, c_PhysicsObjectTag, strlen( c_PhysicsObjectTag )))
    {
        JObject* pPhysObj = ProcessPhysicsObject( node );
        pExportedNode->m_pObject = pPhysObj;
    }
    else
    {
        JBone* pBone = new JBone();
        pBone->SetName( pNodeName );
        Mat4 boneTM = Convert( GetLocalTM( node, m_CurTime ) );

        //  check for the bone scale to be around 1.0f
        const float c_MinScaleFactor = 0.1f;
        const float c_MaxScaleFactor = 100.0f;
        Vec3 scale, trans;
        Mat3 rot;
        boneTM.factorize( scale, rot, trans );
        float scaleFactor = scale.norm();
        if (scaleFactor < c_MinScaleFactor)
        {
            Warn( "Node %s has very small scale factor (%.02f%%). Consider making it close to 100%%.", pNodeName, scaleFactor*100.0f );
        }
        if (scaleFactor > c_MaxScaleFactor)
        {
            Warn( "Node %s has very big scale factor (%.02f%%). Consider making it close to 100%%.", pNodeName, scaleFactor*100.0f );
        }

        pBone->SetTransform( boneTM );

        pExportedNode->m_pObject = pBone;
        ExpNode* pExportedParent = GetExportedNode( node->GetParentNode() );
        if (pExportedParent && pExportedParent->m_pObject)
        {
            pExportedParent->m_pObject->AddChild( pBone );
        }
        else
        {
            m_pModel->AddChild( pBone );
        }

        ObjectState os = node->EvalWorldState( m_CurTime ); 
        if (os.obj) 
        {
            //  look at the super class ID to determine the type of the object.
            DWORD scID = os.obj->SuperClassID();
            DWORD classID = os.obj->ClassID().PartA();
            if (scID == GEOMOBJECT_CLASS_ID && 
                (classID == TRIOBJ_CLASS_ID || 
                 classID == EDITTRIOBJ_CLASS_ID || 
                 classID == POLYOBJ_CLASS_ID))
            {                                              
                Indent();                                  
                ProcessMesh( node ); 
                Indent( false );
            }
            else if (scID == CAMERA_CLASS_ID)
            {
                ProcessCamera( node ); 
            }
            else if (scID == LIGHT_CLASS_ID)
            {
                ProcessLight( node ); 
            }
        }

        //  export animation
        if (m_pConfig->m_bExportAnim)
        {
            ProcessAnimation( node );
        }
    }

    int nChildren = node->NumberOfChildren();
    //  recurse into children
    for (int i = 0; i < nChildren; i++)
    {
        if (!Process( node->GetChildNode( i ) ))
        {
            return FALSE;
        }
    }

    Indent( false );
    
    return TRUE;
} // RBExport::Process

Modifier* RBExport::FindModifier( INode* node, Class_ID modID )
{
    Object* pObj = node->GetObjectRef();
    if (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        IDerivedObject* pDerivedObj = (IDerivedObject*)pObj;
        int nMod = pDerivedObj->NumModifiers();
        for(int i = 0; i < nMod; i++)
        {
            Modifier* pModifier = pDerivedObj->GetModifier(i);
            Class_ID id = pModifier->ClassID(); 
            if (id == modID)
            {
                return pModifier;
            }
        }
    }
    return NULL;
} // RBExport::FindModifier

Matrix3 RBExport::GetLocalTM( INode* node, TimeValue t )
{
    INode* pParent;
    Matrix3 nodeTM, localTM, parentTM, scaleTM;
    scaleTM.IdentityMatrix();
    scaleTM.SetScale( Point3( m_WorldScale, m_WorldScale, m_WorldScale ) );
    
    if (!node) 
    {
        localTM.IdentityMatrix();
        return localTM;
    }
    
    nodeTM = node->GetNodeTM( t );

    pParent = node->GetParentNode();
    if (pParent && !pParent->IsRootNode())
    {
        parentTM = pParent->GetNodeTM( t );
    }
    else
    {
        parentTM = Inverse( scaleTM );
    }
    nodeTM = c_FlipTM*nodeTM;
    parentTM = c_FlipTM*parentTM;

    localTM = nodeTM*Inverse( parentTM );

    //  sanity check for the matrix elemets
    const float c_SanityRange = 1000000.0f;
    for (int i = 0; i < 4; i++)
    {
        //  test for being inside the range
        if (localTM.GetRow( i ).x < -c_SanityRange || localTM.GetRow( i ).x > c_SanityRange ||
            localTM.GetRow( i ).y < -c_SanityRange || localTM.GetRow( i ).y > c_SanityRange ||
            localTM.GetRow( i ).z < -c_SanityRange || localTM.GetRow( i ).z > c_SanityRange)
        {
            Warn( "Node transform matrix has very bigs values. That basically means very big problems." );
        }
        //  test for QNAN
        if (localTM.GetRow( i ).x != localTM.GetRow( i ).x ||
            localTM.GetRow( i ).y != localTM.GetRow( i ).y ||
            localTM.GetRow( i ).z != localTM.GetRow( i ).z)
        {
            Err( "Node transform has not-a-number elements. Please, check your bone setup." );
        }
        
    }
    return localTM;
} // RBExport::GetLocalTM





