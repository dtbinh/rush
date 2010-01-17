//****************************************************************************/
//  File:   JDialog.h
//  Date:   16.08.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JDIALOG_H__
#define __JDIALOG_H__

#include "jwidget.h"

//****************************************************************************/
//	Enum:  JDialogResult
//  Desc:
//****************************************************************************/
enum JDialogResult
{
    drUnknown       = 0,
    drOK            = 1,
    drCancel        = 2
}; // enum JDialogResult

enum_beg(JDialogResult)
enum_val( "Unknown",        drUnknown   ),
enum_val( "OK",             drOK        ),
enum_val( "Cancel",         drCancel    )
enum_end(JDialogResult)

//****************************************************************************/
//  Class:  JDialog
//  Desc:   Base class for user dialog windows
//****************************************************************************/
class JDialog : public JWidget
{
public:
                        JDialog     ();
    bool                IsModal     () const { return m_bModal; }
    bool                IsFloating  () const { return m_bFloating; }

    void                SetModal    ( bool bModal = true ) { m_bModal = bModal; }
    void                SetFloating ( bool bFloating = true ) { m_bFloating = bFloating; }

    JDialogResult       GetResult   () const { return m_Result; }

    void                OnOK        ();
    void                OnCancel    ();

    void                Show        ();
    void                Close       ();

    virtual void        Render      ();
    virtual void        OnFocus     ( bool bEnter );


    expose( JDialog )
    {
        parent( JWidget );
        field ( "Modal",         m_bModal        );
        field ( "Floating",      m_bFloating     );
        field ( "DefaultFocus",  m_DefaultFocus  );
        method( "OnOK",          OnOK            );
        method( "OnCancel",      OnCancel        );
        method( "Show",          Show            );
        method( "OnShow",        Show            );
        method( "Close",         Close           );
        method( "OnClose",       Close           );
    }

protected:
    virtual void        OnShow     () {}
    virtual void        OnClose    () {}

private:
    bool                m_bModal;       //  dialog disables message passing to another
                                        //  visible dialogs
    bool                m_bFloating;    //  dialog is tool-window like
    JString             m_DefaultFocus; //  default control to focus on when the dialog is shown

    JDialogResult       m_Result;       //  dialog result
    Frame               m_OldViewPort;

}; // class JDialog

#endif // __JDIALOG_H__


