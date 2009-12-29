/***********************************************************************************/
//  File:   JBoundsEditor.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JBOUNDSEDITOR_H__
#define __JBOUNDSEDITOR_H__

#include "JWidget.h"
#include "JWeakRef.h"

struct JBoundsEditContext
{
    JWeakRef<JObject>  m_pEdited;
    JWeakRef<JObject>  m_pEditor;
    JString            m_Path;
};

typedef std::vector<JBoundsEditContext> JBoundsEditedList;

/***********************************************************************************/
//  Class:  JBoundsEditor
//  Desc:   Used to edit bounds for the groups of objects
/***********************************************************************************/
class JBoundsEditor : public JWidget
{
public:
                            JBoundsEditor   ();
    virtual void            Render          ();
    virtual void            Init            ();
    void                    SetRootPath     ( const char* objPath );
    const char*             GetRootPath     () const { return m_RootPath.c_str(); }

    void                    ClearEditors    ();
    void                    Refresh         ();

    expose(JBoundsEditor)
    {
        parent( JWidget );
        prop( "RootPath", GetRootPath, SetRootPath );
        field( "RootOnly", m_bRootOnly );
        method( "Refresh", Refresh );
    }
private:
    JObject*                GetEditorsGroup ();
    void                    CreateEditor    ( JObject* pEdited );
    void                    RemoveEditor    ( const char* pEditedPath );

    JString                 m_RootPath;
    JBoundsEditedList       m_Edited;
    bool                    m_bRootOnly;
    JWeakRef<JObject>       m_pRoot;
}; // class JBoundsEditor

#endif //__JBOUNDSEDITOR_H__