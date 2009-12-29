/***********************************************************************************/
//  File:   JBoundsEditor.cpp
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JGroup.h"
#include "JObjectIterator.h"
#include "ISignalServer.h"
#include "JBoundsEditor.h"

/***********************************************************************************/
/*  JBoundsEditor implementation
/***********************************************************************************/
decl_class( JBoundsEditor );
JBoundsEditor::JBoundsEditor()
{
    m_pRoot = NULL;
    m_bRootOnly = false;
} // JBoundsEditor::JBoundsEditor

void JBoundsEditor::Render()
{
    //  update editors
    int i = 0;
    while (i < m_Edited.size() )
    {
        JBoundsEditContext& ctx = m_Edited[i];
        if (ctx.m_pEdited == NULL)
        {
            //  edited object was destroyed
            JString path = ctx.m_Path;
            RemoveEditor( path.c_str() );
            //  try to find it by path (it's possible, that it was reloaded)
            JObject* pObj = g_pObjectServer->FindObject( path.c_str() );
            if (pObj)
            {
                CreateEditor( pObj );
            }
            continue;
        }
        
        if (ctx.m_pEditor == NULL)
        {
            //  editor was destroyed
            GetEditorsGroup()->RemoveChild( ctx.m_pEditor );
            m_Edited.erase( m_Edited.begin() + i );
            continue;
        }
        else
        {
            //  hide/show editor according to edited object's visibility
            ctx.m_pEditor->SetVisible( ctx.m_pEdited->IsPathVisible() );
        }
        i++;
    }
} // JBoundsEditor::Render

void JBoundsEditor::Init()
{
    SetRootPath( m_RootPath.c_str() );
}

void JBoundsEditor::Refresh()
{
    JString path = m_RootPath;
    m_RootPath = "";
    SetRootPath( path.c_str() );
}
    
void JBoundsEditor::SetRootPath( const char* objPath )
{
    if (m_RootPath.is_equal_ci( objPath ))
    {
        return;
    }
    m_RootPath = objPath;
    JObject* pObj = g_pObjectServer->FindObject( objPath );
    if (!pObj) 
    {
        return;
    }

    if (!IsVisible())
    {
        return;
    }

    ClearEditors();
    
    if (m_bRootOnly)
    {
        CreateEditor( pObj );
    }
    else
    {
        JObjectIterator it( pObj );
        while (it)
        {
            JObject* pCurObject = *it;
            CreateEditor( pCurObject );
            ++it;
        }
    }
}

void JBoundsEditor::CreateEditor( JObject* pEdited )
{
    JObject* pTemplates = g_pObjectServer->FindObject( "templates", this );
    JObject* pEditors = GetEditorsGroup();
    if (pTemplates == NULL || pEditors == NULL)
    {
        return;
    }
    int nTemplates = pTemplates->GetNChildren();
    JString editorName;
    for (int i = 0; i < nTemplates; i++)
    {
        JWidget* pTemplate = obj_cast<JWidget>( pTemplates->GetChild( i ) );
        if (pTemplate && is_a( pEdited, pTemplate->GetName() ))
        {
            //  this object can be edited with this editor template
            editorName = pEdited->GetName();
            editorName += "_";
            editorName += pTemplate->GetName();
            editorName += "_editor";
            JObject* pEditor = pTemplate->Clone( pEditors, editorName.c_str(), true );

            pEditors->AddChild( pEditor ); 
            
            JBoundsEditContext ctx;
            ctx.m_pEdited = pEdited;
            ctx.m_pEditor = pEditor;
            pEdited->GetPath( ctx.m_Path );
            m_Edited.push_back( ctx );
            
            //  parse property connections from text field
            JString propcfg( pTemplate->GetText() );
            char* propConfig = (char*)propcfg.c_str();
            while (propConfig && *propConfig != 0)
            {
                char* editedVal = propConfig;
                char* editorVal = propConfig + strcspn( propConfig, "<|" );
                char& ch = *editorVal;

                if (ch == '|')
                {
                    ch = 0;
                    editorVal = "value";
                }
                else if (ch == '<')
                {
                    *editorVal = 0;
                    editorVal++; 
                    propConfig = editorVal + strcspn( editorVal, "|" );
                    if (*propConfig == '|')
                    {
                        *propConfig = 0;
                        propConfig++;
                    }
                }
                else if (ch == 0)
                {
                    propConfig = 0;
                    editorVal = "value";
                }

                g_pSignalServer->Connect( pEditor, editorVal, pEdited, editedVal );
                g_pSignalServer->Connect( pEdited, editedVal, pEditor, editorVal );
                pEdited->SendSignal( editedVal ); 

                g_pSignalServer->Connect( pEdited, "visible", pEditor, "visible" );
                pEdited->SendSignal( "visible" );
            }
        }
    }
}

JObject* JBoundsEditor::GetEditorsGroup()
{
    JObject* pEditors = g_pObjectServer->FindObject( "editors", this );
    if (pEditors == NULL)
    {
        pEditors = new JGroup();
        AddChild( pEditors );
        pEditors->SetName( "editors" );
    }
    return pEditors;
}

void JBoundsEditor::ClearEditors()
{
    JObject* pEditors = GetEditorsGroup();
    pEditors->RemoveChildren();
    m_Edited.clear();
}

void JBoundsEditor::RemoveEditor( const char* pEditedPath )
{
    int i = 0;
    while (i < m_Edited.size())
    {
        if (!stricmp( m_Edited[i].m_Path.c_str(), pEditedPath ))
        {
            JObject* pEditor = m_Edited[i].m_pEditor;
            GetEditorsGroup()->RemoveChild( pEditor );
            m_Edited.erase( m_Edited.begin() + i );
            continue;
        }
        ++i;
    }
}