using System;
using System.Collections;
using System.Drawing;
using System.IO;

namespace rtSprite
{
    public enum ColorFormat
    {
        ARGB4444 = 0,
        ARGB8888 = 1,
        RGB565   = 2,
        RGB888   = 3,
        A8       = 4
    }

    public class Sprite 
    {
        public int             Width;
        public int             Height;

        public short           PivotX;
        public short           PivotY;
        public short           ShiftX;
        public short           ShiftY;

        public Frame            Frame;
        public ColorFormat     ClrFormat;
        public RectPartition   Partition;
        public bool            Filtered;
        public string          Name;
        public bool            Difference;
        public string          FilePath;
        public byte[]          Bytes;
        
        public int GetBytesPerPixel( ColorFormat fmt )
        {
            switch (fmt)
            {
                case ColorFormat.ARGB4444:  return 2;
                case ColorFormat.ARGB8888:  return 4;
                case ColorFormat.RGB565:    return 2;
                case ColorFormat.RGB888:    return 3;
                case ColorFormat.A8:        return 1;     
            }
            return 0;
        }

        public Color GetColor( ref byte[] Bytes, int cByte, ColorFormat fmt )
        {
            byte clrA = 0xFF, clrB = 0xFF, clrR = 0xFF, clrG = 0xFF;
            switch (fmt)
            {
                case ColorFormat.ARGB4444:
                {
                    byte b1 = (byte)Bytes[cByte + 1];
                    byte b2 = (byte)Bytes[cByte];
                    clrA = (byte)(b1&0xF0);
                    clrA |= (byte)(clrA>>4);
                    clrR = (byte)((b1&0x0F)<<4);
                    clrR |= (byte)(clrR>>4);
                    clrG = (byte)(b2&0xF0);
                    clrG |= (byte)(clrG>>4);
                    clrB = (byte)((b2&0x0F)<<4);
                    clrB |= (byte)(clrB>>4);
                }break;
                case ColorFormat.ARGB8888:
                {
                    clrB = (byte)Bytes[cByte];
                    clrG = (byte)Bytes[cByte + 1];
                    clrR = (byte)Bytes[cByte + 2];
                    clrA = (byte)Bytes[cByte + 3];
                }break;
                case ColorFormat.RGB565:
                {
                    byte b1 = (byte)Bytes[cByte + 1];
                    byte b2 = (byte)Bytes[cByte];
                    clrA = 0xFF;
                    clrR = (byte)(b1&0xF8);
                    clrR |= (byte)(clrR>>5);
                    clrG = (byte)(((b1&0x07)<<5)|((b2&0xE0)>>3));
                    clrG |= (byte)(clrG>>6);
                    clrB = (byte)((b2&0x1F)<<3);
                    clrG |= (byte)(clrG>>5);
                }break;  
                case ColorFormat.RGB888:
                {
                    clrA = 0xFF;
                    clrB = (byte)Bytes[cByte];
                    clrG = (byte)Bytes[cByte + 1];
                    clrR = (byte)Bytes[cByte + 2];
                }break; 
                case ColorFormat.A8:
                {
                    clrA = (byte)Bytes[cByte];
                    clrR = 0xFF;
                    clrG = 0xFF;
                    clrB = 0xFF;
                }break;      
            }                        
            return Color.FromArgb( (clrA<<24)|(clrR<<16)|(clrG<<8)|clrB );
        }

