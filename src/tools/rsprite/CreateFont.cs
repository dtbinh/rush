using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.Drawing.Drawing2D;
using System.IO;

namespace rtSprite
{
	public class CreateFont : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Button         SelFont;
        private System.Windows.Forms.Label          label1;
        private System.Windows.Forms.Button         PrevSym;
        private System.Windows.Forms.Button         NextSym;
        private System.Windows.Forms.NumericUpDown  BorderSize;
		private System.ComponentModel.Container     components = null;
        private System.Windows.Forms.Label          FontName;
        public Font                                 CurFont;
        private System.Windows.Forms.PictureBox     CharBox;
        private System.Windows.Forms.Label          FontSize;
		private int                                 CurChar = -1;
        private System.Windows.Forms.Label          CharIdx;
        private System.Windows.Forms.CheckBox       DoAntialias;
        private System.Windows.Forms.Button         ExportBitmap;
        private System.Windows.Forms.Button         ImportBitmap;
        private int                                 MinChar = 32;
        private int                                 MaxChar = 255;
        private ArrayList                           Chars = new ArrayList();
        private System.Windows.Forms.ProgressBar    ImportProgress;
        private Bitmap                              Bmp = new Bitmap( 256, 256, PixelFormat.Format32bppArgb );

        public CreateFont()
		{
			InitializeComponent();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.SelFont = new System.Windows.Forms.Button();
            this.ExportBitmap = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.CharBox = new System.Windows.Forms.PictureBox();
            this.PrevSym = new System.Windows.Forms.Button();
            this.NextSym = new System.Windows.Forms.Button();
            this.BorderSize = new System.Windows.Forms.NumericUpDown();
            this.FontName = new System.Windows.Forms.Label();
            this.FontSize = new System.Windows.Forms.Label();
            this.CharIdx = new System.Windows.Forms.Label();
            this.DoAntialias = new System.Windows.Forms.CheckBox();
            this.ImportBitmap = new System.Windows.Forms.Button();
            this.ImportProgress = new System.Windows.Forms.ProgressBar();
            ((System.ComponentModel.ISupportInitialize)(this.BorderSize)).BeginInit();
            this.SuspendLayout();
            // 
            // SelFont
            // 
            this.SelFont.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SelFont.Location = new System.Drawing.Point(10, 207);
            this.SelFont.Name = "SelFont";
            this.SelFont.Size = new System.Drawing.Size(177, 19);
            this.SelFont.TabIndex = 0;
            this.SelFont.Text = "Select Font...";
            this.SelFont.Click += new System.EventHandler(this.SelFont_Click);
            // 
            // ExportBitmap
            // 
            this.ExportBitmap.Enabled = false;
            this.ExportBitmap.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ExportBitmap.Location = new System.Drawing.Point(10, 234);
            this.ExportBitmap.Name = "ExportBitmap";
            this.ExportBitmap.Size = new System.Drawing.Size(177, 20);
            this.ExportBitmap.TabIndex = 1;
            this.ExportBitmap.Text = "Export Bitmap...";
            this.ExportBitmap.Click += new System.EventHandler(this.ExportBitmap_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(8, 175);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 16);
            this.label1.TabIndex = 3;
            this.label1.Text = "Border:";
            // 
            // CharBox
            // 
            this.CharBox.BackColor = System.Drawing.Color.Black;
            this.CharBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.CharBox.Location = new System.Drawing.Point(10, 6);
            this.CharBox.Name = "CharBox";
            this.CharBox.Size = new System.Drawing.Size(177, 138);
            this.CharBox.TabIndex = 4;
            this.CharBox.TabStop = false;
            this.CharBox.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);
            // 
            // PrevSym
            // 
            this.PrevSym.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.PrevSym.Location = new System.Drawing.Point(10, 148);
            this.PrevSym.Name = "PrevSym";
            this.PrevSym.Size = new System.Drawing.Size(70, 18);
            this.PrevSym.TabIndex = 5;
            this.PrevSym.Text = "<";
            this.PrevSym.Click += new System.EventHandler(this.PrevSym_Click);
            // 
            // NextSym
            // 
            this.NextSym.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.NextSym.Location = new System.Drawing.Point(117, 148);
            this.NextSym.Name = "NextSym";
            this.NextSym.Size = new System.Drawing.Size(70, 18);
            this.NextSym.TabIndex = 6;
            this.NextSym.Text = ">";
            this.NextSym.Click += new System.EventHandler(this.NextSym_Click);
            // 
            // BorderSize
            // 
            this.BorderSize.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(255)), ((System.Byte)(224)), ((System.Byte)(192)));
            this.BorderSize.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.BorderSize.Location = new System.Drawing.Point(52, 167);
            this.BorderSize.Name = "BorderSize";
            this.BorderSize.Size = new System.Drawing.Size(37, 20);
            this.BorderSize.TabIndex = 7;
            this.BorderSize.ValueChanged += new System.EventHandler(this.BorderSize_ValueChanged);
            // 
            // FontName
            // 
            this.FontName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FontName.Location = new System.Drawing.Point(10, 190);
            this.FontName.Name = "FontName";
            this.FontName.Size = new System.Drawing.Size(117, 16);
            this.FontName.TabIndex = 8;
            // 
            // FontSize
            // 
            this.FontSize.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FontSize.Location = new System.Drawing.Point(131, 190);
            this.FontSize.Name = "FontSize";
            this.FontSize.Size = new System.Drawing.Size(56, 16);
            this.FontSize.TabIndex = 9;
            // 
            // CharIdx
            // 
            this.CharIdx.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.CharIdx.Location = new System.Drawing.Point(83, 148);
            this.CharIdx.Name = "CharIdx";
            this.CharIdx.Size = new System.Drawing.Size(31, 18);
            this.CharIdx.TabIndex = 10;
            // 
            // DoAntialias
            // 
            this.DoAntialias.BackColor = System.Drawing.Color.Transparent;
            this.DoAntialias.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.DoAntialias.Location = new System.Drawing.Point(131, 173);
            this.DoAntialias.Name = "DoAntialias";
            this.DoAntialias.Size = new System.Drawing.Size(64, 16);
            this.DoAntialias.TabIndex = 11;
            this.DoAntialias.Text = "Antialias";
            this.DoAntialias.CheckedChanged += new System.EventHandler(this.DoAntialias_CheckedChanged);
            // 
            // ImportBitmap
            // 
            this.ImportBitmap.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ImportBitmap.Location = new System.Drawing.Point(10, 256);
            this.ImportBitmap.Name = "ImportBitmap";
            this.ImportBitmap.Size = new System.Drawing.Size(177, 20);
            this.ImportBitmap.TabIndex = 12;
            this.ImportBitmap.Text = "Import Bitmap...";
            this.ImportBitmap.Click += new System.EventHandler(this.ImportBitmap_Click);
            // 
            // ImportProgress
            // 
            this.ImportProgress.Location = new System.Drawing.Point(12, 258);
            this.ImportProgress.Name = "ImportProgress";
            this.ImportProgress.Size = new System.Drawing.Size(173, 16);
            this.ImportProgress.TabIndex = 13;
            this.ImportProgress.Visible = false;
            // 
            // CreateFont
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(194, 278);
            this.Controls.Add(this.ImportProgress);
            this.Controls.Add(this.ImportBitmap);
            this.Controls.Add(this.DoAntialias);
            this.Controls.Add(this.CharIdx);
            this.Controls.Add(this.FontSize);
            this.Controls.Add(this.FontName);
            this.Controls.Add(this.BorderSize);
            this.Controls.Add(this.NextSym);
            this.Controls.Add(this.PrevSym);
            this.Controls.Add(this.CharBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ExportBitmap);
            this.Controls.Add(this.SelFont);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "CreateFont";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CreateFont";
            this.TopMost = true;
            this.Closed += new System.EventHandler(this.CreateFont_Closed);
            ((System.ComponentModel.ISupportInitialize)(this.BorderSize)).EndInit();
            this.ResumeLayout(false);

        }
		#endregion

        private void SelFont_Click(object sender, System.EventArgs e)
        {
            FontDialog fd = new FontDialog();
            fd.Font  = CurFont;
            if(fd.ShowDialog() != DialogResult.Cancel )
            {
                CurFont = (Font)fd.Font.Clone();
                ExportBitmap.Enabled        = true;
                if (CurChar == -1) CurChar  = 'A';
                CharIdx.Text                = CurChar.ToString();
                CharBox.Invalidate();
                FontName.Text = CurFont.Name;
                FontSize.Text = CurFont.Size.ToString();
            }
            GenerateFontBitmap();
        }
        
        static public int MeasureCharWidth( Graphics graphics, char c, Font font)
        {
            if (c == ' ') c = '_'; 
            char[] ch = new char[1];
            ch[0] = c;
            string           str     = new string( ch ); 
            StringFormat     format  = new StringFormat( StringFormat.GenericTypographic );
            RectangleF       rect    = new RectangleF( 0, 0, 1000, 1000 );
            CharacterRange[] ranges  = { new CharacterRange( 0, str.Length ) };
            Region[]         regions = new Region[1];
            format.SetMeasurableCharacterRanges( ranges );
            regions = graphics.MeasureCharacterRanges( str, font, rect, format );
            rect    = regions[0].GetBounds( graphics );
            return (int)(rect.Right + 1.0f);
        }

        private char GetChar( int code, byte charSet )
        {
            System.Text.Encoding encoding = System.Text.Encoding.GetEncoding( "windows-1252" );
            switch (charSet)
            {
                case 204: encoding = System.Text.Encoding.GetEncoding( "windows-1251" ); break; // RUSSIAN_CHARSET
                case 238: encoding = System.Text.Encoding.GetEncoding( "windows-1250" ); break; // EASTEUROPE_CHARSET
                case 186: encoding = System.Text.Encoding.GetEncoding( "windows-1257" ); break; // BALTIC_CHARSET
            }
            
            byte[] bytes  = new byte[1]; 
            bytes[0] = (byte)code;
            char[] chars = encoding.GetChars( bytes );

            return chars[0];
        }

        private void GenerateFontBitmap()
        {
            if (CurFont == null) return; 
            int         fontH   = (int)CurFont.GetHeight();
            int         border  = (int)BorderSize.Value;
            SolidBrush  br      = new SolidBrush( Color.White );
            SolidBrush  brBack  = new SolidBrush( Color.Black );
            int         maxW    = 0;
            Graphics graphics = Graphics.FromImage( Bmp );

            if (DoAntialias.Checked) 
            {
                graphics.TextRenderingHint = TextRenderingHint.AntiAliasGridFit;
            }
            else 
            {
                graphics.TextRenderingHint = TextRenderingHint.SingleBitPerPixelGridFit;
            }

            Chars.Clear();
            StringFormat fmt    = new StringFormat( StringFormat.GenericTypographic ); 
            for (int i = MinChar; i <= MaxChar; i++)
            {
                FontChar     c      = new FontChar();
                char         ch     = GetChar( i, CurFont.GdiCharSet );
                c.Code              = i;
                c.Width             = MeasureCharWidth( graphics, ch, CurFont );
                c.Height            = fontH + border;
                if (c.Width > maxW) maxW = (int)c.Width;
                Chars.Add( c );
            }
            graphics.Dispose();

            float area = (maxW + border*2)*(fontH + border*2)*(MaxChar - MinChar + 1);
            int bmpSide = 64;
            while (bmpSide*bmpSide < (int)area) bmpSide *= 2;
            Bmp.Dispose();
            Bmp = new Bitmap( bmpSide, bmpSide, PixelFormat.Format32bppArgb );
            graphics = Graphics.FromImage( Bmp );
            if (DoAntialias.Checked) graphics.TextRenderingHint = TextRenderingHint.AntiAliasGridFit;
            else graphics.TextRenderingHint = TextRenderingHint.SingleBitPerPixelGridFit;

            graphics.FillRectangle( brBack, 0, 0, bmpSide, bmpSide );
    
            int x = border;
            int y = border;
            for (int i = MinChar; i <= MaxChar; i++)
            {
                char[]      ch  = new char[1]; 
                ch[0] = GetChar( i, CurFont.GdiCharSet );
                string      s   = new string( ch ); 
                FontChar    c   = (FontChar)Chars[i - MinChar];
                
                if (x + c.Width + border*2 > bmpSide)
                {
                    x = border;
                    y += fontH + border*2;
                }
                graphics.DrawString( s, CurFont, br, x + border, y + border, fmt );
                c.X = x + border;
                c.Y = y + border;
                x += c.Width + border*2;                
            }
            graphics.Dispose();

            ReplicateAlpha( ref Bmp );

            GetFontFromBitmap();
        } // GenerateFontBitmap

        void GetFontFromBitmap()
        {
            int border = (int)BorderSize.Value;
            ImportProgress.Visible  = true;
            ImportProgress.Minimum  = MinChar;
            ImportProgress.Maximum  = MaxChar;
            ImportProgress.Value    = MinChar;
            ImportProgress.Step     = 1;
            for (int i = MinChar; i <= MaxChar; i++)
            {
                FontChar    c   = (FontChar)Chars[i - MinChar];
                Rectangle   rct = new Rectangle( c.X, c.Y, c.Width, c.Height );
                if (rct.Width == 0) continue;
                c.Bmp = Bmp.Clone( rct, PixelFormat.Format32bppArgb );
                ImportProgress.PerformStep();
                ImportProgress.Update();
            }
            ImportProgress.Visible = false;
            Update();
        } // GetFontFromBitmap

        private void pictureBox1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            if (CurChar == -1) return;
            int         tw      = CharBox.Width;
            int         th      = CharBox.Height;
            int         border  = (int)BorderSize.Value;
            Pen         pen     = new Pen( Color.Red, 1 );
            FontChar    c       = (FontChar)Chars[CurChar - MinChar];
            float       x       = CharBox.Left + tw/2 - c.Width/2 - border;
            float       y       = CharBox.Top  + th/2 - c.Height/2 - border;
            e.Graphics.DrawRectangle( pen, x - border, y - border, c.Width + border*2, c.Height + border*2 );
            e.Graphics.DrawImage( c.Bmp, x, y );
        }

        private void BorderSize_ValueChanged(object sender, System.EventArgs e)
        {
            GenerateFontBitmap();
            CharBox.Invalidate();
        }

        private void NextSym_Click(object sender, System.EventArgs e)
        {
            CurChar++;
            if (CurChar > MaxChar) CurChar = MaxChar;
            CharIdx.Text = CurChar.ToString();
            CharBox.Invalidate();
        }

        private void PrevSym_Click(object sender, System.EventArgs e)
        {
            CurChar--;
            if (CurChar < MinChar) CurChar = MinChar;
            CharIdx.Text = CurChar.ToString();
            CharBox.Invalidate();
        }

        private void DoAntialias_CheckedChanged(object sender, System.EventArgs e)
        {
            GenerateFontBitmap();
            CharBox.Invalidate();
        }

        private void ExportBitmap_Click(object sender, System.EventArgs e)
        {
            SaveFileDialog sd = new SaveFileDialog();
            sd.Filter           = "Tiff images (*.tiff)|*.tiff";
            sd.FilterIndex      = 1 ;
            sd.RestoreDirectory = true ;
 
            if(sd.ShowDialog() == DialogResult.OK)
            {
                Stream os = sd.OpenFile();
                if (os != null)
                {
                    Bmp.Save( os, ImageFormat.Tiff  );
                    os.Close();
                }
            }
        }

        private void ImportBitmap_Click(object sender, System.EventArgs e)
        {
            OpenFileDialog od = new OpenFileDialog();
            od.Filter           = "Tiff images (*.tiff)|*.tiff"  ;
            od.FilterIndex      = 1 ;
            od.RestoreDirectory = true ;
 
            if(od.ShowDialog() == DialogResult.OK)
            {
                Bitmap tmp = new Bitmap( od.FileName );
                RectangleF rct = new RectangleF(0, 0, tmp.Width, tmp.Height);
                Bmp = tmp.Clone( rct, PixelFormat.Format32bppArgb );
                GetFontFromBitmap();
            }
        }
        
        private void ReplicateAlpha( ref Bitmap bmp )
        {
            for (int i = 0; i < bmp.Width; i++)
            {
                for (int j = 0; j < bmp.Height; j++)
                {
                    Color  clr = bmp.GetPixel( i, j );
                    int r = clr.R;
                    int g = clr.G;
                    int b = clr.B;
                    int a = (r + g + b)/3;
                    clr = Color.FromArgb( a, r, g, b );
                    bmp.SetPixel( i, j, clr );
                }
            }
        } // ReplicateAlpha

        private void CreateFont_Closed(object sender, System.EventArgs e)
        {
            MainForm.Instance.Visible = true;

            for (int i = MinChar; i <= MaxChar; i++)
            {
                char[]      ch  = new char[1]; ch[0] = (char)i;
                string      str = new string( ch ); 
                if (Chars.Count <= i - MinChar) continue;
                FontChar    c   = (FontChar)Chars[i - MinChar];
                
                ColorFormat colorFmt = (ColorFormat)MainForm.Instance.ColorFormatBox.SelectedIndex;
				bool bBorder = MainForm.Instance.IsFiltered.Checked;
                MainForm.Instance.SPack.Sprites.Add( new Sprite( str, "", c.Bmp, colorFmt, bBorder ) );                
            }
            MainForm.Instance.UpdateFileList();
        }
	}

    public class FontChar
    {
        public int          Code;
        public Bitmap       Bmp = new Bitmap( 1, 1, PixelFormat.Format32bppArgb );
        public int          Width;
        public int          Height;
        public int          X;
        public int          Y;
    } // class FontChar
}
