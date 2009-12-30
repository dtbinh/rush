/***********************************************************************************/
//  File:   PickColorDialog.cpp
//  Date:   27.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "pickcolordialog.h"

//****************************************************************************/
/*    PickColorDialog implementation
//****************************************************************************/
COLORREF PickColorDialog::s_CustomColors[16] =
{
    RGB(0x00, 0x00, 0x00),
    RGB(0xA5, 0x2A, 0x00),
    RGB(0x4B, 0x00, 0x82),
    RGB(0x28, 0x28, 0x28),

    RGB(0xFF, 0x68, 0x20),
    RGB(0x00, 0x93, 0x00),
    RGB(0x38, 0x8E, 0x8E),
    RGB(0x00, 0x00, 0xFF),

    RGB(0xFF, 0x00, 0x00),
    RGB(0x32, 0xCD, 0x32),
    RGB(0x7F, 0xFF, 0xD4),
    RGB(0x80, 0x00, 0x80),

    RGB(0xFF, 0xFF, 0x00),
    RGB(0xC0, 0xC0, 0xC0),
    RGB(0xFF, 0xE4, 0xE1),
    RGB(0xFF, 0xFF, 0xFF)
};

PickColorDialog::PickColorDialog( uint32_t color )
{
    memset( &m_ChooseColor, 0, sizeof(m_ChooseColor) );
    m_ChooseColor.lStructSize = sizeof(m_ChooseColor);
    //  dialog flags
    m_ChooseColor.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
    m_ChooseColor.lpCustColors = s_CustomColors;
    SetColor( color );
}

void PickColorDialog::SetHookProcedure( LPCCHOOKPROC proc )
{
    m_ChooseColor.lpfnHook = proc;
    m_ChooseColor.Flags |= CC_ENABLEHOOK;
}


bool PickColorDialog::Show()
{
    BOOL res = ChooseColor( &m_ChooseColor );
    return (res != FALSE);
}

uint32_t PickColorDialog::FromColorRef( COLORREF clr )
{
    return 0xFF000000 | ((clr & 0x000000FF) << 16) | ((clr & 0x00FF0000) >> 16) | (clr & 0x0000FF00);
}

COLORREF PickColorDialog::ToColorRef( uint32_t clr )
{
    return 0xFF000000 | ((clr & 0x000000FF) << 16) | ((clr & 0x00FF0000) >> 16) | (clr & 0x0000FF00);
}