        public int AddColor( ref byte[] Bytes, int idx, Color clr, ColorFormat fmt )
        {
            int nc = (int)clr.ToArgb();

            byte a = (byte)((nc&0xFF000000)>>24);
            byte r = (byte)((nc&0x00FF0000)>>16);
            byte g = (byte)((nc&0x0000FF00)>>8);
            byte b = (byte)((nc&0x000000FF));
            switch (fmt)
            {
                case ColorFormat.ARGB4444:
                {
                    byte b1 = (byte)((a&0xF0)|(r>>4));
                    byte b2 = (byte)((g&0xF0)|(b>>4));
                    Bytes[idx] = b2;
                    Bytes[idx + 1] = b1;
                    return 2;
                }
                case ColorFormat.ARGB8888:
                {
                    Bytes[idx] = b;
                    Bytes[idx + 1] = g;
                    Bytes[idx + 2] = r;
                    Bytes[idx + 3] = a;    
                    return 4;
                }
                case ColorFormat.RGB565:
                {
                    byte b1 = (byte)((r&0xF8)|((g&0xFC)>>5));
                    byte b2 = (byte)(((g<<3)&0xE0)|((b&0xF8)>>3));
                    Bytes[idx] = b2;
                    Bytes[idx + 1] = b1;
                    return 2;
                }
                case ColorFormat.RGB888:
                {
                    Bytes[idx] = b;
                    Bytes[idx + 1] = g;
                    Bytes[idx + 2] = r;
                    return 3;
                }
                case ColorFormat.A8:
                {
                    Bytes[idx] = a;
                    return 1;
                } 
            }
            return 0;
        }

        public int  GetNumBytes() 
        {
            int sz = 0;
            int stride = GetBytesPerPixel( ClrFormat );
            for (int i = 0; i < Partition.Rects.Count; i++)
            {
                Frame rc = (Frame)Partition.Rects[i];
                int rcw = rc.w;
                int rch = rc.h;
                if (Frame.x + Frame.w < rc.x + rcw) rcw = Frame.x + Frame.w - rc.x;
                if (Frame.y + Frame.h < rc.y + rch) rch = Frame.y + Frame.h - rc.y;
                if (Filtered) 
                {
                    sz += (rcw + 2)*(rch + 2)*stride;
                }
                else
                {
                    sz += rcw*rch*stride;
                }
            }
            return sz;
        }

        static Color GetPixel( Bitmap bmp, int x, int y )
        {
            if (x < 0) x = 0; 
            if (x >= bmp.Width) x = bmp.Width - 1;
            if (y < 0) y = 0;
            if (y >= bmp.Height) y = bmp.Height - 1;
            return bmp.GetPixel( x, y );   
        }

        public      Sprite()
        {
            Frame = new Frame();
            Partition = new RectPartition();
            Difference = false;
        }

        public      Sprite( string name, string path, Bitmap bmp, ColorFormat fmt, bool bFiltered )
        { 
            Name          = name;
            FilePath      = path;
            Width         = bmp.Width;
            Height        = bmp.Height;
            ClrFormat     = fmt;
            Filtered      = bFiltered;  
            PivotX        = 0;
            PivotY        = 0;
            ShiftX        = 0;
            ShiftY        = 0;
            Difference    = false;

            CreatePartition( bmp );
        }

        void CreatePartition( Bitmap bmp )
        {
            Partition     = new RectPartition( bmp, Filtered );
            Frame         = Partition.Frame;            
            int numBytes  = Partition.GetNumPixels( Filtered )*GetBytesPerPixel( ClrFormat );
            
            //  get pixels
            Bytes = new byte[numBytes];
            int curByte = 0;
            for (int k = 0; k < Partition.Rects.Count; k++)
            {
                Frame rc = (Frame)Partition.Rects[k];
                int bX, bY, eX, eY;
                if (Filtered)
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
				
                for (int j = bY; j < eY; j++)
                {
                    for (int i = bX; i < eX; i++)
                    {
                        Color clr = GetPixel( bmp, rc.x + i, rc.y + j );
                        curByte += AddColor( ref Bytes, curByte, clr, ClrFormat );
                    }
                }
            }
        }

