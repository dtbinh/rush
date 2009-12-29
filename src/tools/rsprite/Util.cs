using System;
using System.Drawing;
using System.Collections;

namespace rtSprite
{
    /// <summary>
    /// Summary description for Util.
    /// </summary>
    
    public class BitOps
    {
        public static long NextPow2( long n )
        {
            n |= (n >> 1);
            n |= (n >> 2);
            n |= (n >> 4);
            n |= (n >> 8);
            n |= (n >> 16);
            return n + 1;
        } // NextPow2

        public static long PrevPow2( long n )
        {
            if (IsPow2( n )) return n/2;
            return NextPow2( n/2 );
        } // PrevPow2

        public static bool IsPow2( long n )
        {
            return ((n&(n - 1)) == 0);	
        }
    }

    public class Frame
    {
        public int x, y, w, h;

        public Frame(){}
        public Frame( int _x, int _y, int _w, int _h )
        {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
        }
    } // class Frame

    public class RectPartition
    {
        public Frame               Frame;
        public ArrayList           Rects = new ArrayList();
        public const int           c_MinQuadSide = 16;
        public const int           c_MaxQuadSide = 256;
        public static int          MinQuadSide = c_MinQuadSide;
        public static int          MaxQuadSide = c_MaxQuadSide;


        static bool IsRowEmpty( Bitmap bmp, int idx )
        {
            if (idx < 0 || idx >= bmp.Height) return true;
            for (int i = 0; i < bmp.Width; i++) if (bmp.GetPixel( i, idx ).A != 0) return false;
            return true;
        }

        static bool IsColEmpty( Bitmap bmp, int idx )
        {
            if (idx < 0 || idx >= bmp.Width) return true;
            for (int i = 0; i < bmp.Height; i++) if (bmp.GetPixel( idx, i ).A != 0) return false;
            return true;
        }

        //  trims bitmap deleting transparent border
        //  returns trimmed rectangle
        public static Frame TrimBitmap( Bitmap bmp )
        {
            Frame res = new Frame( 0, 0, bmp.Width, bmp.Height );
            while (IsRowEmpty( bmp, res.y ) && res.y < bmp.Height) { res.y++; res.h--; }
            while (IsRowEmpty( bmp, res.y + res.h - 1 ) && res.h > 0) res.h--;
            while (IsColEmpty( bmp, res.x ) && res.w < bmp.Width) { res.x++; res.w--; }
            while (IsColEmpty( bmp, res.x + res.w - 1 ) && res.w > 0) res.w--;
            return res;
        }

        public static byte MaxAlpha( Bitmap bmp, Frame rc )
        {
            int ex = rc.x + rc.w;
            int ey = rc.y + rc.h;
            byte alpha = 0;
            for (int j = rc.y; j < ey; j++)
            {
                for (int i = rc.x; i < ex; i++)
                {
                    byte ca = (byte)((bmp.GetPixel( i, j ).ToArgb()&0xFF000000)>>24);
                    if (ca > alpha) alpha = ca;
                }
            }
            return alpha;
        } // MaxAlpha

        public static byte GetAlpha( int i, int j, Bitmap bmp )
        {
            if (i < 0 || j < 0 || i >= bmp.Width || j >= bmp.Height) return 0;
            return (byte)((bmp.GetPixel( i, j ).ToArgb()&0xFF000000)>>24);
        }

        public static int GetUtilization( Bitmap bmp, Frame rc )
        {
            int ex = rc.x + rc.w;
            int ey = rc.y + rc.h;
            int cFree = 0;
            for (int j = rc.y; j < ey; j++)
            {
                for (int i = rc.x; i < ex; i++)
                {
                    byte ca = GetAlpha( i, j, bmp );
                    if (ca == 0) cFree++;
                }
            }
            return rc.w*rc.h - cFree;
        } // GetUtilization

        public int GetNumPixels( bool bFiltered )
        {
            int nPixels = 0;
            //  count number of bytes to reserve
            for (int k = 0; k < Rects.Count; k++)
            {
                Frame rc = (Frame)Rects[k];
                int bX, bY, eX, eY;
                if (bFiltered)
                {
                    bX = -1; 
                    eX = rc.w + 1;
                    bY = -1; 
                    eY = rc.h + 1;                    
                    if (rc.x + eX > Frame.x + Frame.w + 1) eX = Frame.x + Frame.w - rc.x + 1;
                    if (rc.y + eY > Frame.y + Frame.h + 1) eY = Frame.y + Frame.h - rc.y + 1;
                }
                else
                {
                    bX = 0; 
                    eX = rc.w;
                    bY = 0; 
                    eY = rc.h;
                    if (rc.x + eX > Frame.x + Frame.w) eX = Frame.x + Frame.w - rc.x;
                    if (rc.y + eY > Frame.y + Frame.h) eY = Frame.y + Frame.h - rc.y;
                }	
			    nPixels += (eX - bX)*(eY - bY);
            }

            return nPixels;
        }

