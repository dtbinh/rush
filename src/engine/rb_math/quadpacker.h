/*****************************************************************/
//  File:   QuadPacker.h
/*****************************************************************/
#ifndef __QUADPACKER_H__
#define __QUADPACKER_H__

const uint16_t  c_PowSide[]  = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };
const int   c_MaxQuadLevels = 16; //  maximal powers of m_Quad side in texture layout

struct QuadPos
{
    float x, y;
}; // struct QuadPos

//****************************************************************************/
//  Struct: QuadPackerLevel
//  Desc:   Helper structure for packing sprite chunks on the texture surface
//****************************************************************************/
struct QuadPackerLevel
{
    QuadPos     m_Quad[3];
    uint16_t        m_NQuads;
};  // struct QuadPackerLevel

/****************************************************************************/
//  Class:  QuadPacker                                        
//  Desc:   Describes layout of sprite chunks on the texture surface
/****************************************************************************/
class QuadPacker
{
    QuadPackerLevel         m_Level[c_MaxQuadLevels + 1];
    int                     m_CurLevel;
    int                     m_MaxLevel;
    int                     m_NFreePixels;
    int                     m_MaxPixels;

public:
    QuadPacker      () : m_MaxLevel( c_MaxQuadLevels ) {}
    inline bool AllocateQuad( int sidePow, uint16_t& allocx, uint16_t& allocy )
    {
        if (sidePow > m_CurLevel) return false;

        assert( sidePow <= m_MaxLevel && sidePow > 0 ); 

        QuadPackerLevel* curLevel = &m_Level[sidePow];
        //  no free place - split parent greater free chunks
        if (m_Level[sidePow].m_NQuads == 0) 
        {
            uint16_t idx = sidePow;
            while (m_Level[idx].m_NQuads == 0) 
            {
                idx++;
                if (idx >= c_MaxQuadLevels) return false;
            }

            //  free last chunk
            QuadPackerLevel* parentLevel = &m_Level[idx];
            parentLevel->m_NQuads--;
            const QuadPos& pos = parentLevel->m_Quad[parentLevel->m_NQuads];
            uint16_t x = pos.x;
            uint16_t y = pos.y;
            //  now do descending splitting of the last free chunk on levels
            for (int i = idx - 1; i >= sidePow; i--)
            {
                curLevel = &m_Level[i];
                curLevel->m_NQuads = 3;

                curLevel->m_Quad[0].x = x;
                curLevel->m_Quad[1].x = x;
                x += c_PowSide[i];

                curLevel->m_Quad[2].x = x;

                curLevel->m_Quad[0].y = y;
                curLevel->m_Quad[2].y = y;
                y += c_PowSide[i];

                curLevel->m_Quad[1].y = y;
            }

            //  now do allocate forth chunk
            allocx = x; 
            allocy = y;
        }
        else
        //  there IS free chunk on the level - so allocate it
        {
            curLevel->m_NQuads--;
            const QuadPos& pos = curLevel->m_Quad[curLevel->m_NQuads];
            allocx = pos.x; 
            allocy = pos.y;
        }

        m_NFreePixels -= (int)c_PowSide[sidePow] * (int)c_PowSide[sidePow];
        return true;
    }
    int         NumFreePixels   () const { return m_NFreePixels; }
    void Init( int texSide )
    {
        m_NFreePixels = texSide * texSide;
        m_MaxLevel = 0; m_MaxPixels = m_NFreePixels;
        while (texSide > 1) 
        {
            texSide >>= 1;
            m_MaxLevel++;               
        }
        Free();
    }

    void Free()
    {
        //  make all quads "free"
        memset( &m_Level, 0, sizeof( m_Level ) );
        m_CurLevel = m_MaxLevel;
        m_Level[m_MaxLevel].m_NQuads = 1;
        m_NFreePixels = m_MaxPixels;
    }
}; //  class QuadPacker

#endif // __QUADPACKER_H__