        public void ShrinkTwice()
        {
            Bitmap bmp = GetBitmap( false );
            int nPixels = Width*Height;
            int nW = Width/2;
            int nH = Height/2;
            Bitmap nbmp = new Bitmap( nW, nH );
            unchecked
            {
                for (int x = 0; x < nW; x++)
                {
                    for (int y = 0; y < nH; y++)
                    {
                        Color clr0 = bmp.GetPixel( x*2, y*2 );
                        Color clr1 = bmp.GetPixel( x*2 + 1, y*2 );
                        Color clr2 = bmp.GetPixel( x*2, y*2 + 1 );
                        Color clr3 = bmp.GetPixel( x*2 + 1, y*2 + 1 );

                        Color clr = Color.FromArgb( (clr0.A + clr1.A + clr2.A + clr3.A)/4,
                            (clr0.R + clr1.R + clr2.R + clr3.R)/4,
                            (clr0.G + clr1.G + clr2.G + clr3.G)/4,
                            (clr0.B + clr1.B + clr2.B + clr3.B)/4 );
                        nbmp.SetPixel( x, y, clr );
                    }
                }
            }
            
            Width  = nW;
            Height = nH;
            Bytes = new byte[nW*nH*GetBytesPerPixel( ClrFormat )];
            CreatePartition( nbmp );
        }

        public void Subtract( ref Sprite sp )
        {
            if (sp.Width  != Width || 
                sp.Height != Height)
            {
                return;
            }
            
            Bitmap sbmp = sp.GetBitmap( false );
            Bitmap dbmp = GetBitmap( false );

            int nPixels     = Width*Height;
            int nSame       = 0;
            int nNonTransp  = 0;
            unchecked
            {
                for (int i = 0; i < nPixels; i++)
                {
                    int x = i%Width;
                    int y = i/Width;
                    Color dclr = dbmp.GetPixel( x, y );
                    Color sclr = sbmp.GetPixel( x, y );
                    if (sclr.A != 0)
                    {
                        nNonTransp++;
                    }
                    Color clr = dclr;
                    if (sclr == dclr) 
                    {
                        clr = Color.FromArgb( 0, 0, 0, 0 );
                        nSame++;
                    }
                    sbmp.SetPixel( x, y, clr );
                }
            }
            
            const int c_SubtractBias = 30;
            if (nSame > nNonTransp*c_SubtractBias/100)
            {
                Difference = true;
                CreatePartition( sbmp );
            }
        }

        public void Convert( ColorFormat dstFmt )
        {
            if (dstFmt == ClrFormat) return;

            int srcBPP = GetBytesPerPixel( ClrFormat );
            int dstBPP = GetBytesPerPixel( dstFmt );
            int nPixels = Bytes.GetLength( 0 )/srcBPP;

            byte[] newBytes = new byte[nPixels*dstBPP];
            
            unchecked
            {
                int curByte = 0;
                for (int i = 0; i < nPixels; i++)
                {
                    Color clr = GetColor( ref Bytes, i*srcBPP, ClrFormat );
                    curByte += AddColor( ref newBytes, curByte, clr, dstFmt );
                }
            }

            Bytes = newBytes;
            ClrFormat = dstFmt;
        }
        
