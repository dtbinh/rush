//****************************************************************************/
//  File:  AlphaBetaSearch.h
//  Desc:  
//****************************************************************************/
#ifndef __ALPHABETASEARCH_H__
#define __ALPHABETASEARCH_H__

enum AlphaBetaFlags
{
    AlphaBeta_Pruning       = 0,
    AlphaBeta_IterDeep      = 1,
    AlphaBeta_NegaScout     = 2,
    AlphaBeta_TranspTable   = 4
}; // enum AlphaBetaFlags

//****************************************************************************/
//  Class:  AlphaBetaSearch
//****************************************************************************/
template <class TBoardState, class TMove>
class AlphaBetaSearch
{
    int                  m_MaxDepth;
    bool                 m_bTerminate;
    int                  m_NScanned;
    AlphaBetaFlags       m_Flags;
    TMove                m_BestMove;
    const TMove*         m_TurnsHistory;
    int                  m_NTurns;

public:    
    AlphaBetaSearch() 
        :   m_MaxDepth   ( 4 ),
            m_NScanned   ( 0 ),
            m_bTerminate ( false ),
            m_Flags      ( AlphaBeta_Pruning )
    {}

    void   Terminate    () { m_bTerminate = true; }
    bool   GetBestMove  ( TMove& move ) const 
    { 
        move = m_BestMove;
        return true; 
    }

    void   Search       ( const TBoardState& board, bool bForWhite, 
                            const TMove* turnsHistory = NULL, int nTurns = 0 )
    {
        m_bTerminate    = false;
        m_NScanned      = 0;
        m_TurnsHistory  = turnsHistory;
        m_NTurns        = nTurns;
        // evaluate current board value
        int boardScore = board.GetScore( !bForWhite );
        boardScore = AlphaBeta( board, boardScore, 1, -INT_MAX, INT_MAX, bForWhite, m_BestMove );
    }

    int AlphaBeta( const TBoardState& board, int boardScore, int depth, int alpha, int beta, 
        bool bForWhite, TMove& bestMove )
    {
        m_NScanned++;
        if (depth >= m_MaxDepth || m_bTerminate)
        {
            return boardScore;
        }
        
        // build all possible moves
        TMove moves[TBoardState::c_MaxMoves];
        int nMoves = board.GetMoves( bForWhite, moves, m_NTurns, 
            m_NTurns > 0 ? m_TurnsHistory[m_NTurns - 1] : -1 );
        
        TMove dummyMove;
        TBoardState curBoard( board );
        for (int i = 0; i < nMoves; i++)
        {
            const TMove& m = moves[i];
            curBoard.ApplyMove( bForWhite, m );
            int curBoardScore = curBoard.GetScore( bForWhite );
            int score = -AlphaBeta( curBoard, -curBoardScore, depth + 1, -beta, -alpha, !bForWhite, dummyMove );
            curBoard.UnapplyMove( bForWhite, m );

            if (score > alpha) 
            {
                bestMove = m;
                alpha = score;
            }
            if (score >= beta) 
            {
                return alpha;
            }
            if (m_bTerminate) 
            {
                break;
            }
        }
        return alpha;
    }

}; // class AlphaBetaSearch

#endif // __ALPHABETASEARCH_H__


