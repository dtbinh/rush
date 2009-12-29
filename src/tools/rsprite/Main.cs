using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;


namespace rtSprite
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private System.Windows.Forms.Button MoveUp;
        private System.Windows.Forms.Button MoveDown;
        private System.Windows.Forms.Button Del;
        private System.Windows.Forms.Button Clear;
        private System.Windows.Forms.Button Sort;
        private System.Windows.Forms.MainMenu mainMenu2;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItem2;
        private System.Windows.Forms.MenuItem menuItem4;
        private System.Windows.Forms.MenuItem menuItem6;
        private System.Windows.Forms.MenuItem menuItem7;
        private System.Windows.Forms.MenuItem menuItem8;
        private System.Windows.Forms.MenuItem menuItem9;
        private System.Windows.Forms.MenuItem menuItem10;
        public  System.Windows.Forms.MenuItem ItemPreview;
        private System.Windows.Forms.MenuItem OpenPackage;
        private System.Windows.Forms.MenuItem SavePackage;
        public  System.Windows.Forms.MenuItem ShowQuads;
        private System.Windows.Forms.MenuItem menuItem3;
        private System.Windows.Forms.MenuItem CreateFontItem;
        private System.Windows.Forms.GroupBox FrameProps;
        private System.Windows.Forms.Label SpriteSize;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.CheckBox IsFiltered;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.ComboBox ColorFormatBox;
        private System.Windows.Forms.Label NumQuads;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label FrameIndex;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.MenuItem menuItem5;
        private System.Windows.Forms.MenuItem ShowAlpha;
        private System.Windows.Forms.Label Pivot;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label Shift;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.MenuItem menuItem11;
        private System.Windows.Forms.MenuItem ItemDropTarget;
        private System.Windows.Forms.MenuItem ClonePivot;
        private System.Windows.Forms.MenuItem ConvertToDiff;
        private System.Windows.Forms.MenuItem ShrinkTwice;
        public static MainForm  Instance;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}


		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
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
            this.FilesList = new System.Windows.Forms.ListBox();
            this.MoveUp = new System.Windows.Forms.Button();
            this.MoveDown = new System.Windows.Forms.Button();
            this.Del = new System.Windows.Forms.Button();
            this.Clear = new System.Windows.Forms.Button();
            this.Sort = new System.Windows.Forms.Button();
            this.mainMenu2 = new System.Windows.Forms.MainMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this.OpenPackage = new System.Windows.Forms.MenuItem();
            this.menuItem4 = new System.Windows.Forms.MenuItem();
            this.SavePackage = new System.Windows.Forms.MenuItem();
            this.menuItem6 = new System.Windows.Forms.MenuItem();
            this.menuItem7 = new System.Windows.Forms.MenuItem();
            this.menuItem10 = new System.Windows.Forms.MenuItem();
            this.ItemPreview = new System.Windows.Forms.MenuItem();
            this.ItemDropTarget = new System.Windows.Forms.MenuItem();
            this.ShowQuads = new System.Windows.Forms.MenuItem();
            this.ShowAlpha = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.CreateFontItem = new System.Windows.Forms.MenuItem();
            this.menuItem5 = new System.Windows.Forms.MenuItem();
            this.menuItem11 = new System.Windows.Forms.MenuItem();
            this.ClonePivot = new System.Windows.Forms.MenuItem();
            this.ConvertToDiff = new System.Windows.Forms.MenuItem();
            this.ShrinkTwice = new System.Windows.Forms.MenuItem();
            this.menuItem8 = new System.Windows.Forms.MenuItem();
            this.menuItem9 = new System.Windows.Forms.MenuItem();
            this.FrameProps = new System.Windows.Forms.GroupBox();
            this.Shift = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.Pivot = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.FrameIndex = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.NumQuads = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.SpriteSize = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.IsFiltered = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ColorFormatBox = new System.Windows.Forms.ComboBox();
            this.FrameProps.SuspendLayout();
            this.SuspendLayout();
            // 
            // FilesList
            // 
            this.FilesList.AllowDrop = true;
            this.FilesList.BackColor = System.Drawing.SystemColors.Info;
            this.FilesList.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.FilesList.Location = new System.Drawing.Point(1, 1);
            this.FilesList.Name = "FilesList";
            this.FilesList.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.FilesList.Size = new System.Drawing.Size(164, 288);
            this.FilesList.TabIndex = 0;
            this.FilesList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FilesList_KeyDown);
            this.FilesList.DragOver += new System.Windows.Forms.DragEventHandler(this.listBox1_DragOver);
            this.FilesList.DragDrop += new System.Windows.Forms.DragEventHandler(this.listBox1_DragDrop);
            this.FilesList.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // MoveUp
            // 
            this.MoveUp.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.MoveUp.Location = new System.Drawing.Point(167, 2);
            this.MoveUp.Name = "MoveUp";
            this.MoveUp.Size = new System.Drawing.Size(122, 21);
            this.MoveUp.TabIndex = 7;
            this.MoveUp.Text = "Move Up";
            this.MoveUp.Click += new System.EventHandler(this.MoveUpClick);
            // 
            // MoveDown
            // 
            this.MoveDown.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.MoveDown.Location = new System.Drawing.Point(167, 24);
            this.MoveDown.Name = "MoveDown";
            this.MoveDown.Size = new System.Drawing.Size(122, 21);
            this.MoveDown.TabIndex = 8;
            this.MoveDown.Text = "Move Down";
            this.MoveDown.Click += new System.EventHandler(this.MoveDownClick);
            // 
            // Del
            // 
            this.Del.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Del.Location = new System.Drawing.Point(167, 50);
            this.Del.Name = "Del";
            this.Del.Size = new System.Drawing.Size(122, 21);
            this.Del.TabIndex = 9;
            this.Del.Text = "Delete";
            this.Del.Click += new System.EventHandler(this.DelClick);
            // 
            // Clear
            // 
            this.Clear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Clear.Location = new System.Drawing.Point(167, 98);
            this.Clear.Name = "Clear";
            this.Clear.Size = new System.Drawing.Size(122, 21);
            this.Clear.TabIndex = 11;
            this.Clear.Text = "Clear";
            this.Clear.Click += new System.EventHandler(this.ClearClick);
            // 
            // Sort
            // 
            this.Sort.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Sort.Location = new System.Drawing.Point(167, 120);
            this.Sort.Name = "Sort";
            this.Sort.Size = new System.Drawing.Size(122, 21);
            this.Sort.TabIndex = 13;
            this.Sort.Text = "Sort";
            this.Sort.Click += new System.EventHandler(this.Sort_Click);
            // 
            // mainMenu2
            // 
            this.mainMenu2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                      this.menuItem1,
                                                                                      this.menuItem10,
                                                                                      this.menuItem3,
                                                                                      this.menuItem8});
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 0;
            this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                      this.menuItem2,
                                                                                      this.OpenPackage,
                                                                                      this.menuItem4,
                                                                                      this.SavePackage,
                                                                                      this.menuItem6,
                                                                                      this.menuItem7});
            this.menuItem1.Text = "File";
            // 
            // menuItem2
            // 
            this.menuItem2.Index = 0;
            this.menuItem2.Text = "New";
            this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
            // 
            // OpenPackage
            // 
            this.OpenPackage.Index = 1;
            this.OpenPackage.Text = "Open";
            this.OpenPackage.Click += new System.EventHandler(this.OpenPackage_Click);
            // 
            // menuItem4
            // 
            this.menuItem4.Index = 2;
            this.menuItem4.Text = "-";
            // 
            // SavePackage
            // 
            this.SavePackage.Index = 3;
            this.SavePackage.Text = "Save As...";
            this.SavePackage.Click += new System.EventHandler(this.SavePackage_Click);
            // 
            // menuItem6
            // 
            this.menuItem6.Index = 4;
            this.menuItem6.Text = "-";
            // 
            // menuItem7
            // 
            this.menuItem7.Index = 5;
            this.menuItem7.Text = "Exit";
            this.menuItem7.Click += new System.EventHandler(this.menuItem7_Click);
            // 
            // menuItem10
            // 
            this.menuItem10.Index = 1;
            this.menuItem10.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                       this.ItemPreview,
                                                                                       this.ItemDropTarget,
                                                                                       this.ShowQuads,
                                                                                       this.ShowAlpha});
            this.menuItem10.Text = "View";
            // 
            // ItemPreview
            // 
            this.ItemPreview.Index = 0;
            this.ItemPreview.Text = "Preview Window";
            this.ItemPreview.Click += new System.EventHandler(this.ItemPreview_Click);
            // 
            // ItemDropTarget
            // 
            this.ItemDropTarget.Index = 1;
            this.ItemDropTarget.Text = "Drop Target";
            this.ItemDropTarget.Click += new System.EventHandler(this.menuItem12_Click);
            // 
            // ShowQuads
            // 
            this.ShowQuads.Enabled = false;
            this.ShowQuads.Index = 2;
            this.ShowQuads.Text = "Show Quads";
            this.ShowQuads.Click += new System.EventHandler(this.ShowQuads_Click);
            // 
            // ShowAlpha
            // 
            this.ShowAlpha.Enabled = false;
            this.ShowAlpha.Index = 3;
            this.ShowAlpha.Text = "Show Alpha";
            this.ShowAlpha.Click += new System.EventHandler(this.ShowAlpha_Click);
            // 
            // menuItem3
            // 
            this.menuItem3.Index = 2;
            this.menuItem3.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                      this.CreateFontItem,
                                                                                      this.menuItem5,
                                                                                      this.menuItem11,
                                                                                      this.ClonePivot,
                                                                                      this.ConvertToDiff,
                                                                                      this.ShrinkTwice});
            this.menuItem3.Text = "Tools";
            // 
            // CreateFontItem
            // 
            this.CreateFontItem.Index = 0;
            this.CreateFontItem.Text = "Create Font...";
            this.CreateFontItem.Click += new System.EventHandler(this.CreateFontItem_Click);
            // 
            // menuItem5
            // 
            this.menuItem5.Index = 1;
            this.menuItem5.Text = "Options...";
            this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
            // 
            // menuItem11
            // 
            this.menuItem11.Index = 2;
            this.menuItem11.Text = "Reset Pivots";
            this.menuItem11.Click += new System.EventHandler(this.menuItem11_Click);
            // 
            // ClonePivot
            // 
            this.ClonePivot.Index = 3;
            this.ClonePivot.Shortcut = System.Windows.Forms.Shortcut.CtrlP;
            this.ClonePivot.Text = "Clone Pivot";
            this.ClonePivot.Click += new System.EventHandler(this.ClonePivot_Click);
            // 
            // ConvertToDiff
            // 
            this.ConvertToDiff.Index = 4;
            this.ConvertToDiff.Text = "Convert to Diff";
            this.ConvertToDiff.Click += new System.EventHandler(this.ConvertToDiff_Click);
            // 
            // ShrinkTwice
            // 
            this.ShrinkTwice.Index = 5;
            this.ShrinkTwice.Text = "Shrink Twice";
            this.ShrinkTwice.Click += new System.EventHandler(this.ShrinkTwice_Click);
            // 
            // menuItem8
            // 
            this.menuItem8.Index = 3;
            this.menuItem8.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                      this.menuItem9});
            this.menuItem8.Text = "Help";
            // 
            // menuItem9
            // 
            this.menuItem9.Index = 0;
            this.menuItem9.Text = "About";
            // 
            // FrameProps
            // 
            this.FrameProps.Controls.Add(this.Shift);
            this.FrameProps.Controls.Add(this.label8);
            this.FrameProps.Controls.Add(this.Pivot);
            this.FrameProps.Controls.Add(this.label6);
            this.FrameProps.Controls.Add(this.FrameIndex);
            this.FrameProps.Controls.Add(this.label4);
            this.FrameProps.Controls.Add(this.NumQuads);
            this.FrameProps.Controls.Add(this.label5);
            this.FrameProps.Controls.Add(this.SpriteSize);
            this.FrameProps.Controls.Add(this.label2);
            this.FrameProps.Controls.Add(this.IsFiltered);
            this.FrameProps.Controls.Add(this.label1);
            this.FrameProps.Controls.Add(this.ColorFormatBox);
            this.FrameProps.Location = new System.Drawing.Point(167, 150);
            this.FrameProps.Name = "FrameProps";
            this.FrameProps.Size = new System.Drawing.Size(126, 139);
            this.FrameProps.TabIndex = 16;
            this.FrameProps.TabStop = false;
            this.FrameProps.Text = "Sprite Properties:";
            // 
            // Shift
            // 
            this.Shift.Location = new System.Drawing.Point(44, 118);
            this.Shift.Name = "Shift";
            this.Shift.Size = new System.Drawing.Size(76, 14);
            this.Shift.TabIndex = 28;
            this.Shift.Text = "{0, 0}";
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(13, 118);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(33, 14);
            this.label8.TabIndex = 27;
            this.label8.Text = "Shift:";
            // 
            // Pivot
            // 
            this.Pivot.Location = new System.Drawing.Point(44, 102);
            this.Pivot.Name = "Pivot";
            this.Pivot.Size = new System.Drawing.Size(76, 14);
            this.Pivot.TabIndex = 26;
            this.Pivot.Text = "{0, 0}";
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(10, 102);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(35, 14);
            this.label6.TabIndex = 25;
            this.label6.Text = "Pivot:";
            // 
            // FrameIndex
            // 
            this.FrameIndex.Location = new System.Drawing.Point(44, 86);
            this.FrameIndex.Name = "FrameIndex";
            this.FrameIndex.Size = new System.Drawing.Size(76, 14);
            this.FrameIndex.TabIndex = 24;
            this.FrameIndex.Text = "0";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(8, 86);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(38, 14);
            this.label4.TabIndex = 23;
            this.label4.Text = "Index:";
            // 
            // NumQuads
            // 
            this.NumQuads.Location = new System.Drawing.Point(44, 70);
            this.NumQuads.Name = "NumQuads";
            this.NumQuads.Size = new System.Drawing.Size(76, 14);
            this.NumQuads.TabIndex = 22;
            this.NumQuads.Text = "0";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(3, 70);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(49, 14);
            this.label5.TabIndex = 21;
            this.label5.Text = "Quads:";
            // 
            // SpriteSize
            // 
            this.SpriteSize.Location = new System.Drawing.Point(44, 54);
            this.SpriteSize.Name = "SpriteSize";
            this.SpriteSize.Size = new System.Drawing.Size(76, 14);
            this.SpriteSize.TabIndex = 20;
            this.SpriteSize.Text = "100x100";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(14, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 14);
            this.label2.TabIndex = 19;
            this.label2.Text = "Size:";
            // 
            // IsFiltered
            // 
            this.IsFiltered.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.IsFiltered.ForeColor = System.Drawing.Color.Black;
            this.IsFiltered.Location = new System.Drawing.Point(4, 16);
            this.IsFiltered.Name = "IsFiltered";
            this.IsFiltered.Size = new System.Drawing.Size(79, 14);
            this.IsFiltered.TabIndex = 18;
            this.IsFiltered.Text = "Stretchable";
            // 
            // label1
            // 
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(2, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 15);
            this.label1.TabIndex = 17;
            this.label1.Text = "Format:";
            // 
            // ColorFormatBox
            // 
            this.ColorFormatBox.BackColor = System.Drawing.SystemColors.Info;
            this.ColorFormatBox.Items.AddRange(new object[] {
                                                                "ARGB4444",
                                                                "ARGB8888",
                                                                "RGB565",
                                                                "RGB888",
                                                                "A8"});
            this.ColorFormatBox.Location = new System.Drawing.Point(46, 32);
            this.ColorFormatBox.Name = "ColorFormatBox";
            this.ColorFormatBox.Size = new System.Drawing.Size(78, 21);
            this.ColorFormatBox.TabIndex = 16;
            this.ColorFormatBox.Text = "ARGB4444";
            // 
            // MainForm
            // 
            this.AllowDrop = true;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(297, 291);
            this.Controls.Add(this.FrameProps);
            this.Controls.Add(this.Sort);
            this.Controls.Add(this.Clear);
            this.Controls.Add(this.Del);
            this.Controls.Add(this.MoveDown);
            this.Controls.Add(this.MoveUp);
            this.Controls.Add(this.FilesList);
            this.ForeColor = System.Drawing.Color.Black;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Menu = this.mainMenu2;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "rtsprite";
            this.TopMost = true;
            this.TransparencyKey = System.Drawing.Color.FromArgb(((System.Byte)(192)), ((System.Byte)(192)), ((System.Byte)(0)));
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MainForm_MouseDown);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.DragOver += new System.Windows.Forms.DragEventHandler(this.MainForm_DragOver);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MainForm_MouseMove);
            this.FrameProps.ResumeLayout(false);
            this.ResumeLayout(false);

        }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
            using (Instance = new MainForm())
            {
                Instance.Show();

                // While the form is still valid, render and process messages
                while(Instance.Created)
                {
                    Application.DoEvents();
                }
            }
		}
        private Point                               m_MousePos;
        private Bitmap                              CurImage;
        private System.Windows.Forms.ListBox        FilesList;
        private FramePreview                        Preview = new FramePreview();
        private DropTarget                          Convertor = new DropTarget();
        public  SpritePackage                       SPack = new SpritePackage();



        private void MainForm_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            m_MousePos = new Point(-e.X, -e.Y);
        }

        private void MainForm_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left) 
            {
                Point mousePos = Control.MousePosition;
                mousePos.Offset(m_MousePos.X, m_MousePos.Y);
                Location = mousePos;
            }
        }
        
        private bool IsImageFile( string s )
        {
            string ext = Path.GetExtension( s ).ToLower();
            if (ext == ".jpg" || ext == ".bmp" || ext == ".tiff" || ext == ".png") return true;
            if (ext != ".tga") return false;
            FileStream fs = File.Open( s, FileMode.Open, FileAccess.Read, FileShare.None );
            long sz = fs.Length;
            byte[] hdr = new byte[18];
            fs.Read( hdr, 0, 18 );
            if (hdr[1] != 0 || (hdr[2] != 2 && hdr[2] != 3)) return false;
            int width		= hdr[0 + 12] + hdr[1 + 12]*256; 
            int height		= hdr[2 + 12] + hdr[3 + 12]*256;
            int bitspp	    = hdr[4 + 12]; 

            CurImage = new Bitmap( width, height );
            int nPix = width*height;
            if (bitspp != 32 && bitspp != 24) return false;
            fs.Close();
            return true;
        }

        private static bool LoadImage( string fname, ref Bitmap img )
        {
            string ext = Path.GetExtension( fname ).ToLower();
            if (ext == ".jpg" || ext == ".bmp" || ext == ".tiff" || ext == ".png") 
            {
                try
                {
                    img = new Bitmap( fname );
                    return true;
                }
                catch (Exception)
                {
                    return false;
                }
            }
            
            //  load as tga
            FileStream fs;
            try
            {
                fs = File.Open( fname, FileMode.Open, FileAccess.Read, FileShare.None );
            }
            catch (Exception)
            {
                return false;
            }

            long sz = fs.Length;
            byte[] hdr = new byte[18];
            fs.Read( hdr, 0, 18 );
            if (hdr[1] != 0 || (hdr[2] != 2 && hdr[2] != 3)) return false;
            int width		= hdr[0 + 12] + hdr[1 + 12]*256; 
            int height		= hdr[2 + 12] + hdr[3 + 12]*256;
            int bitspp	= hdr[4 + 12]; 

            img = new Bitmap( width, height );
            int nPix = width*height;
            int bytespp = 0;
            if (bitspp == 32) bytespp = 4;
            else if (bitspp == 24) bytespp = 3;
            else return false;

            byte[] clr = new byte[bytespp];
            for (int j = height - 1; j >= 0; j--)
            {
                for (int i = 0; i < width; i++)
                {
                    fs.Read( clr, 0, bytespp );
                    Color color;
                    byte A = 255;
                    if (bytespp == 4) A = clr[3];
                    color = Color.FromArgb( A, clr[2], clr[1], clr[0] );
                    img.SetPixel( i, j, color );
                }
            }
            return true;
        }

        private void listBox1_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(DataFormats.FileDrop)) return;
            string[] filenames = (string[]) e.Data.GetData(DataFormats.FileDrop);
            foreach( string s in filenames ) 
            {
                if (!IsImageFile( s )) 
                {
                    if (Path.GetExtension( s ) == ".rbs" || Path.GetExtension( s ) == ".rspr")
                        OpenSpritePack( s );
                    continue; 
                }
                Bitmap img = new Bitmap(1,1);
                if (!LoadImage( s, ref img )) continue;
                ColorFormat colorFmt = (ColorFormat)ColorFormatBox.SelectedIndex;
                SPack.Sprites.Add( new Sprite( Path.GetFileName( s ), s, img, colorFmt, IsFiltered.Checked ) );
            }
            UpdateFileList();
        }

        public void UpdateFileList()
        {
            FilesList.Items.Clear();
            foreach( Sprite s in SPack.Sprites )
            {
                FilesList.Items.Add( s.Name );
            }
            FilesList.Update();
            Refresh();
        }

        private void listBox1_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            UpdateSpriteInfo();
        }

        public void UpdateSpriteInfo()
        {
            int idx = FilesList.SelectedIndex;
            SetPreviewPicture( idx );

            if (idx == -1) return;
            Sprite sp = (Sprite)SPack.Sprites[idx];
            ColorFormatBox.SelectedIndex = (int)sp.ClrFormat;
            IsFiltered.Checked = sp.Filtered;
            SpriteSize.Text = sp.Width + "x" + sp.Height;
            FrameIndex.Text = "" + idx;
            NumQuads.Text = "" + sp.Partition.Rects.Count;
            Pivot.Text = "{" + sp.PivotX + ", " + sp.PivotY + "}";
            Shift.Text = "{" + sp.ShiftX + ", " + sp.ShiftY + "}";
        }

        private void SetPreviewPicture( int idx )
        {
            if (idx < 0 || idx >= SPack.Count) return;
            string fname = ((Sprite)SPack.GetSprite( idx )).FilePath;
            if (CurImage != null) CurImage.Dispose();

            CurImage = ((Sprite)SPack.GetSprite( idx )).GetBitmap( ShowAlpha.Checked );

            if (Preview.Visible)
            {
                Preview.ThumbBox.Image = (Image)CurImage;
                int ncW = Preview.Width - Preview.ClientSize.Width;
                int ncH = Preview.Height - Preview.ClientSize.Height;
                Preview.Width  = ncW + CurImage.Width + 2;
                Preview.Height = ncH + CurImage.Height + 2;
                Preview.CurSprite = ((Sprite)SPack.GetSprite( idx ));
            }
            Update();
        }

        private void MainForm_DragOver(object sender, System.Windows.Forms.DragEventArgs e)
        {
            e.Effect =  DragDropEffects.Move;
        }

        private void listBox1_DragOver(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.All;
            }
        }

        private void DelClick(object sender, System.EventArgs e)
        {
            int idx = FilesList.SelectedIndex;
            if (idx < 0 || idx >= SPack.Sprites.Count) return;
            SPack.RemoveAt( idx );
            FilesList.Items.RemoveAt( idx );
            if (idx == SPack.Sprites.Count) idx--;
            if (idx > 0) FilesList.SelectedIndex = idx;
        }
        
        private void DoMoveUp()
        {
            foreach (int idx in FilesList.SelectedIndices)
            {
                if (idx >= FilesList.Items.Count || idx <= 0) return;
                object s = SPack.GetSprite( idx );
                SPack.RemoveAt( idx );
                SPack.Insert( idx - 1, s );

                object its = FilesList.Items[idx];
                FilesList.Items.RemoveAt( idx );
                FilesList.Items.Insert( idx - 1, its );
                FilesList.SelectedIndex = idx - 1;
            }
        }

        private void DoMoveDown()
        {
            foreach (int idx in FilesList.SelectedIndices)
            {
                if (idx >= FilesList.Items.Count - 1 || idx < 0) return;
                object s = SPack.GetSprite( idx );
                SPack.RemoveAt( idx );
                SPack.Insert( idx + 1, s );

                object its = FilesList.Items[idx];
                FilesList.Items.RemoveAt( idx );
                FilesList.Items.Insert( idx + 1, its );
                FilesList.SelectedIndex = idx + 1;
            }
        }


        private void MoveUpClick(object sender, System.EventArgs e)
        {
            DoMoveUp();
        }

        private void MoveDownClick(object sender, System.EventArgs e)
        {
            DoMoveDown();
        }

        private void ClearClick(object sender, System.EventArgs e)
        {
            SPack.Clear();
            UpdateFileList();
        }

        private bool OpenSpritePack( string fname )
        {
            FileStream ins = File.OpenRead( fname );
            if (ins == null) return false;
            SPack.Read( ins, Path.GetFileNameWithoutExtension( fname ) );
            ins.Close();
            UpdateFileList();
            Text = "rtsprite - " + Path.GetFileNameWithoutExtension( fname );
            return true;
        }

        private void MainForm_Load(object sender, System.EventArgs e)
        {
            String[] arguments = Environment.GetCommandLineArgs();
            if (arguments.Length > 1)
            {
                string fname = arguments[1];
                OpenSpritePack( fname );
                if (SPack.Sprites.Count > 0)
                {
                    FilesList.SelectedIndex = 0;
                    ShowPreview( true );
                }
            }
        }

        private void Sort_Click(object sender, System.EventArgs e)
        {
            SPack.Sort();
            UpdateFileList();
        }

        private void ShowPreview( bool bShow )
        {
            if (bShow)
            {
                Preview                 = new FramePreview();
                Preview.Visible         = true;
                SetPreviewPicture( FilesList.SelectedIndex );
                ItemPreview.Checked     = true;
                ShowQuads.Enabled       = true;
                ShowAlpha.Enabled       = true;
            }
            else
            {
                Preview.Visible         = false;
                Preview.Dispose();
                ItemPreview.Checked     = false;
                ShowQuads.Enabled       = false;
                ShowAlpha.Enabled       = false;
            }
        }

        private void ItemPreview_Click(object sender, System.EventArgs e)
        {
            ShowPreview( !ItemPreview.Checked );
        }

        private void OpenPackage_Click(object sender, System.EventArgs e)
        {
            Stream ins;
            OpenFileDialog ofd   = new OpenFileDialog();
            ofd.Filter           = "Sprite packages (*.rspr)|*.rspr|All files (*.*)|*.*" ;
            ofd.FilterIndex      = 1;
            ofd.RestoreDirectory = true ;
 
            if(ofd.ShowDialog() == DialogResult.OK)
            {
                ins = ofd.OpenFile();
                if (ins != null)
                {
                    SPack.Read( ins, Path.GetFileNameWithoutExtension( ofd.FileName ) );
                    ins.Close();
                    Text = "rtsprite - " + Path.GetFileNameWithoutExtension( ofd.FileName );
                }
                UpdateFileList();
            }
        }

        private void SavePackage_Click(object sender, System.EventArgs e)
        {
            Stream os ;
            SaveFileDialog sfd   = new SaveFileDialog();
            sfd.Filter           = "Sprite packages (*.rspr)|*.rspr|All files (*.*)|*.*" ;
            sfd.FilterIndex      = 1;
            sfd.RestoreDirectory = true ;
 
            if(sfd.ShowDialog() == DialogResult.OK)
            {
                os = sfd.OpenFile();
                if (os != null)
                {
                    SPack.Write( os );
                    os.Close();
                }
            }
        }

        private void ShowQuads_Click(object sender, System.EventArgs e)
        {
            if (!ShowQuads.Checked)
            {
                ShowQuads.Checked     = true;
            }
            else
            {
                ShowQuads.Checked     = false;
            }
            Preview.Refresh();
        }

        private void CreateFontItem_Click(object sender, System.EventArgs e)
        {
            CreateFont cf = new CreateFont();
            Visible = false;
            cf.Show();
        }

        private void menuItem7_Click(object sender, System.EventArgs e)
        {
            Close();
        }

        private void menuItem2_Click(object sender, System.EventArgs e)
        {
            SPack.Clear();
            UpdateFileList();
        }

        private void menuItem5_Click(object sender, System.EventArgs e)
        {
            Options options = new Options();
            options.MinQuadSide         = RectPartition.MinQuadSide;
            options.MaxQuadSide         = RectPartition.MaxQuadSide;
            options.spMinQuadSide.Value = RectPartition.MinQuadSide;
            options.spMaxQuadSide.Value = RectPartition.MaxQuadSide;
            options.ShowDialog();
            RectPartition.MinQuadSide   = options.MinQuadSide;
            RectPartition.MaxQuadSide   = options.MaxQuadSide;
        }

        private void ShowAlpha_Click(object sender, System.EventArgs e)
        {
            if (!ShowAlpha.Checked)
            {
                ShowAlpha.Checked     = true;
            }
            else
            {
                ShowAlpha.Checked     = false;
            }
            SetPreviewPicture( FilesList.SelectedIndex );
            Preview.Refresh();
        }

        private void FilesList_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Up && e.Control) 
            {
                DoMoveUp();
            }

            if (e.KeyCode == Keys.Down && e.Control) 
            {
                DoMoveDown();
            }
        }

        private void menuItem11_Click(object sender, System.EventArgs e)
        {
            SPack.ResetPivots();
        }

        private void menuItem12_Click(object sender, System.EventArgs e)
        {
            if (!ItemDropTarget.Checked)
            {
                Convertor                 = new DropTarget();
                Convertor.Visible         = true;
                ItemDropTarget.Checked  = true;
            }
            else
            {
                Convertor.Visible         = false;
                Convertor.Dispose();
                ItemDropTarget.Checked  = false;
            }
        }

        private void ClonePivot_Click(object sender, System.EventArgs e)
        {
            if (FilesList.SelectedIndices.Count == 0)
            {
                return;
            }
            int firstIdx = FilesList.SelectedIndices[0];
            Sprite firstSprite = (Sprite)SPack.Sprites[firstIdx];
            short pivotX = firstSprite.PivotX;
            short pivotY = firstSprite.PivotY;
            short shiftX = firstSprite.ShiftX;
            short shiftY = firstSprite.ShiftY;
            foreach (int idx in FilesList.SelectedIndices)
            {
                if (idx <= 0) continue;
                Sprite sprite = (Sprite)SPack.Sprites[idx];
                sprite.PivotX = pivotX;
                sprite.PivotY = pivotY;
                sprite.ShiftX = shiftX;
                sprite.ShiftY = shiftY;
            }
        }

        private void ConvertToDiff_Click(object sender, System.EventArgs e)
        {
            if (FilesList.SelectedIndices.Count == 0)
            {
                return;
            }
            int firstIdx = FilesList.SelectedIndices[0];
            Sprite firstSprite = (Sprite)SPack.Sprites[firstIdx];
            foreach (int idx in FilesList.SelectedIndices)
            {
                if (idx <= 0) continue;
                Sprite sprite = (Sprite)SPack.Sprites[idx];
                sprite.Subtract( ref firstSprite );
            }
        }

        private void ShrinkTwice_Click(object sender, System.EventArgs e)
        {
            if (FilesList.SelectedIndices.Count == 0)
            {
                return;
            }
            foreach (int idx in FilesList.SelectedIndices)
            {
                Sprite sprite = (Sprite)SPack.Sprites[idx];
                sprite.ShrinkTwice();
            }
        }

	}
}
