/***********************************************************************************/
/*  File:   JSlideGame.cpp
/*  Date:   07.08.2006
/*  Author: Juriy Skolyar
/***********************************************************************************/
#include "stdafx.h"
#include "JLocation.h"
#include "JSlidePiece.h"
#include "JSlideGame.h"
#include "ISoundServer.h"

/***********************************************************************************/
/*  JSlideGame implementation
/***********************************************************************************/
decl_class(JSlideGame);
JSlideGame::JSlideGame()
{
    m_NumCycles         = 1;
    m_NumCaught         = 0;
    m_MinSpeed          = 30;
    m_MaxSpeed          = 40; 
    m_TopLine           = -150;
    m_BottomLine        = 768;
    m_LeftLine          = 0;
    m_RightLine         = 1024;
    m_NumID             = 0;
    m_NumToCatch        = 1;
    m_MinToCatch        = 1;
    m_MaxToCatch        = 1;
    m_NumRightCycles    = 3;
    m_CurRightCycle     = 0;
    m_MaxIncorrect      = 3;
    m_NumCaughtWrong    = 0;
    m_bRandomCatchID    = false;
} // JSlideGame::JSlideGame

void JSlideGame::Init()
{
    //  calculate how much different kinds of pieces we have
    m_NumID = -1;
    JObjectIterator it( this );
    while (it)
    {
        JSlidePiece* pPiece = dynamic_cast<JSlidePiece*>( *it );
        ++it;
        if (!pPiece) continue;
        int id = pPiece->GetTag();
        if (id > m_NumID) m_NumID = id;
    }
    m_NumID++;
} // JSlideGame:Init

void JSlideGame::StartCycle()
{
    srand( GetTickCount() );

    if (m_bRandomCatchID)
    {
        //  pick id of piece to catch on this cycle
        //  make it different from previous cycle's id
        int catchID = m_CatchID;
        while (catchID == m_CatchID && m_NumID > 1)
        {
            catchID = rand()%m_NumID;
        }
        m_CatchID = catchID;
    }

    //  pick the number to catch
    int nPieces = 0;
    JObjectIterator it( this );
    while (it)
    {
        JSlidePiece* pPiece = dynamic_cast<JSlidePiece*>( *it );
        ++it;
        if (!pPiece) continue;
        pPiece->StartFalling();
        if (pPiece->GetTag() == m_CatchID) nPieces++;
    }

    SendSignal( "NumToCatch" );
    SendSignal( "CatchID" );
} // JSlideGame::StartCycle

void JSlideGame::StartGame()
{
    m_NumToCatch = m_MinToCatch + rand()%(m_MaxToCatch - m_MinToCatch);
    m_NumCaught  = 0;
    m_CurRightCycle = 0;
    m_NumCycles++;
    StartCycle();
} // JSlideGame::StartGame

int JSlideGame::GetLeftToCatch() const
{
    return m_NumToCatch - m_NumCaught;
} // JSlideGame::GetLeftToCatch

void JSlideGame::Render()
{
    if (m_NumRightCycles == m_CurRightCycle)
    {
        return;
    }

    //  check whether all pieces have fallen
    bool bAllFallen = true;

    JObjectIterator it( this );
    while (it)
    {
        JSlidePiece* pPiece = dynamic_cast<JSlidePiece*>( *it );
        ++it;
        if (!pPiece || !pPiece->IsVisible()) continue;
        if (pPiece->m_Pos.y < m_BottomLine) bAllFallen = false;
    }

    if (m_NumCaughtWrong >= m_MaxIncorrect)
    {
        m_NumCaughtWrong = 0;
        m_NumCaught = 0;
        StartCycle();
    }

    if (m_NumCaught >= m_NumToCatch)
    {
        m_CurRightCycle++;
        m_NumCycles++;
        if (m_NumRightCycles == m_CurRightCycle)
        {
            SendSignal( "OnSuccess" );

            //  stop pieces from falling
            JObjectIterator pit( this );
            while (pit)
            {
                JSlidePiece* pPiece = dynamic_cast<JSlidePiece*>( *pit );
                ++pit;
                if (!pPiece) continue;
                pPiece->SetSpeed( 0.0f );
            }
            return;
        }
        else
        {
            StartCycle();
        }
    }

    if (bAllFallen)
    {
        StartCycle();
        m_NumCycles++;
    }

} // JSlideGame::Render

void JSlideGame::OnTakeRight()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndTakeRight.c_str(), 0 ) );
    m_NumCaught++;
    SendSignal( "OnTakeRight" );
} // JSlideGame::OnTakeRight

void JSlideGame::OnTakeWrong()
{
    g_pSoundServer->Play( g_pSoundServer->GetSoundID( m_SndTakeWrong.c_str(), 0 ) );
    m_NumCaughtWrong++;
    m_NumCaught--;
    if (m_NumCaught < 0)
    {
        m_NumCaught = 0;
    }
    SendSignal( "OnTakeWrong" );
} // JSlideGame::OnTakeWrong