#include "stdafx.h"
/*
#include "ColorSurface.h"

static ColorSurface surf;

TEST( SpriteGetColor )
{
    int spID = g_pDrawServer->GetSpriteID( "sprite_checker" );
    CHECK( spID >= 0 );
    
    const int   c_NumFrames = 8;
    const Frame c_InnerRect( 7, 12, 59, 69 );
    const Frame c_OuterRect( 1, 2, 71, 88 );
    const Vec2  c_Pivot( 41, 43 );
    const Vec2  c_Size( 74, 94 );
    const DWORD c_InnerColor0 = 0xFFFF0000;
    const DWORD c_InnerColor1 = 0x00000000;
    const DWORD c_OuterColor  = 0x000000FF;
    const DWORD c_BorderColor = 0xFFFFFF00;

    CHECK_EQUAL( c_NumFrames, g_pDrawServer->GetNFrames( spID ) );
    bool bRightPivotSize = true;
    for (int i = 0; i < c_NumFrames; i++)
    {
        Vec2 pivot = g_pDrawServer->GetFramePivot( spID, i );
        if (pivot.x != c_Pivot.x || pivot.y != c_Pivot.y) 
        {
            bRightPivotSize = false;
        }
        float w = g_pDrawServer->GetFrameW( spID, i );
        float h = g_pDrawServer->GetFrameH( spID, i );
        if (w != c_Size.x)
        {
            bRightPivotSize = false;      
        }
        if (h != c_Size.y)
        {
            bRightPivotSize = false;      
        }
        //  TODO: Frame Bounds don't make sense
        //Frame ext = g_pDrawServer->GetFrameBounds( spID, i );
        //if (ext.x != 0 || ext.y != 0 || ext.w != w || ext.h != h)
        //{
        //    bRightPivotSize = false;   
        //}
    }
    CHECK_EQUAL( true, bRightPivotSize );
    
    int frameW = g_pDrawServer->GetFrameW( spID, 0 );
    int frameH = g_pDrawServer->GetFrameH( spID, 0 );
    const int c_NumSamples = 100;
    bool bRightColor = true;
    int numCorrect[c_NumFrames];
    for (int i = 0; i < c_NumFrames; i++) numCorrect[i] = 0;
    
    const int tx = int( c_InnerRect.x );
    const int ty = int( c_InnerRect.y );
    for (int i = 0; i < c_NumSamples; i++)
    {
        int cx = rand()%frameW;
        int cy = rand()%frameH;
        DWORD rightClr = c_BorderColor;
        //  find out right color
        if (c_InnerRect.PtIn( float( cx ), float( cy ) ))
        {
            rightClr = ((cx - tx)%2 + (cy - ty)%2 == 1) ? c_InnerColor1 : c_InnerColor0;
        }
        else if (c_OuterRect.PtIn( float( cx ), float( cy ) ))
        {
            rightClr = c_OuterColor;
        }
        for (int j = 0; j < c_NumFrames; j++)
        {
            DWORD clr = g_pDrawServer->GetPixel( spID, j, Vec2( cx, cy ) );
            if (clr == rightClr) 
            {
                numCorrect[j]++;
            }
            else
            {
                bRightColor = false; 
            }
        }
    }    
    CHECK_EQUAL( true, bRightColor );
    CHECK_EQUAL( c_NumSamples, numCorrect[0] );
    CHECK_EQUAL( c_NumSamples, numCorrect[1] );
    CHECK_EQUAL( c_NumSamples, numCorrect[2] );
    CHECK_EQUAL( c_NumSamples, numCorrect[3] );
    CHECK_EQUAL( c_NumSamples, numCorrect[4] );
    CHECK_EQUAL( c_NumSamples, numCorrect[5] );
    CHECK_EQUAL( c_NumSamples, numCorrect[6] );
    CHECK_EQUAL( c_NumSamples, numCorrect[7] );
}
*/
