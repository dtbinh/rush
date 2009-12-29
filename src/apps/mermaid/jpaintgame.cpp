/***********************************************************************************/
/*  File:   JPaintGame.cpp
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JButton.h"
#include "JDialog.h"
#include "JLocation.h"
#include "JPaintArea.h"
#include "JPaintGame.h"

/***********************************************************************************/
/*  JPaintGame implementation
/***********************************************************************************/
decl_class(JPaintGame);
JPaintGame::JPaintGame()
{
    m_NPainted = 0;
    m_NAreas   = 0;
    m_CurColor = 0;
} // JPaintGame::JPaintGame

void JPaintGame::Init()
{
    m_NAreas = 0;
    JObjectIterator it( this );
    while (it)
    {
        if (dynamic_cast<JPaintArea*>( *it ))
        {
            m_NAreas++;
        }
        ++it;
    }
} // JPaintGame:Init

void JPaintGame::Reset()
{
    JObjectIterator it( this );
    while (it)
    {
        JPaintArea* pArea = dynamic_cast<JPaintArea*>( *it );
        if (pArea)
        {
            pArea->SetBgColor( 0 );
        }
        ++it;
    }
} // JPaintGame::Reset

void JPaintGame::Render()
{
    m_NPainted = 0;
    JObjectIterator it( this );
    while (it)
    {
        JPaintArea* pArea = dynamic_cast<JPaintArea*>( *it );
        if (pArea)
        {
            if (pArea->GetColor() != 0)
            {
                m_NPainted++;
            }
        }
        ++it;
    }

    if (m_NPainted == m_NAreas)
    {
        OnSuccess();
        SendSignal( "OnSuccess" );
    }
} // JPaintGame::Render