        public float GetUtilization( Bitmap bmp )
        {
            int res = 0;
            int total = 0;
            foreach (Frame rc in Rects)
            {
                res += GetUtilization( bmp, rc );
                total += rc.w*rc.h;
            }
            return ((float)res)/((float)total);
        }

        float CreateUniform( Bitmap img, int side )
        {
            Frame rc = TrimBitmap( img );
            int nAllPix = 0;
            int nPix = 0;
            int nx = rc.w/side + 1;
            int ny = rc.h/side + 1;
            Rects.Clear();
            for (int j = 0; j < ny; j++)
            {
                for (int i = 0; i < nx; i++)
                {
                    Frame crc = new Frame( rc.x + i*side, rc.y + j*side, side, side );
                    nAllPix += side*side;
                    int np = GetUtilization( img, crc );
                    if (np == 0) continue;
                    nPix += np;
                    Rects.Add( crc );
                }
            }
            return ((float)nPix)/((float)nAllPix);
        } // CreateUniform
        
        static int GetCoverSide( Frame rc, int inflate )
        {
            long s = 0;
            if (rc.w > rc.h) s = (long)rc.w; else s = (long)rc.h;
            
            if (BitOps.IsPow2( s ) && s <= MaxQuadSide && s >= MinQuadSide)
            {
                return (int)s;
            }

            s += inflate*2;
            s = BitOps.NextPow2( s );
            while (s < MinQuadSide) s *= 2;
            while (s > MaxQuadSide) s /= 2;
            return (int)s;
        }

        const float SubdivUsedBias = 0.75F;
        bool Subdivide( Bitmap img, Frame rc )
        {
            if (rc.w <= MinQuadSide) return false;
            int hw = rc.w/2;
            int hh = rc.h/2;
            Frame[] subr = new Frame[4];
            int[]  nutl  = new int[4];
            subr[0] = new Frame( rc.x, rc.y, hw, hh );
            subr[1] = new Frame( rc.x + hw, rc.y, hw, hh );
            subr[2] = new Frame( rc.x, rc.y + hh, hw, hh );
            subr[3] = new Frame( rc.x + hw, rc.y + hh, hw, hh );
            
            int nPUsed = 0;
            for (int i = 0; i < 4; i++)
            {
                nutl[i] = GetUtilization( img, subr[i] );   
                nPUsed += nutl[i];
            }

            float used = ((float)nPUsed)/(float)(rc.w*rc.w);
            if (used > SubdivUsedBias) return false;
            for (int i = 0; i < 4; i++)
            {
                if (nutl[i] > 0)
                {
                    Rects.Add( subr[i] );
                }
            }

            return true;
        } 

        void CreateUnfiltered( Bitmap img )
        {
            Frame = TrimBitmap( img );
            int side = GetCoverSide( Frame, 0 );
            CreateUniform( img, side );
            bool bWasDivided = true;
            while (bWasDivided)
            {
                bWasDivided = false;
                for (int i = 0; i < Rects.Count; i++) 
                {
                    Frame r = (Frame)Rects[i];
                    bool bSubd = Subdivide( img, r );
                    if (bSubd)
                    {
                        bWasDivided = true;
                        Rects.RemoveAt( i );
                        break;
                    }
                }
            }
        } // CreateUnfiltered

        void CreateFiltered( Bitmap img )
        {
            Frame = TrimBitmap( img );
            int side = GetCoverSide( Frame, 1 );
            CreateUniform( img, side - 2 );
            while (GetUtilization( img ) < SubdivUsedBias && 
                    Rects.Count > 0 &&
                    ((Frame)Rects[0]).w > MinQuadSide)
            {
                side /= 2;
                CreateUniform( img, side - 2 );
            }
        } // CreateFiltered

        public RectPartition()
        {
        }

        public RectPartition( Bitmap img, bool isFiltered )
        {
            if (isFiltered) CreateFiltered( img );
            else CreateUnfiltered( img );
        }
    } // class RectPartition
}