        public Bitmap GetBitmap( bool bAlphaOnly )
        {
            Bitmap bm = new Bitmap( Width, Height );
            System.Drawing.Color clrVoid = System.Drawing.Color.Empty;
            int cx = Frame.x;
            int cy = Frame.y;
            int nRects = Partition.Rects.Count;
            int border = 0;
            int cByte  = 0; 
            int stride = GetBytesPerPixel( ClrFormat );
            if (Filtered) border = 1;
            for (int i = 0; i < nRects; i++)
            {
                Frame ext = (Frame)Partition.Rects[i];
                int frw = ext.w;
                int frh = ext.h;
                if (ext.x + frw > Frame.x + Frame.w) frw = Frame.x + Frame.w - ext.x;
                if (ext.y + frh > Frame.y + Frame.h) frh = Frame.y + Frame.h - ext.y;
                cByte += border*(frw + 2)*stride;
                for (int k = 0; k < frh; k++)
                {
                    cByte += border*stride;
                    for (int j = 0; j < frw; j++)
                    {
                        unchecked
                        {
                            byte clrA = 0xFF, clrB = 0xFF, clrR = 0xFF, clrG = 0xFF;
                            switch (ClrFormat)
                            {
                                case ColorFormat.ARGB4444:
                                {
                                    byte b1 = (byte)Bytes[cByte + 1];
                                    byte b2 = (byte)Bytes[cByte];
                                    clrA = (byte)(b1&0xF0);
                                    clrR = (byte)((b1&0x0F)<<4);
                                    clrG = (byte)(b2&0xF0);
                                    clrB = (byte)((b2&0x0F)<<4);
                                }break;
                                case ColorFormat.ARGB8888:
                                {
                                    clrB = (byte)Bytes[cByte];
                                    clrG = (byte)Bytes[cByte + 1];
                                    clrR = (byte)Bytes[cByte + 2];
                                    clrA = (byte)Bytes[cByte + 3];
                                }break;
                                case ColorFormat.RGB565:
                                {
                                    byte b1 = (byte)Bytes[cByte + 1];
                                    byte b2 = (byte)Bytes[cByte];
                                    clrA = 0xFF;
                                    clrR = (byte)(b1&0xF8);
                                    clrG = (byte)(((b1&0x07)<<5)|((b2&0xE0)>>3));
                                    clrB = (byte)((b2&0x1F)<<3);
                                }break;  
                                case ColorFormat.RGB888:
                                {
                                    clrA = 0xFF;
                                    clrB = (byte)Bytes[cByte];
                                    clrG = (byte)Bytes[cByte + 1];
                                    clrR = (byte)Bytes[cByte + 2];
                                }break; 
                                case ColorFormat.A8:
                                {
                                    clrA = (byte)Bytes[cByte];
                                    clrR = 0xFF;
                                    clrG = 0xFF;
                                    clrB = 0xFF;
                                }break;      
                            }
                        
                            if (bAlphaOnly)
                            {
                                bm.SetPixel( ext.x + j, ext.y + k, Color.FromArgb( (255<<24)|(clrA<<16)|(clrA<<8)|clrA ) );
                            }
                            else
                            {
                                bm.SetPixel( ext.x + j, ext.y + k, GetColor( ref Bytes, cByte, ClrFormat ) );
                            }
                            cByte += stride;
                        }
                    }
                    cByte += border*stride;
                }
                cByte += border*(frw + 2)*stride;
            }
            
            return bm;
        }

        public int  GetSizeBytes() { return Bytes.GetLength( 0 ); }

    }


    public class SpritePackage
    {
        public ArrayList Sprites = new ArrayList();
        
        public int Count
        {
            get { return Sprites.Count; }
        }
        
        public void RemoveAt( int idx ){ Sprites.RemoveAt( idx ); }
        public void Insert( int idx, object spr ) { Sprites.Insert( idx, (Sprite)spr ); }
        public Sprite GetSprite( int idx ) { return (Sprite)Sprites[idx]; }

        public class SpriteCmp : IComparer  
        {
            int IComparer.Compare( Object x, Object y )  
            {
                string xs = ((Sprite)x).Name;
                string ys = ((Sprite)y).Name;
                return( xs.CompareTo( ys ) );
            }
        }

        public void Sort()
        {
            SpriteCmp cmp = new SpriteCmp();
            Sprites.Sort( cmp );    
        }

        public void Clear()
        {
            Sprites.Clear();    
        }
        
        public void WriteWord( Stream os, int w )
        {
            os.WriteByte( (byte)(w&0x00FF) );
            os.WriteByte( (byte)((w&0xFF00)>>8) );
        }

        public void WriteDword( Stream os, int n )
        {
            WriteWord( os, (int)(n&0x0000FFFF) );
            WriteWord( os, (int)((n&0xFFFF0000)>>16) ); 
        }

        public int ReadWord( Stream s )
        {
            int lb = s.ReadByte();
            int hb = s.ReadByte();
            return (hb<<8)|lb;
        }

        public int ReadDword( Stream s )
        {
            int lw = ReadWord( s );
            int hw = ReadWord( s );
            return (hw<<16)|lw;
        }

