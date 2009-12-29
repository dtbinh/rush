/***********************************************************************************/
/*  File:   JPuzzleGame.cpp
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "JLocation.h"
#include "JPuzzlePiece.h"
#include "JPuzzleGame.h"
#include "ISoundServer.h"

/***********************************************************************************/
/*  JPuzzleGame implementation
/***********************************************************************************/
decl_class(JPuzzleGame);
JPuzzleGame::JPuzzleGame()
{
    m_SnapDist      = 16.0f;
    m_bHomeFreeze   = true;
    m_bDropReturn   = false;
} // JPuzzleGame::JPuzzleGame

void JPuzzleGame::Init()
{
} // JPuzzleGame:Init

void JPuzzleGame::Shuffle()
{
    JObjectIterator it ( this );
    while (it)
    {
        JPuzzlePiece* pPiece = dynamic_cast<JPuzzlePiece*>( *it );
        ++it;
        if (!pPiece) continue;
        pPiece->MoveToInitPos();
    }
} // JPuzzleGame::Shuffle

void JPuzzleGame::Render()
{
    //  check whether all pieces are at home
    bool bAllAtHome = true;

    if (m_Criteria.size() > 0)
    {
        //  check custom criteria
        for (int i = 0; i < m_Criteria.size(); i++)
        {
            int tag = m_Criteria[i].x;
            int num = m_Criteria[i].y;
            if (GetNumAtHome( tag ) != num)
            {
                bAllAtHome = false;
                break;
            }
        }
    }
    else
    {
        JObjectIterator it ( this );
        while (it)
        {
            JPuzzlePiece* pPiece = dynamic_cast<JPuzzlePiece*>( *it );
            ++it;
            if (!pPiece) continue;
            if (!pPiece->IsAtHome() || pPiece->IsDragged()) bAllAtHome = false;
        }
    }

    if (bAllAtHome)
    {
        SendSignal( "OnSuccess" );
    }
} // JPuzzleGame::Render

void JPuzzleGame::OnTakePiece()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndTake.c_str(), 0 ) );
    SendSignal( "PiecePos" );
    SendSignal( "OnTakePiece" );
} // JPuzzleGame::OnTakePiece

void JPuzzleGame::OnDropRight()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndDropRight.c_str(), 0 ) );
    SendSignal( "PiecePos" );
    SendSignal( "OnDropRight" );
} // JPuzzleGame::OnDropRight

void JPuzzleGame::OnDropWrong()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndDropWrong.c_str(), 0 ) );
    SendSignal( "PiecePos" );
    SendSignal( "OnDropWrong" );
} // JPuzzleGame::OnDropWrong

int JPuzzleGame::GetNumAtHome( int tag ) const
{
    int num = 0;
    JObjectIterator it( const_cast<JPuzzleGame*>( this ) );
    while (it)
    {
        JPuzzlePiece* pPiece = dynamic_cast<JPuzzlePiece*>( *it );
        ++it;
        if (!pPiece) continue;
        if (tag != -1 && pPiece->GetTag() != tag) continue;
        if (pPiece->IsAtHome()) num++;
    }
    return num;
} // JPuzzleGame::GetNumAtHome