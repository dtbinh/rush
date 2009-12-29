//****************************************************************************/
//  File:  PerlinNoise.h
//  Desc:  Perlin Noise generator
//****************************************************************************/
#ifndef __PERLINNOISE_H__
#define __PERLINNOISE_H__
#pragma once

#include "Vec3.h"
#include "Quaternion.h"

static const int s_Permutation[] = 
{       151,160,137,91,90,15,
        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

//****************************************************************************/
//  Class:  PerlinNoise2
//  Desc:   
//****************************************************************************/
class PerlinNoise2 
{
public:

    PerlinNoise2() 
    {
        for (int i = 0; i < 256 ; i++) s_P[256 + i] = s_P[i] = s_Permutation[i];
    }

    double Noise( double x ) 
    {
        return Noise( x, 0.0 );
    } // Noise

    double Noise( double x, double y ) 
    {
        double fx = floor( x );
        double fy = floor( y );

        //  find quad that contains point
        int X = ((int)floorf(x))&255;
        int Y = ((int)floorf(y))&255;              
        
        //  relative x, y of point in quad
        x -= fx;                                
        y -= fy;                                
        
        //  fade curves
        double u = fade( x );
        double v = fade( y );

        //  hash coordinates of the quad corners
        int A  = s_P[X    ] + Y;
        int AA = s_P[A    ];
        int AB = s_P[A + 1];
        int B  = s_P[X + 1] + Y;
        int BA = s_P[B    ];
        int BB = s_P[B + 1];
        
        //  lerp results from quad corners
        double res = lerp( v, lerp( u, grad( s_P[AA], x, y ),     grad( s_P[BA], x - 1, y ) ),
                              lerp( u, grad( s_P[AB], x, y - 1 ), grad( s_P[BB], x - 1, y - 1 ) ) );
        return res;
    } // Noise

    double Noise( double x, double y, double z ) 
    {
        float fx = floorf( x );
        float fy = floorf( y );
        float fz = floorf( z );

        //  find cube that contains point
        int X = ((int)floorf(x))&255;
        int Y = ((int)floorf(y))&255;              
        int Z = ((int)floorf(z))&255;

        //  relative x, y, z of point in cube
        x -= fx;                                
        y -= fy;                                
        z -= fz;

        //  fade curves
        double u = fade( x );
        double v = fade( y );
        double w = fade( z );

        //  hash coordinates of the cube corners
        int A  = s_P[X    ] + Y;
        int AA = s_P[A    ] + Z;
        int AB = s_P[A + 1] + Z;
        int B  = s_P[X + 1] + Y;
        int BA = s_P[B    ] + Z;
        int BB = s_P[B + 1] + Z;

        //  lerp results from cube corners
        double res = 
            lerp( w, lerp( v, lerp( u, grad( s_P[AA], x, y, z ), grad( s_P[BA], x - 1, y, z ) ),
            lerp( u, grad( s_P[AB], x, y - 1, z ),           grad( s_P[BB], x - 1, y - 1, z ) ) ),
            lerp( v, lerp( u, grad( s_P[AA + 1], x, y, z - 1 ),       grad( s_P[BA + 1], x - 1, y, z - 1 ) ),
            lerp( u, grad( s_P[AB + 1], x, y - 1, z - 1 ),   grad( s_P[BB + 1], x - 1, y - 1, z - 1 ) ) ) );
        return res;
    } // Noise

private:
    double fade( double t ) { return t*t*t*(t*(t*6 - 15) + 10); }
    double lerp( double t, double a, double b ) { return a + t*(b - a); }
    double grad( int hash, double x, double y, double z ) 
    {
        int h = hash & 15;                      
        double u = h < 8 ? x : y;                 
        double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    }

    double grad( int hash, double x, double y ) 
    {
        int h = hash & 15;                      
        double u = h < 8 ? x : y;                 
        double v = h < 4 ? y : h == 12 || h == 14 ? x : 0.0;
        return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    }

    int s_P[512];
}; // PerlinNoise2 

#endif // __PERLINNOISE_H__