        public void Write( Stream os )
        {
            os.WriteByte( (byte)'R' );
            os.WriteByte( (byte)'B' );
            os.WriteByte( (byte)'S' );
            os.WriteByte( (byte)'P' );

            WriteWord ( os, Sprites.Count );
            WriteDword( os, 0 );
            WriteDword( os, 0 );
            //  write header
            for (int i = 0; i < Sprites.Count; i++)
            {
                Sprite sp = (Sprite)Sprites[i];
                WriteWord ( os, sp.Width            );
                WriteWord ( os, sp.Height           );
                WriteWord ( os, (int)sp.ClrFormat   );
                WriteWord ( os, sp.Frame.x          );
                WriteWord ( os, sp.Frame.y          );
                WriteWord ( os, sp.Frame.w          );
                WriteWord ( os, sp.Frame.h          );
                int flags = 0;
                if (sp.Filtered)   flags |= 0x1;
                if (sp.Difference) flags |= 0x2;
                WriteWord ( os, flags );
                WriteWord ( os, sp.Partition.Rects.Count );
                WriteWord ( os, (int)sp.PivotX      );      
                WriteWord ( os, (int)sp.PivotY      );
                WriteWord ( os, (int)sp.ShiftX      );
                WriteWord ( os, (int)sp.ShiftY      );
                
                foreach (Frame rc in sp.Partition.Rects)
                {
                    WriteWord( os, rc.x );
                    WriteWord( os, rc.y );
                    WriteWord( os, rc.w );
                    WriteWord( os, 0 );
                }                
            }

            //  write pixels
            for (int i = 0; i < Sprites.Count; i++)
            {
                Sprite sp = (Sprite)Sprites[i];
                os.Write( sp.Bytes, 0, sp.Bytes.GetLength( 0 ));
            }
        }

        public void Read( Stream ins, String PackName )
        {
            if (ins.ReadByte() != (byte)'R' ) return;
            if (ins.ReadByte() != (byte)'B' ) return;
            if (ins.ReadByte() != (byte)'S' ) return;
            if (ins.ReadByte() != (byte)'P' ) return;

            int nSprites = ReadWord( ins );
            ReadDword( ins );
            ReadDword( ins );

            int firstSprite = Sprites.Count;

            //  read header
            for (int i = 0; i < nSprites; i++)
            {
                Sprite sp = new Sprite();
                sp.Width = ReadWord( ins );
                sp.Height = ReadWord( ins );
                sp.ClrFormat = (ColorFormat)ReadWord( ins );
                sp.Frame.x = ReadWord( ins );
                sp.Frame.y = ReadWord( ins );
                sp.Frame.w = ReadWord( ins );
                sp.Frame.h = ReadWord( ins );
                sp.Name    = PackName + i;
                int bFiltered = ReadWord( ins );
                if (bFiltered == 1) sp.Filtered = true; else sp.Filtered = false;
                int nRects = ReadWord( ins );
                sp.PivotX  = (short)ReadWord( ins );
                sp.PivotY  = (short)ReadWord( ins );
                sp.ShiftX  = (short)ReadWord( ins );
                sp.ShiftY  = (short)ReadWord( ins );
                
                for (int j = 0; j < nRects; j++)
                {
                    Frame rc = new Frame();
                    rc.x = ReadWord( ins );
                    rc.y = ReadWord( ins );
                    rc.w = ReadWord( ins );
                    rc.h = rc.w;
                    ReadWord( ins );
                    sp.Partition.Rects.Add( rc );
                }   
                Sprites.Add( sp );
            }

            //  read pixels
            for (int i = firstSprite; i < Sprites.Count; i++)
            {
                Sprite sprite = (Sprite)Sprites[i];
                int nBytes = sprite.GetNumBytes();
                sprite.Bytes = new byte[nBytes];
                ins.Read( sprite.Bytes, 0, nBytes );
            }
        }

        public void Convert( ColorFormat dstFmt )
        {
            foreach (Sprite sp in Sprites)
            {
                sp.Convert( dstFmt );
            }
        }

        public void ResetPivots()
        {
            foreach (Sprite sp in Sprites)
            {
                sp.PivotX = 0;
                sp.PivotY = 0;
                sp.ShiftX = 0;
                sp.ShiftY = 0;
            }
        }
    }
}
