/***********************************************************************************/
//  File:   PickColorDialog.h
//  Date:   27.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PICKCOLORDIALOG_H__
#define __PICKCOLORDIALOG_H__

#include "windows.h"
#include "commdlg.h"

/***********************************************************************************/
//  Class:  PickColorDialog
//  Desc:   
/***********************************************************************************/
class PickColorDialog
{
    CHOOSECOLOR        m_ChooseColor;

public:
    PickColorDialog( DWORD color = 0xFFFFFFFF );

    bool                Show();
    DWORD               GetColor() const { return FromColorRef( m_ChooseColor.rgbResult ); }
    void                SetColor( DWORD color ) { m_ChooseColor.rgbResult = ToColorRef( color ); }


    static DWORD        FromColorRef( COLORREF    clr );
    static COLORREF     ToColorRef    ( DWORD        clr );

    typedef UINT_PTR (CALLBACK *LPCCHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

    void                SetHookProcedure( LPCCHOOKPROC proc );

protected:
    static COLORREF     s_CustomColors[16];
}; // class PickColorDialog

#endif //__PICKCOLORDIALOG_H__