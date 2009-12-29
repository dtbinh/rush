/***********************************************************************************/
//  File:   JObjectInspector.h
//  Date:   15.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JOBJECTINSPECTOR_H__
#define __JOBJECTINSPECTOR_H__

#include "JWeakRef.h"
#include "JWidget.h"

/***********************************************************************************/
//  Class:  JObjectInspector
//  Desc:   Represents an editable list of some JObject's exposed properties
/***********************************************************************************/
class JObjectInspector : public JWidget
{
public:
                        JObjectInspector();
    virtual void        Render          ();
    virtual void        PostRender      ();
    virtual void        OnMouse         ( JMouseEvent& e );
    virtual void        Init            ();
    void                SetObject       ( JObject* pObj );
    JObject*            GetObject       () const { return m_pObject; }

    expose(JObjectInspector)
    {
        parent( JWidget );
        prop( "Object", GetObject, SetObject );
        field( "LeftColWidth",      m_LeftColWidth      );
        field( "RightColWidth",     m_RightColWidth     );
        field( "DefaultRowHeight",  m_DefaultRowHeight  );
        field( "EvenRowColor",      m_EvenRowColor      );
        field( "OddRowColor",       m_OddRowColor       );
        field( "LabelsColor",       m_LabelsColor       );
        field( "FontHeight",        m_FontHeight        );
    }  

private:
    int                 m_LeftColWidth;
    int                 m_RightColWidth;
    int                 m_DefaultRowHeight;
    DWORD               m_EvenRowColor;
    DWORD               m_OddRowColor;
    DWORD               m_LabelsColor;
    DWORD               m_SelectionColor;

    int                 m_SelectedIdx;
    int                 m_FontHeight;

    JWeakRef<JObject>   m_pObject;
    JWeakRef<JObject>   m_pEditorsFactory;
    JWeakRef<JObject>   m_pDynamicWidgets;


    JWidget*            CreateEditor    ( const char* className, const char* editorName );
    JWidget*            CreateBlock     ( JMetaClass* pMeta );

}; // class JObjectInspector

#endif //__JOBJECTINSPECTOR_H__