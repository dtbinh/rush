/***********************************************************************************/
/*  File:   JBlotGame.cpp
/*  Date:   01.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#include "stdafx.h"
#include "JPicture.h"
#include "Timer.h"
#include "JRipplePicture.h"
#include "JLocation.h"
#include "JMathTypeCast.h"
#include "JBlotPiece.h"
#include "JBlotGame.h"
#include "ISoundServer.h"

/***********************************************************************************/
/*  JBlotGame implementation
/***********************************************************************************/
decl_class(JBlotGame);
JBlotGame::JBlotGame()
{
    m_pSelected1    = NULL;
    m_pSelected2    = NULL;
} // JBlotGame::JBlotGame

void JBlotGame::Init()
{
    m_BlotsCount = 0;
    JObjectIterator it ( this );
    while (it)
    {
        JBlotPiece* pPiece = dynamic_cast<JBlotPiece*>( *it );
        ++it;
        if (!pPiece) continue;
        m_BlotsCount++;
    }
} // JBlotGame:Init

JBlotPiece* JBlotGame::GetPiece( int idx )
{
    JObjectIterator it( this );
    while (it)
    {
        JBlotPiece* pPiece = dynamic_cast<JBlotPiece*>( *it );
        ++it;
        if (pPiece)
        {
            if (idx == 0)
            {
                return pPiece;
            }
            idx--;
        }
    }
    return NULL;
} // JBlotGame::GetPiece

void JBlotGame::Shuffle()
{
    JObjectIterator it ( this );
    while (it)
    {
        JBlotPiece* pPiece = dynamic_cast<JBlotPiece*>( *it );
        ++it;
        if (!pPiece) continue;
        pPiece->MoveToHomePos();
    }

    srand( GetTickCount() );
    do {
        int nSwaps = m_BlotsCount;
        for (int i = 0; i < nSwaps; i++)
        {
            JBlotPiece* pPiece1 = GetPiece( (int)rand()%m_BlotsCount );
            JBlotPiece* pPiece2 = GetPiece( (int)rand()%m_BlotsCount );
            if ( pPiece1 && pPiece1 != pPiece2)
            {
                SwapPieces( pPiece1, pPiece2, true );
            }
        }
    } while (AllAtHome());
} // JBlotGame::Shuffle

void JBlotGame::Render()
{
    if ((m_pSelected1 != NULL)&&(m_pSelected2 != NULL))
    {
        SwapPieces( m_pSelected1 , m_pSelected2, false );
        m_pSelected1->m_bSelected   = false;
        m_pSelected2->m_bSelected   = false;
        m_pSelected1                = NULL;
        m_pSelected2                = NULL;
    }

    if (AllAtHome())
    {
        SendSignal( "OnSuccess" );
    }
} // JBlotGame::Render

bool JBlotGame::AllAtHome() 
{
    //  check whether all pieces are at home
    bool bAllAtHome     = true;

    JObjectIterator it ( this );
    
    while (it)
    {
        JBlotPiece* pPiece = dynamic_cast<JBlotPiece*>( *it );
        ++it;
        if (!pPiece) continue;
        if (!pPiece->IsAtHome()) bAllAtHome = false;
    }

    return bAllAtHome;
} // JBlotGame::AllAtHome

void JBlotGame::SwapPieces( JBlotPiece* p1, JBlotPiece* p2, bool bImmediately )
{
    if (p1 == p2) return;

    Vec2 pos1 = p1->m_MovePos;
    Vec2 pos2 = p2->m_MovePos;

    //Move first piece to place of the second
    p1->MoveTo( pos2, bImmediately );

    //Move second piece to place of the first
    p2->MoveTo( pos1, bImmediately );

} // JBlotGame::SwapPieces

void JBlotGame::OnSelectPiece()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndSelect.c_str(), 0 ) );
    SendSignal( "PiecePos" );
    SendSignal( "OnSelectPiece" );
} // JBlotGame::OnSelectPiece