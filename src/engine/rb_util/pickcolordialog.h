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
    PickColorDialog( uint32_t color = 0xFFFFFFFF );

    bool                Show();
    uint32_t               GetColor() const { return FromColorRef( m_ChooseColor.rgbResult ); }
    void                SetColor( uint32_t color ) { m_ChooseColor.rgbResult = ToColorRef( color ); }


    static uint32_t        FromColorRef( COLORREF    clr );
    static COLORREF     ToColorRef    ( uint32_t        clr );

    typedef UINT_PTR (CALLBACK *LPCCHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

    void                SetHookProcedure( LPCCHOOKPROC proc );

protected:
    static COLORREF     s_CustomColors[16];
}; // class PickColorDialog

#endif //__PICKCOLORDIALOG_H__
