/***********************************************************************************/
//  File:   ModelViewer.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __MODELVIEW_H__
#define __MODELVIEW_H__

#include "jdialog.h"
#include "janimation.h"
#include "jmodelinstance.h"
#include "janiminstance.h"

/***********************************************************************************/
//  Class:  ModelViewer
//  Desc:   
/***********************************************************************************/
class ModelViewer : public JDialog
{
public:
                            ModelViewer     ();
    virtual void            Init            ();
    virtual void            Render          ();
    virtual void            PostRender      ();
    virtual void            OnEvent         ( JEvent* evt );

    void                    Exit            ();

    const char*             GetModel        () const { return m_ModelName.c_str(); }
    const char*             GetAnim         () const { return m_AnimName.c_str(); }

    int                     GetAnimID       () const;
    int                     GetModelID      () const;

    void                    SetModel        ( const char* name );
    void                    SetAnim         ( const char* name );

    void                    SaveModel       ();


    expose(ModelViewer)
    {
        parent(JDialog); 
        prop( "Model", GetModel, SetModel );
        prop( "Animation", GetAnim, SetAnim );
        method( "Exit", Exit );
        method( "SaveModel", SaveModel );
    }

private:
    std::string                 m_ModelName;
    std::string                 m_AnimName;

    JWeakRef<JAnimInstance>     m_pAnimInstance;
    JWeakRef<JModelInstance>    m_pModelInstance;

}; // class ModelViewer

#endif //__ModelView_H__